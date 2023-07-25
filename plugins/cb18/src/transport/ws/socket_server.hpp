#pragma once
#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <memory>
#include <set>
#include <websocketpp/common/thread.hpp>
#include "ws_message.hpp"

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

typedef websocketpp::server<websocketpp::config::asio> server;
typedef std::set<connection_hdl,std::owner_less<connection_hdl> > con_list;

namespace swr {

enum action_type {
    SUBSCRIBE,
    UNSUBSCRIBE,
    MESSAGE
};

struct action {
    action(action_type t, connection_hdl h) : type(t), hdl(h) {}
    action(action_type t, connection_hdl h, server::message_ptr m)
      : type(t), hdl(h), msg(m) {}

    action_type type;
    websocketpp::connection_hdl hdl;
    server::message_ptr msg;
};

class socket_server: std::enable_shared_from_this<socket_server>
{
public:
    socket_server(uint16_t port):running(true),m_port(port) {
        // Initialize Asio Transport
        m_server.set_access_channels(websocketpp::log::alevel::none);
        // m_server.clear_access_channels(websocketpp::log::alevel::all);
        // m_server.clear_error_channels(websocketpp::log::alevel::all);
        m_server.init_asio();

        // Register handler callbacks
        m_server.set_reuse_addr(true);
        m_server.set_open_handler(bind(&socket_server::on_open,this,::_1));
        m_server.set_close_handler(bind(&socket_server::on_close,this,::_1));
        m_server.set_message_handler(bind(&socket_server::on_message,this,::_1,::_2));

        // Start a thread to run the processing loop
        p_thread.reset(new websocketpp::lib::thread(websocketpp::lib::bind(&socket_server::process_messages,this)));
        s_thread = std::make_shared<std::thread>(std::bind(&socket_server::send_process,this));
    }

    void set_open(std::function<void ()> func){
        this->open_func = func;
    }

    void set_close(std::function<void ()> func){
        this->close_func = func;
    }

    void set_handler(std::function<void (server::message_ptr)> func){
        this->msg_func = func;
    }

    

    void start()
    {
        socket_thread.reset(new websocketpp::lib::thread(&socket_server::run,this));
    }

    void stop()
    {
        m_server.stop_listening();
        running = false;
        if(p_thread!=nullptr && p_thread->joinable())
        {
            p_thread->join();
            p_thread = nullptr;
        }
        if(s_thread !=nullptr && s_thread->joinable())
        {
            s_thread->join();
            s_thread = nullptr;
        }
    }

    void pending(std::shared_ptr<ws_message> package)
    {
        {
            std::lock_guard<std::mutex> guard(send_mtx);
            send_queue.push(package);
        }
        send_cond.notify_one();
    }

    void send(const uint8_t* buf,size_t size){
        con_list::iterator it;
        for (it = m_connections.begin(); it != m_connections.end(); ++it) {
            websocketpp::lib::error_code ec;
            m_server.send(*it,buf,size,websocketpp::frame::opcode::binary,ec);
            if(ec)
            {
                std::cerr << "throw excetion:" << ec.message() << std::endl;
            }
            else
            {
                std::cout << "send success." << std::endl;
            }
        }
    }

    private:
        void run() {
            // listen on specified port
            m_server.listen(m_port);
            // Start the server accept loop
            m_server.start_accept();
            // Start the ASIO io_service run loop
            try {
                m_server.run();
            } catch (const std::exception & e) {
                std::cout << e.what();
            }
        }

        bool send_process()
        {
            while (running)
            {
                std::unique_lock<std::mutex> lock(send_mtx);
                while(running && send_queue.empty()) {
                    send_cond.wait(lock);
                }
                if(send_queue.empty()) continue;
                std::shared_ptr<ws_message> msg = send_queue.front();
                send_queue.pop();
                lock.unlock();

                //SLOG(INFO) << "start send message. queue size:" << send_queue.size();
                send(msg->get_data(),msg->get_data_size());
            }
        }

    void on_open(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_open" << std::endl;
            m_actions.push(action(SUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
    }

    void on_close(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_close"<< std::endl;
            m_actions.push(action(UNSUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        // queue message up for sending by processing thread
        {
            lock_guard<mutex> guard(m_action_lock);
            //std::cout << "on_message:" << msg.get()->get_payload();
            m_actions.push(action(MESSAGE,hdl,msg));
        }
        m_action_cond.notify_one();
    }

    void process_messages() {
        while(running) {
            unique_lock<mutex> lock(m_action_lock);
            while(running && m_actions.empty()) {
                m_action_cond.wait(lock);
            }

            action a = m_actions.front();
            m_actions.pop();
            lock.unlock();

            if (a.type == SUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.insert(a.hdl);
                if(this->open_func)
                {
                    this->open_func();
                }
            } else if (a.type == UNSUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.erase(a.hdl);
                if(this->close_func)
                {
                    this->close_func();
                }
            } else if (a.type == MESSAGE) {
                lock_guard<mutex> guard(m_connection_lock);
                if(this->msg_func)
                {
                    this->msg_func(a.msg);
                }
            } else {
                // undefined.
            }
        }
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

    std::function<void (server::message_ptr)> msg_func;
    std::function<void ()> open_func;
    std::function<void ()> close_func;
    
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> p_thread;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> socket_thread;

    std::shared_ptr<std::thread> s_thread;
    std::mutex send_mtx;
    std::condition_variable send_cond;
    std::queue<std::shared_ptr<swr::ws_message>> send_queue;
};
}
