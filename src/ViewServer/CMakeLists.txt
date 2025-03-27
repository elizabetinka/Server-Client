cmake_minimum_required(VERSION 3.12)
project(PresentationLayerServer)

add_library(${PROJECT_NAME}
        Server.cpp
        Server.h
)

include(FetchContent)

FetchContent_Declare(
        Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.84.0
)

FetchContent_MakeAvailable(Boost)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PUBLIC
        ${Boost_SOURCE_DIRS}
)

target_link_libraries(${PROJECT_NAME} BusinessLayer Boost::system Boost::program_options Boost::asio ws2_32)

#target_link_libraries(${PROJECT_NAME}
#        PRIVATE
#        BusinessLayer
#        PUBLIC
#        ${Boost_LIBRARIES}
#)

#include_directories(${Boost_SOURCE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE BusinessLayer PUBLIC Boost::system Boost::program_options Boost::regex ws2_32)
##target_link_libraries(${PROJECT_NAME} PRIVATE BusinessLayer)