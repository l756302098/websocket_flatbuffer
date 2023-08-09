/*
 * TCPFacade.h
 *
 *  Created on: Jun 11, 2021
 *      Author: ubuntu
 */

#pragma once

#include <mutex>
#include "ws/socket_server.hpp"
#include "ws/message_helper.hpp"

namespace swr {
namespace transport {

class WsFacade {
public:
    WsFacade(std::string configPath);
    bool IsConntected();
    bool Accept(std::function<void(bool connected)>);
    bool Start();
    bool Stop();
    bool Pause();
    bool Resume();
    bool SendReport(const nlohmann::json& report);
    bool SendResponse(const nlohmann::json& response);
    bool UpdateRequestHandle(std::function<bool(nlohmann::json& req)> f);

private:
    std::shared_ptr<swr::socket_server> mMsSocket;
    std::shared_ptr<swr::socket_server> mDataSocket;
    std::function<void(bool connected)> OnConnectionTriggered;
    std::mutex mtxSenssionCounter;
    std::mutex mtxSsnIns;
    std::mutex mtxSsnData;
    int sessionCounter;

    std::function<bool(nlohmann::json& req)> requestHandle;
};

}  // namespace transport
}  // namespace swr
