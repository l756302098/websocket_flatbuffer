cmake_minimum_required(VERSION 3.0)

project(hmi VERSION 1.0)

add_definitions(-std=c++17)

find_package (Boost)

include_directories(
    ${PROJECT_SOURCE_DIR}/../../
    src/
    ${Boost_INCLUDE_DIRS}
)

add_executable(hmi_plugin src/HMINode.cpp)

target_link_libraries(hmi_plugin pthread ${Boost_LIBRARIES} boost_system)
