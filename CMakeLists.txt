cmake_minimum_required(VERSION 3.28)
project(Mapper)

find_package(pybind11 CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 23)
pybind11_add_module(MapperLib PythonModule.cpp
                            SingleLinkage.cpp
                            LinalgHelpers.cpp
                            DataCover.cpp
                            Mapper.cpp
                            Projection.cpp
                            CechComplex.cpp
                            SLink_SingleLinkage.cpp)
target_compile_options(MapperLib PRIVATE ${MY_COMPILE_OPTIONS})
target_link_libraries(MapperLib PRIVATE ${MY_LIBRARY_DEPENDENCIES})

add_executable(Mapper main.cpp
        SingleLinkage.cpp
        LinalgHelpers.cpp
        DataCover.cpp
        Mapper.cpp
        Projection.cpp
        CechComplex.cpp
        SLink_SingleLinkage.cpp)
