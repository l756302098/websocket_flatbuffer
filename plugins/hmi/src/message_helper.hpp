#pragma once
#include <iostream>
#include "../protocol/generated/s2c_generated.h"

namespace abby {

class ws_message
{
public:
    std::uint8_t *body;

public:
    ws_message(std::uint32_t size)
    {
        body = new std::uint8_t[size];
    }

    ~ws_message()
    {
        if (body != nullptr)
        {
            delete[] body;
        }
    }
};

static std::shared_ptr<ws_message> package(flatbuffers::FlatBufferBuilder builder)
{
    uint8_t *buf = builder.GetBufferPointer();
    std::uint32_t size = builder.GetSize();
    uint8_t *data = new uint8_t[size+4];
    memcpy(data,&size,4);
    memcpy(data+4,buf,size);
    std::shared_ptr<ws_message> msg_ptr = generate(size+4);
    memcpy(msg_ptr->body,data,size+4);
    delete data;
    return msg_ptr;
}

static std::shared_ptr<ws_message> package_fixedsize(flatbuffers::FlatBufferBuilder builder)
{
    uint8_t *buf = builder.GetBufferPointer();
    std::uint32_t size = builder.GetSize();
    std::shared_ptr<ws_message> msg_ptr = generate(builder.GetSize());
    memcpy(msg_ptr->body,buf,size);
    return msg_ptr;
}

static const abby::ServerData* unpackage(const uint8_t* buf)
{
    const abby::ServerData *server_data = GetServerData(buf);
    return server_data;
}

static std::shared_ptr<ws_message> generate(std::uint32_t bodyLength = 0) {
    auto msg = std::make_shared<ws_message>(bodyLength);
    return msg;
}
}
