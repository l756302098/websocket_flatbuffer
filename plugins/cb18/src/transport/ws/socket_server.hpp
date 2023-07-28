#pragma once
#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <memory>
#include <set>
#include <websocketpp/common/thread.hpp>
#include "ws_message.hpp"

using websocketpp::connection_hdl;
using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

using websocketpp::lib::condition_variable;
using websocketpp::lib::lock_guard;
using websocketpp::lib::mutex;
using websocketpp::lib::thread;
using websocketpp::lib::unique_lock;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;

namespace swr
{
    enum action_type
    {
        SUBSCRIBE,
        UNSUBSCRIBE,
        MESSAGE
    };

    struct action
    {
        action(action_type t, connection_hdl h) : type(t), hdl(h) {}
        action(action_type t, connection_hdl h, server::message_ptr m)
            : type(t), hdl(h), msg(m) {}

        action_type type;
        websocketpp::connection_hdl hdl;
        server::message_ptr msg;
    };

    class socket_server
    {
    public:
        socket_server(uint16_t port) : m_port(port),running(true)
        {
            // Initialize Asio Transport
            m_server.set_access_channels(websocketpp::log::alevel::none);
            // m_server.clear_access_channels(websocketpp::log::alevel::all);
            // m_server.clear_error_channels(websocketpp::log::alevel::all);
            m_server.init_asio();

            // Register handler callbacks
            m_server.set_reuse_addr(true);
            m_server.set_open_handler(bind(&socket_server::on_open, this, ::_1));
            m_server.set_close_handler(bind(&socket_server::on_close, this, ::_1));
            m_server.set_message_handler(bind(&socket_server::on_message, this, ::_1, ::_2));

            // Start a thread to run the processing loop
            p_thread.reset(new websocketpp::lib::thread(websocketpp::lib::bind(&socket_server::process_messages, this)));
            p_thread->detach();
            s_thread = std::make_shared<std::thread>(std::bind(&socket_server::send_process, this));
            s_thread->detach();
        }

        ~socket_server(){
            std::cout << "start ~socket_server" << std::endl;
            this->open_func = nullptr;
            this->close_func = nullptr;
            this->msg_func = nullptr;
            std::cout << "end ~socket_server" << std::endl;
        }

        void set_open(std::function<void()> func)
        {
            this->open_func = func;
        }

        void set_close(std::function<void()> func)
        {
            this->close_func = func;
        }

        void set_handler(std::function<void(server::message_ptr)> func)
        {
            this->msg_func = func;
        }

        void start()
        {
            socket_thread.reset(new websocketpp::lib::thread(&socket_server::run, this));
            socket_thread->detach();
        }

        void stop()
        {
            if(m_server.is_listening())
            {
                m_server.stop_listening();
            }
            running = false;
            m_action_cond.notify_all();
            if (p_thread != nullptr && p_thread->joinable())
            {
                p_thread->join();
                p_thread = nullptr;
            }
            send_cond.notify_all();
            if (s_thread != nullptr && s_thread->joinable())
            {
                s_thread->join();
                s_thread = nullptr;
            }
            std::cout << "socket_server stop..." << std::endl;
        }

        void pending(std::shared_ptr<ws_message> package)
        {
            {
                std::lock_guard<std::mutex> guard(send_mtx);
                send_queue.push(package);
            }
            send_cond.notify_one();
        }

        void send(const uint8_t *buf, size_t size)
        {
            try
            {
                lock_guard<mutex> guard(m_connection_lock);
                std::cout << "m_connections count:" << m_connections.size() << std::endl;
                for (auto it = m_connections.begin(); it != m_connections.end(); ++it)
                {
                    websocketpp::lib::error_code ec;
                    m_server.send(*it, buf, size, websocketpp::frame::opcode::binary, ec);
                    if (ec)
                    {
                        std::cerr<< "throw excetion:" << ec.message();
                        
                    }
                }
            }
            catch(const websocketpp::exception& e)
            {
                std::cerr<< "websocketpp error:" << e.what();
            }
            catch(...)
            {
                std::cerr<< "unknown errors.";
            } 
        }

