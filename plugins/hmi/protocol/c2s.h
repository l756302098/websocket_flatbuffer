#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>
#include <vector>

namespace abby {
namespace client_types {

struct C_A
{
    double price;
    std::string name;
};

struct C_B
{
    int age;
    std::string name;
};

struct C_C
{
    int weight;
    std::string name;
};

}  // namespace hmi_types
}  // namespace swr

#endif
