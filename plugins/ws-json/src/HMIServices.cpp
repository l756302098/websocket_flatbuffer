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
    
    cv::Mat mat = cv::imread("/home/li/test.png", cv::IMREAD_UNCHANGED);

    int params[3] = {0};
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = 100;

	std::vector<uchar> imageBuf;
	bool code = cv::imencode(".jpg", mat,imageBuf, std::vector<int>(params, params+2));

    wf.Start();
    sleep(10);
    // self-test
    int count = 0;
    while (++count<52)
    {
        std::string encode_str = base64_encode(&imageBuf[0],imageBuf.size());
        //std::cout  << " encode size:" << encode_str.size() << std::endl;
        std::cout  << "encode str:" << encode_str << std::endl;
        nlohmann::json ijson;
        ijson["data"] = encode_str;
        ijson["encoding"] = "base64";
        ijson["width"] = mat.cols;
        ijson["height"] = mat.rows;
        wf.SendReport(ijson);
        
        // std::string decoded_string = base64_decode(encode_str);
        // std::vector<uchar> data(decoded_string.begin(), decoded_string.end());
        // cv::Mat decodeImg = cv::imdecode(data, cv::IMREAD_UNCHANGED);
        // cv::imwrite("/home/li/d.jpg", decodeImg);

        usleep(1e5);
    }

    //report fault code
    while (1)
    {
        std::cout << "SendReport response." << std::endl;
        auto json = ToJson();
        std::cout << "json type:" << json.type_name() << std::endl;
        wf.SendResponse(json);
        
        sleep(1);
    }   

    std::cout << "exit..." << std::endl;
    wf.Stop();
    
    return 0;
}