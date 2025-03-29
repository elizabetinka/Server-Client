cmake_minimum_required(VERSION 3.12)
project(PresentationLayerHttpServer)

add_library(${PROJECT_NAME}
        HttpServer.cpp
        HttpServer.h
        Controller.h
        Controller.cpp
        jsonModelDefs.h
)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

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

target_link_libraries(${PROJECT_NAME} BusinessLayer Boost::system Boost::program_options Boost::asio  Boost::beast ws2_32 wsock32 nlohmann_json::nlohmann_json)

#target_link_libraries(${PROJECT_NAME}
#        PRIVATE
#        BusinessLayer
#        PUBLIC
#        ${Boost_LIBRARIES}
#)

#include_directories(${Boost_SOURCE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE BusinessLayer PUBLIC Boost::system Boost::program_options Boost::regex ws2_32)
##target_link_libraries(${PROJECT_NAME} PRIVATE BusinessLayer)