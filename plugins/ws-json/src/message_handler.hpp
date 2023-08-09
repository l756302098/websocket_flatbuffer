#pragma once
#include <iostream>
#include "transport/ws/socket_client.hpp"
#include "transport/ws/message_helper.hpp"
#include "transport/json.hpp"
#include "transport/base64.hpp"

namespace swr {
class message_handler
{
private:
    /* data */
public:
    message_handler(/* args */){

    }
    ~message_handler(){

    }

    void on_response(const ws_client::message_ptr &msg)
    {
        nlohmann::json j;
        if(swr::unpackage(msg.get()->get_payload(),j))
        {
            std::cout << "unpackage success. type:" << j.type_name() << " " << j.dump() << std::endl;
            
            if(j.contains("pi"))
            {
                double pi = j.at("pi").get<double>();
                std::cout << "pi:" << pi << std::endl;
            }
            bool happy = j.at("happy").get<bool>();
            std::cout << "happy:" << happy << std::endl;
            std::string name = j.at("name").get<std::string>();
            std::cout << "name:" << name << std::endl;
        }
    }

    void on_report(const ws_client::message_ptr &msg)
    {
        nlohmann::json j;
        if(swr::unpackage(msg.get()->get_payload(),j))
        {
            std::cout << "unpackage success. type:" << j.type_name() << std::endl;
            
            if(j.contains("encoding"))
            {
                auto encoding = j.at("encoding").get<std::string>();
                std::cout << "encoding:" << encoding << std::endl;
            }
            if(j.contains("data"))
            {
                std::vector<std::uint8_t> decodeBuf;
                std::string decode_str = base64_decode(j.at("data").get<std::string>());
                decodeBuf.assign(decode_str.begin(),decode_str.end());
            }
            
        }
    }


};
}
