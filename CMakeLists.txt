cmake_minimum_required(VERSION 3.12)
project(Server C CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp)
# enable_testing()

add_subdirectory(src/Model)
add_subdirectory(src/Utils)
add_subdirectory(src/Service)
add_subdirectory(src/View)

enable_testing()
add_subdirectory(tests)

target_link_libraries(${PROJECT_NAME}
        PresentationLayer
)