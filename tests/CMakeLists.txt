project(ServerTest)
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        ${PROJECT_NAME}
        mock/MockClientRepository.h
        mock/MockItemRepository.h
        mock/MockClientService.h
        mock/MockItemService.h
        integration/ItemIntegrationServiceTests.cpp
        integration/ClientIntegrationServiceTests.cpp
        ut/ClientRepositoryTests.cpp
        ut/ItemRepositoryTests.cpp
        ut/ClientServiceTests.cpp
        ut/ItemServiceTests.cpp
        ut/HttpServerTests.cpp
)

target_link_libraries(${PROJECT_NAME}
        PresentationLayerHttpServer
        GTest::gtest_main
        gmock_main
)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME})