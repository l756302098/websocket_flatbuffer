#include <iostream>
#include "message_handler.hpp"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "transport/WsFacade.h"
#include <functional>

using namespace swr;

int main(int argc,char** argv){
    std::cout << "start main func." << std::endl;
    swr::transport::WsFacade wf("test");
    std::function<bool(std::uint16_t, const swr::Request&)> f = 
        [](std::uint16_t id,const swr::Request& req){
            std::cout << "msg id:" << id << std::endl;
            std::cout << "receive type:" << EnumNameRequestType(req.type()) << std::endl;
            switch (req.type())
            {
            case swr::RequestType::RequestType_Login:
                {
                    auto msg = req.message_as_RequestLogin();
                    std::cout << "login success pwd:" << msg->pwd()->c_str() << std::endl;
                    break;
                }
            case swr::RequestType::RequestType_SelfTest:
                {
                    auto msg = req.message_as_RequestSelfTest();
                    std::cout << "request self test." << std::endl;
                    break;
                }
            default:
                break;
            }
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
        //flatbuffers::FlatBufferBuilder rptbf;
        //report pose
        // int hz = 50;
        // while (--hz>0)
        // {
        //     auto data = CreateReportPose(rptbf, 0, 0, 1, 90);
        //     auto report =
        //     CreateReport(rptbf, ReportType::ReportType_ReportPose,
        //                 ReportDataField::ReportDataField_ReportPose, data.Union());
        //     rptbf.Finish(report);
        //     wf.SendReport(rptbf);
        // }
        // report image
        // hz = 15;
        // while (--hz>0)
        // {
        //     auto data = CreateReportImage(rptbf, 3.0, rptbf.CreateVector(imagebuf));
        //     auto report = CreateReport(rptbf, ReportType::ReportType_ReportImage,
        //                             ReportDataField::ReportDataField_ReportImage,
        //                             data.Union());
        //     rptbf.Finish(report);
        //     wf.SendReport(rptbf);
        // }

        //response
        flatbuffers::FlatBufferBuilder rpbf;
        auto message = CreateResponseLogin(rpbf,true);
        auto response = CreateResponse(rpbf,0,swr::RequestType_Login,swr::ResponseDataField_ResponseLogin,message.Union());
        rpbf.Finish(response);
        wf.SendResponse(1,rpbf);

        flatbuffers::FlatBufferBuilder builder;
        auto data = CreateReportSelfTestProcess(builder, count, 10);
        auto report = CreateReport(
            builder, ReportType::ReportType_ReportSelfTestProcess,
            ReportDataField::ReportDataField_ReportSelfTestProcess, data.Union());
        builder.Finish(report);
        wf.SendReport(builder);

        sleep(1);
    }

    {
        transport::ReportBuffer builder;
        std::vector<flatbuffers::Offset<swr::ErrorCode>> errorList;
        auto data =
            CreateReportSelfTest(builder, builder.CreateVector(errorList));
        auto report = CreateReport(
            builder, ReportType::ReportType_ReportSelfTest,
            ReportDataField::ReportDataField_ReportSelfTest, data.Union());
        builder.Finish(report);
        wf.SendReport(builder);
        std::cout << "SendReport ReportSelfTest." << std::endl;
    }

    //report fault code
    uint32_t code = 1;
    while (1)
    {
        std::cout << "SendReport FaultCode." << std::endl;
        swr::FaultCode fault(static_cast<uint32_t>(code),swr::FaultState::FaultState_On);

        flatbuffers::FlatBufferBuilder builder;
        auto data = CreateReportFaultCode(builder, &fault);
        auto report = CreateReport(
            builder, ReportType::ReportType_ReportFaultCode,
            ReportDataField::ReportDataField_ReportFaultCode, data.Union());
        builder.Finish(report);
        wf.SendReport(builder);
        
        sleep(1);
        code++;
    }   

    std::cout << "exit..." << std::endl;
    imagebuf.clear();
    wf.Stop();
    
    return 0;
}