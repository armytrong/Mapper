cmake_minimum_required(VERSION 3.28)
project(Mapper)

set(CMAKE_CXX_STANDARD 20)

add_executable(Mapper main.cpp
        SingleLinkage.cpp
        SingleLinkage.h
        Clusterer.h
        LinalgHelpers.cpp
        LinalgHelpers.h
        typedefs.h
        DataCover.cpp
        DataCover.h)
