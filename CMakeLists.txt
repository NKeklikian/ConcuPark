cmake_minimum_required(VERSION 2.8.9)
project (ConcuPark)

#For the source files:
file(GLOB SOURCES "src/*.cpp")

include_directories(src)

add_executable(ConcuPark ${SOURCES})
set(CMAKE_CXX_FLAGS " -std=c++11" )
