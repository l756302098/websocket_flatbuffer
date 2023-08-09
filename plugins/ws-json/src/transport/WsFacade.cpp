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
        nlohmann::json json;
        auto success = swr::unpackage(ptr->get_payload(),json);
        if(!success)
        {
            std::cerr << "request is null." << std::endl;
        }
        else
        {
            std::cout << "receive json:" << json.dump() << std::endl;
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

bool WsFacade::SendReport(const nlohmann::json& report) {
    std::lock_guard<std::mutex> lck(mtxSsnData);
    std::string data;
    if(swr::package(report,data))
    {
        mDataSocket->pending(data);
        return true;
    }
    return false;
}

bool WsFacade::SendResponse(const nlohmann::json& response) {
    std::lock_guard<std::mutex> lck(mtxSsnIns);
    std::string data;
    if(swr::package(response,data))
    {
        mMsSocket->send((const uint8_t *)data.data(), data.size());
        return true;
    }
    return false;
}

bool WsFacade::UpdateRequestHandle(
    std::function<bool(nlohmann::json&)> f) {
    requestHandle = f;
    return true;
}

}  // namespace transport
}  // namespace swr
