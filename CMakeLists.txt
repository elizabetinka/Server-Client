if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

cmake_minimum_required(VERSION 3.12)
project(Server C CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)
# enable_testing()

add_subdirectory(src/Model)
add_subdirectory(src/Utils)
add_subdirectory(src/Service)
add_subdirectory(src/HttpServer)

enable_testing()
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME}
        PresentationLayerHttpServer
)