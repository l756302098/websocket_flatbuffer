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
                // uint8_t *buf = builder.GetBufferPointer();
                // std::uint32_t bodySize = builder.GetSize();
                // std::cout << "buf size:" << bodySize << std::endl;
                auto package = abby::package(builder);
                server_instance.send(package->get_body(),package->get_size());
                
               
                /*
                uint8_t *data = new uint8_t[bodySize];
                memcpy(data,buf,bodySize);
                //auto desData = GetServerData((const uint8_t*)data);
                auto desData = abby::unpackage((const uint8_t*)data,bodySize);
                std::cout << "type:" << desData->message_type() << std::endl;
                switch (desData->message_type()) // union自带type
                {
                    case ServerType_ServerA:
                    {
                        auto quote = reinterpret_cast<const abby::ServerA *>(desData->message());
                        std::cout << "name: " << quote->name()->c_str() << ", price: " << quote->price() << std::endl;
                        break;
                    }
                    case ServerType_ServerB:
                    {
                        auto quote = reinterpret_cast<const abby::ServerB *>(desData->message());
                        std::cout << "name: " << quote->name()->c_str() << ", age: " << quote->age();
                        break;
                    }
                    case ServerType_ServerC:
                    {
                        auto quote = reinterpret_cast<const abby::ServerC *>(desData->message());
                        std::cout << "name: " << quote->name()->c_str() << ", weight: " << quote->weight();
                        break;
                    }
                    default:
                    {
                        std::cout << "min:" <<   abby::ServerType::ServerType_MIN << std::endl;
                        std::cout << "max:" <<   abby::ServerType::ServerType_MAX << std::endl;
                        std::cout << "undefined type." << desData->message_type() << std::endl;
                        break;
                    }
                }
                */
                
                // uint8_t *data = new uint8_t[bodySize+4];
                // memcpy(data,&bodySize,4);
                // memcpy(data+4,buf,bodySize);
                // // for (size_t i = 0; i < bodySize+4; i++)
                // // {
                // //     printf("%02X ",data[i]);
                // // }
                // // printf("\n");
                // server_instance.send(data,bodySize+4);

                // delete data;
                
                
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
                server_instance.send(imagePackage->get_body(),imagePackage->get_size());

                sleep(0.01);
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