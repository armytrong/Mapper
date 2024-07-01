cmake_minimum_required(VERSION 3.28)
project(Mapper)

set(CMAKE_CXX_STANDARD 20)

add_executable(Mapper main.cpp
        SingleLinkage.cpp
        LinalgHelpers.cpp
        DataCover.cpp
        Mapper.cpp
        Projection.cpp
        CechComplex.cpp)
