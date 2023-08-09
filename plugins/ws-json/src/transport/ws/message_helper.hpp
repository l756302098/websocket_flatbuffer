#pragma once
#include <iostream>
#include "../json.hpp"

namespace swr
{

    static bool package(const nlohmann::json &builder,std::string& result)
    {
        try
        {
            result =  builder.dump();
            return true;
        }
        catch(nlohmann::detail::type_error const& e)
        {
            std::cerr << "failed to package json to string: " << builder << ",error: " << e.what();
        }
        catch (...)
        {
            std::cerr << "package error." << std::endl;
        }
        return false;
    }

    static bool unpackage(const std::string& buf,nlohmann::json &builder)
    {
        try
        {
            builder = nlohmann::json::parse(buf);
            return true;
        }
        catch(nlohmann::detail::type_error const& e)
        {
            std::cerr << "failed to parse msg. string: " << buf << ",error: " << e.what();
        }
        catch (...)
        {
            std::cerr << "unpackage error." << std::endl;
        }
        return false;
    }

}
