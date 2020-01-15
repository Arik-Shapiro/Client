cmake_minimum_required(VERSION 3.15)
project(Client)

set(CMAKE_CXX_STANDARD 11)
link_libraries(-pthread)
link_libraries(-lboost_system)
include_directories(.)

add_executable(Client main.cpp
        include/connectionHandler.h
        src/connectionHandler.cpp src/Client.cpp include/Client.h src/Message.cpp include/Message.h include/Transmitter.h src/Transmitter.cpp include/BlockingQueue.h include/Inventory.h src/Inventory.cpp include/ClientProtocol.h src/ClientProtocol.cpp)