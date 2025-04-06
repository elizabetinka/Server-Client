if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

#set(CMAKE_GENERATOR Ninja)
# set(CMAKE_C_COMPILER "C:\\Program Files\\JetBrains\\CLion 2024.2.1\\bin\\mingw\\bin\\gcc.exe")
#set(CMAKE_CXX_COMPILER "C:\\Program Files\\JetBrains\\CLion 2024.2.1\\bin\\mingw\\bin\\g++.exe")
#set(CMAKE_MAKE_PROGRAM "C:\\Program Files\\JetBrains\\CLion 2024.2.1\\bin\\mingw\\bin\\mingw32-make.exe")
cmake_minimum_required(VERSION 3.12)
project(Server CXX)

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