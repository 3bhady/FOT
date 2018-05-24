cmake_minimum_required(VERSION 3.8)
project(FOT)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp SecurityName.cpp SecurityName.h utility.h)
find_package(Threads REQUIRED)
add_executable(FOT ${SOURCE_FILES})
target_link_libraries(FOT -lpthread)