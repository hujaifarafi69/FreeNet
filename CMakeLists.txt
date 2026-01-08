cmake_minimum_required(VERSION 3.10)
project(ChatClone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")

include_directories(common server client)

add_executable(server server/main.cpp server/server.cpp server/client_handler.cpp)
add_executable(client client/main.cpp client/gui.cpp client/network.cpp)
