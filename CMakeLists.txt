cmake_minimum_required(VERSION 4.1)
project(CS2013_CommandCenter_)

set(CMAKE_CXX_STANDARD 14)

add_executable(CS2013_CommandCenter_
        main.cpp
        src/Entity.cpp
        includes/Entity.h
        src/Item.cpp
        includes/item.h
        src/CommandCenter.cpp
        includes/CommandCenter.h)
