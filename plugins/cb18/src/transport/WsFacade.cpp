/*
 * WsFacade.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: ubuntu
 */

#include "WsFacade.h"
#include <thread>

namespace swr {
namespace transport {

WsFacade::WsFacade(std::string configPath) {
    sessionCounter = 0;
    mMsSocket = std::make_shared<swr::socket_server>(8765);
    mDataSocket = std::make_shared<swr::socket_server>(8766);
}

bool WsFacade::Accept(std::function<void(bool connected)> f) {
    OnConnectionTriggered = f;
    mMsSocket->set_open([this]() {
        {
            std::unique_lock<std::mutex> lck(mtxSenssionCounter);
            if (++sessionCounter == 2) {
                OnConnectionTriggered(true);
            }
        }
    });
    mMsSocket->set_close([this]() {
        std::unique_lock<std::mutex> lck(mtxSenssionCounter);
        if (--sessionCounter < 2) {
            OnConnectionTriggered(false);
        }
    });
    mMsSocket->set_handler([this](server::message_ptr ptr){
        const std::string& payload = ptr->get_payload();
        auto request = swr::unpackage_request((const uint8_t*)payload.c_str(),payload.size());
        if(request==nullptr)
        {
            std::cerr << "request is null." << std::endl;
        }
        else
        {
            std::cout << "RequestType: " << EnumNameRequestType(request->type()) << std::endl;
            requestHandle(0, *request);
        } 
    });
    mDataSocket->set_open([this]() {
        {
            std::unique_lock<std::mutex> lck(mtxSenssionCounter);
            if (++sessionCounter == 2) {
                OnConnectionTriggered(true);
            }
        }
    });
    mDataSocket->set_close([this]() {
        std::unique_lock<std::mutex> lck(mtxSenssionCounter);
        if (--sessionCounter < 2) {
            OnConnectionTriggered(false);
        }
    });
    return true;
}

bool WsFacade::IsConntected() {
    std::unique_lock<std::mutex> lck(mtxSenssionCounter);
    return sessionCounter == 2;
}

bool WsFacade::Start() {
    mMsSocket->start();
    mDataSocket->start();
    return true;
}

bool WsFacade::Stop() {
    mMsSocket->stop();
    mDataSocket->stop();
    return true;
}

bool WsFacade::Pause() {
    return true;
}
bool WsFacade::Resume() {
    return true;
}

bool WsFacade::SendReport(ReportBuffer& report) {
    std::lock_guard<std::mutex> lck(mtxSsnData);
    auto package = swr::package(report);
    mDataSocket->pending(package);
    return true;
}

bool WsFacade::SendResponse(MessageId mid, ResponseBuffer& response) {
    std::lock_guard<std::mutex> lck(mtxSsnIns);
    auto package = swr::package(response);
    mMsSocket->send(package->get_data(),package->get_data_size());
    auto resp = swr::unpackage_response(package->get_data(),package->get_data_size());
    std::cout << "SendResponse RequestType: " << EnumNameRequestType(resp->type())
     << std::endl;
    return true;
}

bool WsFacade::UpdateRequestHandle(
    std::function<bool(std::uint16_t, const swr::Request&)> f) {
    requestHandle = f;
    return true;
}

}  // namespace transport
}  // namespace swr
