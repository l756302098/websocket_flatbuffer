#pragma once
#include <iostream>
#include "ws_message.hpp"
#include "../protocol/generated/public_generated.h"
#include "../protocol/generated/report_generated.h"
#include "../protocol/generated/request_generated.h"
#include "../protocol/generated/response_generated.h"

namespace swr
{

    static std::shared_ptr<ws_message> generate(std::uint32_t bodyLength = 0)
    {
        auto msg = std::make_shared<ws_message>(bodyLength);
        return msg;
    }

    static std::shared_ptr<ws_message> package(const flatbuffers::FlatBufferBuilder &builder)
    {
        uint8_t *buf = builder.GetBufferPointer();
        std::uint32_t buf_size = builder.GetSize();
        std::uint16_t id = 0;

        std::uint32_t all_size = buf_size + ws_message::get_id_size() + ws_message::get_header_size();
        uint8_t *data = new uint8_t[all_size];
        memcpy(data, &all_size, ws_message::get_header_size());
        memcpy(data + ws_message::get_header_size(), &id, ws_message::get_id_size());
        memcpy(data + ws_message::get_id_size() + ws_message::get_header_size(), buf, buf_size);

        std::shared_ptr<ws_message> msg_ptr = generate(all_size);
        memcpy(msg_ptr->get_data(), data, all_size);
        delete[] data;
        return msg_ptr;
    }

    static const swr::Request *unpackage_request(const uint8_t *buf, std::uint32_t size)
    {
        try
        {
            std::uint32_t all_size = 0;
            std::uint16_t id = 0;
            std::uint32_t body_size = 0;
            memcpy(&all_size, buf, ws_message::get_header_size());
            if(all_size != size){
                std::cerr << "unpackage_request size error.read size:" << all_size << " size:" << size << std::endl;
                return nullptr;
            }
            memcpy(&id, buf + ws_message::get_header_size(), ws_message::get_id_size());
            body_size = all_size - ws_message::get_header_size() - ws_message::get_id_size();
            //std::cout << "unpackage_request all_size:" << all_size << " id:" << id << " body_size:" << body_size << std::endl;
            flatbuffers::Verifier verifer(buf + ws_message::get_header_size() + ws_message::get_id_size(), body_size);
            if (!VerifyRequestBuffer(verifer))
            {
                std::cerr << "invalid flatbuffers data" << std::endl;
                return nullptr;
            }
            const swr::Request *request = GetRequest(buf + ws_message::get_header_size() + ws_message::get_id_size());
            return request;
        }
        catch (...)
        {
            std::cerr << "unpackage error." << std::endl;
        }
        return nullptr;
    }

    static const swr::Response *unpackage_response(const uint8_t *buf, std::uint32_t size)
    {
        try
        {
            std::uint32_t all_size = 0;
            std::uint16_t id = 0;
            std::uint32_t body_size = 0;
            memcpy(&all_size, buf, ws_message::get_header_size());
            if(all_size != size){
                std::cerr << "unpackage_response size error." << std::endl;
                return nullptr;
            }
            memcpy(&id, buf + ws_message::get_header_size(), ws_message::get_id_size());
            body_size = all_size - ws_message::get_header_size() - ws_message::get_id_size();
            std::cout << "unpackage_response all_size:" << all_size << " id:" << id << " body_size:" << body_size << std::endl;
            flatbuffers::Verifier verifer(buf + ws_message::get_header_size() + ws_message::get_id_size(), body_size);
            if (!VerifyResponseBuffer(verifer))
            {
                std::cerr << "invalid flatbuffers data" << std::endl;
                return nullptr;
            }
            const swr::Response *request = GetResponse(buf + ws_message::get_header_size() + ws_message::get_id_size());
            return request;
        }
        catch (...)
        {
            std::cerr << "unpackage error." << std::endl;
        }
        return nullptr;
    }

    static const swr::Report *unpackage_report(const uint8_t *buf, std::uint32_t size)
    {
        try
        {
            std::uint32_t all_size = 0;
            std::uint16_t id = 0;
            std::uint32_t body_size = 0;
            memcpy(&all_size, buf, ws_message::get_header_size());
            if(all_size != size){
                std::cerr << "unpackage_report size error." << std::endl;
                return nullptr;
            }
            memcpy(&id, buf + ws_message::get_header_size(), ws_message::get_id_size());
            body_size = all_size - ws_message::get_header_size() - ws_message::get_id_size();
            std::cout << "unpackage_report all_size:" << all_size << " id:" << id << " body_size:" << body_size << std::endl;
            flatbuffers::Verifier verifer(buf + ws_message::get_header_size() + ws_message::get_id_size(), body_size);
            if (!VerifyReportBuffer(verifer))
            {
                std::cerr << "invalid flatbuffers data" << std::endl;
                return nullptr;
            }
            const swr::Report *request = GetReport(buf + ws_message::get_header_size() + ws_message::get_id_size());
            return request;
        }
        catch (...)
        {
            std::cerr << "unpackage error." << std::endl;
        }
        return nullptr;
    }

}
