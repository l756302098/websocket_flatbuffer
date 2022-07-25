#pragma once
#include <iostream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include "../protocol/generated/s2c_generated.h"
#include "message_helper.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> WsClient;
typedef websocketpp::connection_hdl WsConnection;

namespace abby {

class socket_client
{
public:
    socket_client():running(true) {
        // Initialize Asio Transport
        client.set_access_channels(websocketpp::log::alevel::all);
        client.clear_access_channels(websocketpp::log::alevel::all);
        client.clear_error_channels(websocketpp::log::alevel::all);
        client.init_asio();
        
        client.set_open_handler(websocketpp::lib::bind(&socket_client::on_open, this, websocketpp::lib::placeholders::_1));
        client.set_fail_handler(websocketpp::lib::bind(&socket_client::on_failed, this, websocketpp::lib::placeholders::_1));
        client.set_close_handler(websocketpp::lib::bind(&socket_client::on_close, this, websocketpp::lib::placeholders::_1));
        client.set_message_handler(websocketpp::lib::bind(&socket_client::on_message, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
         // Start a thread to run the processing loop
        p_thread = new websocketpp::lib::thread(websocketpp::lib::bind(&socket_client::process_messages,this));
    }

    void start()
    {
        //connect
        //client.reset();
        websocketpp::lib::error_code ec;
        std::string url = "ws://127.0.0.1:9002";
        auto con = client.get_connection(url, ec);
        if (ec) {
            std::cout << "[easy_client] could not create connection because: " << ec.message() << std::endl;
            return;
        }
       
        client.connect(con);

        client.run();
    }

    void stop()
    {
        //m_server.stop_listening();
        running = false;
        if(p_thread)
        {
            p_thread->join();
            p_thread = nullptr;
        }
    }

    void send(const char* buf,size_t size){
        
    }

    void deal_msg(std::string &payload)
    {
        std::cout << "deal_msg..." << std::endl;
        try
        {
            // std::uint32_t bodySize = 0;
            // memcpy(&bodySize,payload.c_str(),4);
            // std::vector<char> body(payload.begin()+4, payload.end());
            // std::cout << "payload size:" << payload.size() << " size:" << bodySize << std::endl;
            // for (size_t i = 0; i < body.size(); i++)
            // {
            //     printf("%02X ",body[i]);
            // }
            // printf("\n");
            
            // // verifier data
            // flatbuffers::Verifier verifier((const uint8_t *)(payload.c_str()+4), bodySize);
            // if (!VerifyServerDataBuffer(verifier))
            // {
            //     std::cout << "error data" << std::endl;
            //     return;
            // }
            // parsing
            //auto data = GetServerData((const uint8_t*)&body[0]);
            auto data = abby::unpackage((const uint8_t*)payload.c_str(),payload.size());
            if(data==nullptr) {
                std::cout << "data is null." << std::endl;
                return;
            }
            switch (data->message_type()) // union自带type
            {
                case ServerType_ServerA:
                {
                    auto quote = reinterpret_cast<const abby::ServerA *>(data->message());
                    std::cout << "name: " << quote->name()->c_str() << ", price: " << quote->price();
                    break;
                }
                case ServerType_ServerB:
                {
                    auto quote = reinterpret_cast<const abby::ServerB *>(data->message());
                    std::cout << "name: " << quote->name()->c_str() << ", age: " << quote->age();
                    break;
                }
                case ServerType_ServerC:
                {
                    auto quote = reinterpret_cast<const abby::ServerC *>(data->message());
                    std::cout << "name: " << quote->name()->c_str() << ", weight: " << quote->weight();
                    break;
                }
                default:
                {
                    std::cout << "min:" <<   abby::ServerType::ServerType_MIN << std::endl;
                    std::cout << "max:" <<   abby::ServerType::ServerType_MAX << std::endl;
                    std::cout << "undefined type." << data->message_type() << std::endl;
                    break;
                }
            }

        }
        catch(const std::exception& e)
        {
            std::cout << e.what()<< std::endl;
        }
        catch(...)
        {
            std::cout << "unknown error." << std::endl;
        } 
    }

    void on_open(WsConnection hdl) {
        std::cout << "on_open" << std::endl;
    }

    void on_close(WsConnection hdl) {
        std::cout << "on_close" << std::endl;
    }

    void on_message(WsConnection hdl, WsClient::message_ptr msg) {
        // queue message up for sending by processing thread
        // std::cout << "on_message:" << msg.get()->get_payload().size() << std::endl;
        // for (size_t i = 0; i < msg.get()->get_payload().size(); i++)
        // {
        //     printf("%02X ",msg.get()->get_payload()[i]);
        // }
        // printf("\n");

        std::unique_lock<std::mutex> lock(action_lock);
        messages.push_back(msg->get_payload());
        action_condition.notify_one();
    }

    void on_failed(WsConnection hdl){
        std::cout <<" on_failed  " << std::endl;
    }

    void process_messages() {
        while(running) {
            std::unique_lock<std::mutex> lock(action_lock);
            while (messages.empty() && running)
                action_condition.wait(lock);

            std::vector<std::string> localMsgs;
            localMsgs.swap(messages);
            lock.unlock();

            for (std::vector<std::string>::iterator itr = localMsgs.begin();
                itr != localMsgs.end(); ++itr)
            {
                deal_msg(*itr);
            }
        }
    }
private:
    WsClient client;    
    websocketpp::lib::thread *p_thread;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> socket_thread;
    uint16_t port;
    std::function<void (WsClient::message_ptr)> deal;
    std::atomic<bool> running;
    std::mutex action_lock;  
    std::condition_variable action_condition;
    std::vector<std::string> messages;
};
}
