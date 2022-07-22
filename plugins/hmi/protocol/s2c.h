#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>
#include <vector>

namespace abby {
namespace service_types {

struct A
{
    double price;
    std::string name;
};

struct B
{
    int age;
    std::string name;
};

struct C
{
    int weight;
    std::string name;
};

}  // namespace hmi_types
}  // namespace swr

#endif
