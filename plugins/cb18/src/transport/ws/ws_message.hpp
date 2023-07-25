#pragma once
#include <iostream>

namespace swr{

class ws_message
{
private:
    std::uint32_t header_size;
    std::uint16_t mid;  //unused
    std::uint8_t *data;
    std::uint32_t data_size;

public:
    ws_message(std::uint32_t data_size)
    {
        this->mid = 0;
        this->data_size = data_size;
        data = new std::uint8_t[data_size];
    }

    ~ws_message()
    {
        if (data != nullptr)
        {
            delete[] data;
        }
    }

    std::uint8_t* get_data()
    {
        return data;
    }

    static std::uint32_t get_header_size()
    {
        return sizeof(std::uint32_t);
    }

    static std::uint32_t get_id_size()
    {
        return sizeof(std::uint16_t);
    }

    std::uint32_t get_body_size()
    {
        return data_size - sizeof(std::uint32_t)- sizeof(std::uint16_t);
    }

    std::uint32_t get_data_size()
    {
        return data_size;
    }
};
}