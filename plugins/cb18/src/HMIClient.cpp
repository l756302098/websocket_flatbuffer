#include <iostream>
#include "message_handler.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <functional>
#include "transport/ws/socket_client.hpp"

using namespace swr;

int main(int argc,char** argv){
    std::cout << "start main func." << std::endl;
    bool isConnected = false;

    socket_client clientMs(8765);
    message_handler handlerMs;
    clientMs.set_handler(std::bind(&message_handler::on_response,&handlerMs,std::placeholders::_1));
    clientMs.set_connect_handler([&isConnected](bool connected){
        std::cout << "clientMs connected:" << connected << std::endl;
        isConnected = connected;
    });

    socket_client clientData(8766);
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
        // flatbuffers::FlatBufferBuilder reqfb;
        // auto data = CreateRequestLogin(reqfb,swr::LoginType_HmiUser,reqfb.CreateString("657870"));
        // auto request = CreateRequest(reqfb, swr::RequestType::RequestType_Login,
        //             RequestDataField::RequestDataField_RequestLogin, data.Union());
        // reqfb.Finish(request);

        // std::shared_ptr<ws_message> msg = swr::package(reqfb);
        // clientMs.send(msg->get_data(),msg->get_data_size());


        flatbuffers::FlatBufferBuilder reqfb;
        auto data = CreateRequestSelfTest(reqfb);
        auto request = CreateRequest(reqfb, swr::RequestType::RequestType_SelfTest,
                    RequestDataField::RequestDataField_RequestSelfTest, data.Union());
        reqfb.Finish(request);

        std::shared_ptr<ws_message> msg = swr::package(reqfb);
        clientMs.send(msg->get_data(),msg->get_data_size());

        flatbuffers::FlatBufferBuilder reqfb1;
        auto data1 = CreateRequestLockState(reqfb1,0);
        auto request1 = CreateRequest(reqfb1, swr::RequestType::RequestType_LockState,
                    RequestDataField::RequestDataField_RequestLockState, data1.Union());
        reqfb1.Finish(request1);

        std::shared_ptr<ws_message> msg1 = swr::package(reqfb1);
        clientMs.send(msg1->get_data(),msg1->get_data_size());

        sleep(1);
    }

    clientData.stop();
    clientMs.stop();
    
    return 0;
}