    private:
        void run()
        {
            try
            {
                // listen on specified port
                m_server.listen(m_port);
                // Start the server accept loop
                m_server.start_accept();
                // Start the ASIO io_service run loop
                m_server.run();
            }
            catch(const websocketpp::exception& e)
            {
                std::cerr<< "websocketpp error:" << e.what();
            }
            catch(...)
            {
                std::cerr<< "unknown errors.";
            }
            std::cout << "run exit..." << std::endl;
        }

        void send_process()
        {
            std::cout << "send start." << std::endl;
            while (running)
            {
                std::unique_lock<std::mutex> lock(send_mtx);
                while (running && send_queue.empty())
                {
                    send_cond.wait(lock);
                }
                if(!running) continue;
                std::shared_ptr<ws_message> msg = send_queue.front();
                send_queue.pop();
                lock.unlock();
                // SLOG(INFO) << "start send message. queue size:" << send_queue.size();
                send(msg->get_data(), msg->get_data_size());
            }
            std::cout << "send end." << std::endl;
        }

        void on_open(connection_hdl hdl)
        {
            {
                lock_guard<mutex> guard(m_action_lock);
                std::cout << "on_open" << std::endl;
                m_actions.push(action(SUBSCRIBE, hdl));
            }
            m_action_cond.notify_one();
        }

        void on_close(connection_hdl hdl)
        {
            {
                lock_guard<mutex> guard(m_action_lock);
                std::cout << "on_close" << std::endl;
                m_actions.push(action(UNSUBSCRIBE, hdl));
            }
            m_action_cond.notify_one();
        }

        void on_message(connection_hdl hdl, server::message_ptr msg)
        {
            // queue message up for sending by processing thread
            {
                lock_guard<mutex> guard(m_action_lock);
                // std::cout << "on_message:" << msg.get()->get_payload();
                m_actions.push(action(MESSAGE, hdl, msg));
            }
            m_action_cond.notify_one();
        }

        void process_messages()
        {
            std::cout << "process start." << std::endl;
            while (running)
            {
                //std::cout << "process 1..." << std::endl;
                unique_lock<mutex> lock(m_action_lock);
                while (running && m_actions.empty())
                {
                    m_action_cond.wait(lock);
                }
                //std::cout << "process 2..." << std::endl;
                if(!running) continue;
                action a = m_actions.front();
                m_actions.pop();
                lock.unlock();
                //std::cout << "process 3..." << std::endl;
                if (a.type == SUBSCRIBE)
                {
                    lock_guard<mutex> guard(m_connection_lock);
                    m_connections.insert(a.hdl);
                    if (this->open_func)
                    {
                        this->open_func();
                    }
                }
                else if (a.type == UNSUBSCRIBE)
                {
                    lock_guard<mutex> guard(m_connection_lock);
                    m_connections.erase(a.hdl);
                    if (this->close_func)
                    {
                        this->close_func();
                    }
                }
                else if (a.type == MESSAGE)
                {
                    if (this->msg_func)
                    {
                        this->msg_func(a.msg);
                    }
                }
                else
                {
                    // undefined.
                }
                //std::cout << "process 4..." << std::endl;
            }
            std::cout << "process end." << std::endl;
        }

    private:
        server m_server;
        con_list m_connections;
        uint16_t m_port;
        std::atomic<bool> running;

        std::queue<action> m_actions;
        mutex m_action_lock;
        mutex m_connection_lock;
        condition_variable m_action_cond;

        std::function<void(server::message_ptr)> msg_func;
        std::function<void()> open_func;
        std::function<void()> close_func;

        websocketpp::lib::shared_ptr<websocketpp::lib::thread> p_thread;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> socket_thread;

        std::shared_ptr<std::thread> s_thread;
        std::mutex send_mtx;
        std::condition_variable send_cond;
        std::queue<std::shared_ptr<swr::ws_message>> send_queue;
    };
}
