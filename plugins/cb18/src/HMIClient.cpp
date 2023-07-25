#include <iostream>
#include "message_handler.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <functional>
#include "transport/ws/socket_client.hpp"

using namespace swr;

int main(int argc,char** argv){
    std::cout << "start main func." << std::endl;
    socket_client clientMs(8765);
    message_handler handlerMs;
    clientMs.set_handler(std::bind(&message_handler::on_response,&handlerMs,std::placeholders::_1));
    clientMs.start();
    socket_client clientData(8766);
    message_handler handlerData;
    clientData.set_handler(std::bind(&message_handler::on_report,&handlerData,std::placeholders::_1));
    clientData.start();
    while (1)
    {
        flatbuffers::FlatBufferBuilder reqfb;
        auto data = CreateRequestLogin(reqfb,swr::LoginType_HmiUser,reqfb.CreateString("657870"));
        auto request = CreateRequest(reqfb, swr::RequestType::RequestType_Login,
                    RequestDataField::RequestDataField_RequestLogin, data.Union());
        reqfb.Finish(request);

        std::shared_ptr<ws_message> msg = swr::package(reqfb);
        clientMs.send(msg->get_data(),msg->get_data_size());

        sleep(1);
    }

    clientData.stop();
    clientMs.stop();
    
    return 0;
}