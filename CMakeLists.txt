cmake_minimum_required(VERSION 4.1)
project(DA2)

set(CMAKE_CXX_STANDARD 20)

add_executable(DA2 src/main.cpp
        include/Menu.h
        src/Menu.cpp
        include/RangeParser.h
        include/RegisterParser.h
        include/ProgramPoint.h
        include/LiveRange.h
        include/AllocationConfig.h
        src/RangeParser.cpp
        src/RegisterParser.cpp
        include/Web.h
        include/WebBuilder.h
        src/WebBuilder.cpp)
