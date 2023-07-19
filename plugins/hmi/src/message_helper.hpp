#pragma once
#include <iostream>
#include "../protocol/generated/s2c_generated.h"

namespace abby {

class ws_message
{
private:
    std::uint8_t *body;
    std::uint32_t size;

public:
    ws_message(std::uint32_t size)
    {
        this->size = size;
        body = new std::uint8_t[size];
    }

    ~ws_message()
    {
        if (body != nullptr)
        {
            delete[] body;
        }
    }

    std::uint8_t* get_body()
    {
        return body;
    }

    std::uint32_t get_size()
    {
        return size;
    }
};

static std::shared_ptr<ws_message> generate(std::uint32_t bodyLength = 0) {
    auto msg = std::make_shared<ws_message>(bodyLength);
    return msg;
}

static std::shared_ptr<ws_message> package(const flatbuffers::FlatBufferBuilder& builder)
{
    uint8_t *buf = builder.GetBufferPointer();
    std::uint32_t size = builder.GetSize();
    uint8_t *data = new uint8_t[size+4];
    memcpy(data,&size,4);
    memcpy(data+4,buf,size);
    std::shared_ptr<ws_message> msg_ptr = generate(size+4);
    memcpy(msg_ptr->get_body(),data,size+4);
    delete[] data;
    return msg_ptr;
}

static std::shared_ptr<ws_message> package_fixedsize(flatbuffers::FlatBufferBuilder builder)
{
    uint8_t *buf = builder.GetBufferPointer();
    std::uint32_t size = builder.GetSize();
    std::shared_ptr<ws_message> msg_ptr = generate(builder.GetSize());
    memcpy(msg_ptr->get_body(),buf,size);
    return msg_ptr;
}

static const abby::ServerData* unpackage(const uint8_t* buf,std::uint32_t size)
{
    try
    {
        std::uint32_t bodySize = 0;
        memcpy(&bodySize,buf,4);
        std::cout << "buf size:" << size << " body size:" << bodySize << std::endl;
        flatbuffers::Verifier verifer(buf + 4, size - 4);
        if (!VerifyServerDataBuffer(verifer))
        {
            std::cout << "invalid flatbuffers data" << std::endl;
            return nullptr;
        }
        const abby::ServerData *server_data = GetServerData(buf + 4);
        return server_data;
    }
    catch(...)
    {
        std::cerr  << "error." << '\n';
    }
    return nullptr;
}

static const abby::ServerData* unpackage_fixedsize(const uint8_t* buf,std::uint32_t size)
{
    try
    {
        const abby::ServerData *server_data = GetServerData(buf + 4);
        return server_data;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n' << std::endl;
    }
    return nullptr;
}
}
