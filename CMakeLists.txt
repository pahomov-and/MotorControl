cmake_minimum_required(VERSION 3.0)
project(MotorControl)

set(CMAKE_CXX_STANDARD 14)


find_package(PkgConfig)

pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.h
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )



include_directories(".")

file(GLOB SRC
        main.cpp
        Control.h
        Control.cpp
        "SPI/*.h"
        "SPI/*.cpp"
        "PT/*.h"
        "PT/*.cpp"
        "Configs/*.h"
        "Configs/*.cpp"
        )

add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${ZeroMQ_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME}
        pthread
        jsoncpp
        ${ZeroMQ_LIBRARY}
        )