#pragma once
#include <iostream>
#include "../protocol/generated/c2s_generated.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

namespace abby {
class message_handler
{
private:
    /* data */
public:
    message_handler(/* args */){

    }
    ~message_handler(){

    }

    void on_message(const server::message_ptr &msg)
    {
        try
        {
            //std::cout << "header:" << msg.get()->get_header();
            //std::cout << "payload:" << msg.get()->get_payload();
            std::string payload = msg.get()->get_payload();
            std::uint32_t bodySize = 0;
            memcpy(&bodySize,payload.c_str(),4);
            std::vector<char> body(payload.begin()+4, payload.end());
            std::cout << "payload size:" << payload.size() << " size:" << bodySize << " body:" << body.size() << std::endl;
            // verifier data
            // flatbuffers::Verifier verifier((const uint8_t *)(&body[0]), bodySize);
            // if (!VerifyCDataTypeEntryBuffer(verifier))
            // {
            //     std::cout << "error data";
            //     return;
            // }
            // // parsing
            // auto data = GetCDataTypeEntry(&body[0]);
            // switch (data->entry_type()) // union自带type
            // {
            //     case CDataType_C_A:
            //     {
            //         auto quote = reinterpret_cast<const abby::C_A *>(data->entry());
            //         std::cout << "name: " << quote->name()->c_str() << ", price: " << quote->price();
            //         break;
            //     }
            //     case CDataType_C_B:
            //     {
            //         auto quote = reinterpret_cast<const abby::C_B *>(data->entry());
            //         std::cout << "name: " << quote->name()->c_str() << ", age: " << quote->age() << std::endl;
            //         break;
            //     }
            //     case CDataType_C_C:
            //     {
            //         auto quote = reinterpret_cast<const abby::C_C *>(data->entry());
            //         std::cout << "name: " << quote->name()->c_str() << ", weight: " << quote->weight()<< std::endl;
            //         break;
            //     }
            //     default:
            //     {
            //         std::cout << "undefined type.";
            //         break;
            //     }
            // }

        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
        catch(...)
        {
            std::cout << "unknown error." << std::endl;
        }
    
    }
};
}
