#pragma once
#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <set>
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

namespace abby {

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

class socket_server
{
public:
    socket_server():running(true) {
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
        p_thread = new websocketpp::lib::thread(websocketpp::lib::bind(&socket_server::process_messages,this));
    }

    void set_port(uint16_t port)
    {
        this->port = port;
    }

    void set_handler(std::function<void (server::message_ptr)> func){
        this->deal = func;
    }

    void run() {
        // listen on specified port
        m_server.listen(port);

        // Start the server accept loop
        m_server.start_accept();

        // Start the ASIO io_service run loop
        try {
            m_server.run();
        } catch (const std::exception & e) {
            std::cout << e.what();
        }
    }

    void start(uint16_t port)
    {
        this->port = port;
        socketThread.reset(new websocketpp::lib::thread(&socket_server::run,this));
    }

    void stop()
    {
        m_server.stop_listening();
        running = false;
        if(p_thread)
        {
            p_thread->join();
            p_thread = nullptr;
        }
    }

    void send(const uint8_t* buf,size_t size){
        con_list::iterator it;
        for (it = m_connections.begin(); it != m_connections.end(); ++it) {
            websocketpp::lib::error_code ec;
            m_server.send(*it,buf,size,websocketpp::frame::opcode::binary,ec);
            if(ec)
            {
                std::cout << "throw excetion:" << ec.message() << std::endl;
            }
            else
            {
                std::cout << "send success." << std::endl;
            }

        }
    }

    void on_open(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_open";
            m_actions.push(action(SUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
    }

    void on_close(connection_hdl hdl) {
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_close";
            m_actions.push(action(UNSUBSCRIBE,hdl));
        }
        m_action_cond.notify_one();
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        // queue message up for sending by processing thread
        {
            lock_guard<mutex> guard(m_action_lock);
            std::cout << "on_message:" << msg.get()->get_payload();
            m_actions.push(action(MESSAGE,hdl,msg));
        }
        m_action_cond.notify_one();
    }

    void process_messages() {
        while(running) {
            unique_lock<mutex> lock(m_action_lock);

            while(m_actions.empty()) {
                m_action_cond.wait(lock);
            }

            action a = m_actions.front();
            m_actions.pop();

            lock.unlock();

            if (a.type == SUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.insert(a.hdl);
            } else if (a.type == UNSUBSCRIBE) {
                lock_guard<mutex> guard(m_connection_lock);
                m_connections.erase(a.hdl);
            } else if (a.type == MESSAGE) {
                lock_guard<mutex> guard(m_connection_lock);
                if(this->deal)
                {
                    this->deal(a.msg);
                }
            } else {
                // undefined.
            }
        }
    }
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl> > con_list;

    server m_server;
    con_list m_connections;
    std::queue<action> m_actions;

    mutex m_action_lock;
    mutex m_connection_lock;
    condition_variable m_action_cond;
    websocketpp::lib::thread *p_thread;
    uint16_t port;
    std::function<void (server::message_ptr)> deal;
    std::atomic<bool> running;

    websocketpp::lib::shared_ptr<websocketpp::lib::thread> socketThread;
};
}
