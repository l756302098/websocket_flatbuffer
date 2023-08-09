#include <iostream>
#include "message_handler.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "transport/WsFacade.h"
#include <functional>
#include "transport/json.hpp"
#include "transport/base64.hpp"

using namespace swr;

nlohmann::json ToJson()
{
    nlohmann::json j;

    // add a number that is stored as double (note the implicit conversion of j to an object)
    j["pi"] = 3.141;

    // add a Boolean that is stored as bool
    j["happy"] = true;

    // add a string that is stored as std::string
    j["name"] = "Niels";

    // add another null object by passing nullptr
    j["nothing"] = nullptr;

    // add an object inside the object
    j["answer"]["everything"] = 42;

    // add an array that is stored as std::vector (using an initializer list)
    j["list"] = { 1, 0, 2 };

    // add another object (using an initializer list of pairs)
    j["object"] = { {"currency", "USD"}, {"value", 42.99} };

    return j;

}

int main(int argc,char** argv){
    std::cout << "start main func." << std::endl;
    swr::transport::WsFacade wf("test");
    std::function<bool(nlohmann::json&)> f = 
        [](nlohmann::json& req){
            std::cout << "receive msg." << req.type_name() << " json:" << req.dump() << std::endl;
            return true;
    };
    wf.UpdateRequestHandle(f);
    wf.Accept([](bool connected){
        std::cout << "connected:" << connected <<  std::endl;
    });
    std::vector<std::uint8_t> imagebuf;
    cv::Mat image = cv::imread("/home/li/test.png", cv::IMREAD_UNCHANGED);
    if (!cv::imencode(".png", image, imagebuf)) {
        std::cout << "fail to imencode(.png). w:" << image.cols
                    << ", h:" << image.rows << std::endl;
        return 1;
    }
    wf.Start();
    sleep(10);
    // self-test
    int count = 0;
    while (++count<11)
    {
        std::string encode_str = base64_encode(imagebuf.data(),imagebuf.size());
        std::cout << "image size:" << imagebuf.size() 
            << " encode size:" << encode_str.size() << std::endl;
        nlohmann::json ijson;
        ijson["data"] = encode_str;
        ijson["encoding"] = "base64";
        ijson["width"] = image.cols;
        ijson["height"] = image.rows;
        wf.SendReport(ijson);
        
        // std::vector<std::uint8_t> decodeBuf;
        // std::string decode_str = base64_decode(encode_str);
        // decodeBuf.assign(decode_str.begin(),decode_str.end());
        // std::cout << "decode bug size:" << decodeBuf.size() << std::endl;

        usleep(1e5);
    }

    //report fault code
    uint32_t code = 1;
    while (1)
    {
        std::cout << "SendReport response." << std::endl;
        auto json = ToJson();
        std::cout << "json type:" << json.type_name() << std::endl;
        wf.SendResponse(json);
        
        sleep(1);
        code++;
    }   

    std::cout << "exit..." << std::endl;
    wf.Stop();
    
    return 0;
}