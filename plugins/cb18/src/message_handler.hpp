#pragma once
#include <iostream>
#include "../protocol/generated/public_generated.h"
#include "../protocol/generated/report_generated.h"
#include "../protocol/generated/request_generated.h"
#include "../protocol/generated/response_generated.h"
#include "transport/ws/socket_client.hpp"
#include "transport/ws/message_helper.hpp"
#include "transport/ws/ws_message.hpp"

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
        const std::string& payload = msg.get()->get_payload();
        auto response = swr::unpackage_response((const uint8_t*)payload.c_str(),payload.size());
        if(response==nullptr) {
            std::cout << "data is null." << std::endl;
            return;
        }
        switch (response->type())
        {
            case swr::RequestType::RequestType_Login:
                {
                    const swr::ResponseLogin* login = response->message_as_ResponseLogin();
                    std::cout << "receive login:" << login->success() << std::endl;
                    break;
                }
            default:
                {
                    std::cerr << "not found handle." << std::endl;
                    break;
                }
        }
    }

    void on_report(const ws_client::message_ptr &msg)
    {
        std::string payload = msg.get()->get_payload();
        auto report = swr::unpackage_report((const uint8_t*)payload.c_str(),payload.size());
        if(report==nullptr) {
            std::cout << "data is null." << std::endl;
            return;
        }
        switch (report->type())
        {
            case swr::ReportType::ReportType_ReportImage:
                {
                    const swr::ReportImage* image = report->data_as_ReportImage();
                    std::cout << "receive image:" << image->time() << std::endl;
                    break;
                }
            case swr::ReportType::ReportType_ReportPose:
                {
                    const swr::ReportPose* pose = report->data_as_ReportPose();
                    std::cout << "receive pose:" << pose->angle() << std::endl;
                    break;
                }
            case swr::ReportType::ReportType_ReportFaultCode:
                {
                    const swr::ReportFaultCode* code = report->data_as_ReportFaultCode();
                    std::cout << "receive fault code:" << code->fault()->code() << std::endl;
                    break;
                }
            case swr::ReportType::ReportType_ReportSelfTestProcess:
                {
                    const swr::ReportSelfTestProcess* test = report->data_as_ReportSelfTestProcess();
                    std::cout << "receive self test:" << test->over() << " all:" << test->all() << std::endl;
                    break;
                }
            case swr::ReportType::ReportType_ReportSelfTest:
                {
                    const swr::ReportSelfTest* test = report->data_as_ReportSelfTest();
                    std::cout << "receive self error size:" << test->error_list()->size() << std::endl;
                    break;
                }
            default:
                {
                    std::cerr << "not found handle." << std::endl;
                    break;
                }
        }
    }


};
}
