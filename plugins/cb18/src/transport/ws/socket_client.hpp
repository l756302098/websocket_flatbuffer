#pragma once
#include <iostream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include "message_helper.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> ws_client;
typedef websocketpp::connection_hdl ws_connection;

namespace swr
{

    class socket_client
    {
    public:
        socket_client(uint16_t port) : m_port(port),running(true)
        {
            // Initialize Asio Transport
            client.set_access_channels(websocketpp::log::alevel::none);
            // client.clear_access_channels(websocketpp::log::alevel::all);
            // client.clear_error_channels(websocketpp::log::alevel::all);
            client.init_asio();

            client.set_open_handler(websocketpp::lib::bind(&socket_client::on_open, this, websocketpp::lib::placeholders::_1));
            client.set_fail_handler(websocketpp::lib::bind(&socket_client::on_failed, this, websocketpp::lib::placeholders::_1));
            client.set_close_handler(websocketpp::lib::bind(&socket_client::on_close, this, websocketpp::lib::placeholders::_1));
            client.set_message_handler(websocketpp::lib::bind(&socket_client::on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
            p_thread.reset(new websocketpp::lib::thread(websocketpp::lib::bind(&socket_client::process_messages, this)));
            p_thread->detach();
        }

        ~socket_client()
        {
            std::cout << "start ~socket_client" << std::endl;
            this->msg_func = nullptr;
            this->connect_func = nullptr;
            std::cout << "end ~socket_client" << std::endl;
        }

        void start()
        {
            // Start a thread to run the processing loop
            socket_thread.reset(new websocketpp::lib::thread(&socket_client::run, this));
            socket_thread->detach();
        }

        void run()
        {
            std::cout << "start run..." << std::endl;
            // connect
            websocketpp::lib::error_code ec;
            std::string url = "ws://127.0.0.1:" + std::to_string(m_port);
            auto con = client.get_connection(url, ec);
            if (ec)
            {
                std::cout << "could not create connection because: " << ec.message() << std::endl;
                return;
            }

            client.connect(con);
            client.run();
            client.reset();
            std::cout << "end run..." << std::endl;
        }

        void stop()
        {
            running = false;
            if(!client.stopped())
            {
                client.stop();
            }
            action_condition.notify_all();
            if (p_thread != nullptr && p_thread->joinable())
            {
                p_thread->join();
                p_thread = nullptr;
            }
        }

        void send(const uint8_t *buf, size_t size)
        {
            if(!connected) {
                std::cerr<< "connected is false.";
                return;
            }
            try
            {
                websocketpp::lib::error_code ec;
                client.send(connect, buf, size, websocketpp::frame::opcode::binary, ec);
                if (ec)
                {
                    std::cerr << "throw excetion:" << ec.message() << std::endl;
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

        void set_handler(std::function<void(ws_client::message_ptr)> func)
        {
            this->msg_func = func;
        }

        void set_connect_handler(std::function<void(bool)> func)
        {
            this->connect_func = func;
        }

        void on_open(ws_connection hdl)
        {
            std::cout << "on_open" << std::endl;
            connect = hdl;
            connected = true;
            if (this->connect_func)
            {
                this->connect_func(connected);
            }
        }

        void on_close(ws_connection hdl)
        {
            std::cout << "on_close" << std::endl;
            connected = false;
            if (this->connect_func)
            {
                this->connect_func(connected);
            }
        }

        void on_message(ws_connection hdl, ws_client::message_ptr msg)
        {
            std::unique_lock<std::mutex> lock(action_lock);
            messages.push_back(msg);
            action_condition.notify_one();
        }

        void on_failed(ws_connection hdl)
        {
            std::cout << " on_failed  " << std::endl;
            connected = false;
            if (this->connect_func)
            {
                this->connect_func(connected);
            }
        }

        void process_messages()
        {
            while (running)
            {
                std::unique_lock<std::mutex> lock(action_lock);
                while (messages.empty() && running)
                    action_condition.wait(lock);
                if(!running) continue;
                std::vector<ws_client::message_ptr> localMsgs;
                localMsgs.swap(messages);
                lock.unlock();

                for (auto itr = localMsgs.begin(); itr != localMsgs.end(); ++itr)
                {
                    if (msg_func != nullptr)
                    {
                        msg_func(*itr);
                    }
                }
            }
        }

        bool connect_status(){
            return connected;
        }

    private:
        ws_client client;
        uint16_t m_port;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> p_thread;
        websocketpp::lib::shared_ptr<websocketpp::lib::thread> socket_thread;
        ws_connection connect;
        bool connected;

        std::vector<ws_client::message_ptr> messages;
        std::function<void(ws_client::message_ptr)> msg_func;
        std::function<void(bool)> connect_func;
        std::atomic<bool> running;
        std::mutex action_lock;
        std::condition_variable action_condition;
    };
}
