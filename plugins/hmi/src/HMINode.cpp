#include <iostream>
#include "socket_server.hpp"
#include "message_handler.hpp"
#include "socket_client.hpp"
#include "message_helper.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "../protocol/generated/s2c_generated.h"

using namespace abby;

int main(int argc,char** argv){
    std::cout << "start main func." << std::endl;
    try {
        message_handler handler;
        socket_server server_instance;

        server_instance.set_handler(std::bind(&message_handler::on_message,&handler,std::placeholders::_1));
        server_instance.start(9002);

        sleep(1);

        std::thread([&]{
             while (1)
            {
                std::cout << "sleep 1" << std::endl;
                flatbuffers::FlatBufferBuilder builder;
                std::cout << "--------------test 1--------------------" << std::endl;
                auto entry = CreateServerA(builder,2.30,builder.CreateString("a"));
                auto serverData = CreateServerData(builder,ServerType_ServerA,entry.Union());
                builder.Finish(serverData);
                
                auto package = abby::package(builder);
                server_instance.send(package->get_body(),package->get_size());
                
                sleep(0.01);

                std::cout << "--------------test image--------------------" << std::endl;
                cv::Mat image = cv::imread("/home/li/test.png", cv::IMREAD_UNCHANGED);
                std::vector<std::uint8_t> imagebuf;
                if (!cv::imencode(".png", image, imagebuf)) {
                    std::cout << "fail to imencode(.png). w:" << image.cols
                                << ", h:" << image.rows << std::endl;
                    return;
                }
                flatbuffers::FlatBufferBuilder builder2;
                auto imageEntry = CreateServerImage(builder2,image.rows, image.cols,builder2.CreateVector(imagebuf));
                auto serverImageData = CreateServerData(builder2,ServerType_ServerImage,imageEntry.Union());
                builder2.Finish(serverImageData);

                auto imagePackage = abby::package(builder2);
                std::cout << "origin size:" << imagebuf.size() << " decode size:" << imagePackage->get_size() << std::endl;
                server_instance.send(imagePackage->get_body(),imagePackage->get_size());

                sleep(0.1);
            }
        }).detach();

        socket_client client;
        client.start();

        server_instance.stop();

    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}