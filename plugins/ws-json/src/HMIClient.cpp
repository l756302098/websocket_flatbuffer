#include <iostream>
#include "message_handler.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <functional>
#include "transport/ws/socket_client.hpp"

using namespace swr;

nlohmann::json ToJson()
{
    nlohmann::json j;

    j["ip"] = "127.0.0.1";

    j["port"] = 9366;

    return j;

}

int main(int argc,char** argv){
    std::cout << "start main func." << std::endl;
    bool isConnected = false;

    swr::socket_client clientMs(8765);
    message_handler handlerMs;
    clientMs.set_handler(std::bind(&message_handler::on_response,&handlerMs,std::placeholders::_1));
    clientMs.set_connect_handler([&isConnected](bool connected){
        std::cout << "clientMs connected:" << connected << std::endl;
        isConnected = connected;
    });

    swr::socket_client clientData(8766);
    message_handler handlerData;
    clientData.set_handler(std::bind(&message_handler::on_report,&handlerData,std::placeholders::_1));
    clientData.set_connect_handler([&isConnected](bool connected){
        std::cout << "clientData connected:" << connected << std::endl;
        isConnected = connected;
    });
    
    while (!isConnected)
    {
        clientMs.start();
        clientData.start();

        sleep(3);
    }
    std::cout << "client connected success." << std::endl;
    int count = 100;
    while (--count > 0)
    {
        auto json = ToJson();
        std::string str = json.dump();
        clientMs.send((const uint8_t *)str.data(), str.size());
        sleep(1);
    }

    clientData.stop();
    clientMs.stop();
    
    return 0;
}