cmake_minimum_required(VERSION 3.15)
project(Client)

set(CMAKE_CXX_STANDARD 11)
link_libraries(-pthread)
link_libraries(-lboost_system)
include_directories(.)
include_directories(bin)
include_directories(include)
include_directories(src)

add_executable(Client main.cpp
        include/connectionHandler.h
        src/connectionHandler.cpp src/Client.cpp include/Client.h src/Message.cpp include/Message.h include/Transmitter.h src/Transmitter.cpp include/BlockingQueue.h include/Inventory.h src/Inventory.cpp)