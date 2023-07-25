cmake_minimum_required(VERSION 3.0)

project(cb18 VERSION 1.0)

add_definitions(-std=c++17)

find_package (Boost)
find_package(OpenCV 3.4 REQUIRED)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/../../
    src/
    ${Boost_INCLUDE_DIRS}
)

add_executable(cb18_server src/HMIServices.cpp src/transport/WsFacade.cpp)

target_link_libraries(cb18_server pthread ${Boost_LIBRARIES} ${OpenCV_LIBS} boost_system rt)

add_executable(cb18_client src/HMIClient.cpp)

target_link_libraries(cb18_client pthread ${Boost_LIBRARIES} ${OpenCV_LIBS} boost_system rt)
