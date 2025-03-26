//
// Created by e.kravchenkova on 25.03.2025.
//

#include <gtest/gtest.h>
#include "../src/Service/ClientService.h"

class ClientServiceTests : public testing::Test {

protected:
    ClientService clientService;

    void addClients(){
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "b", .birthday = "07-05-2004"});
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "c", .birthday = "07-05-2004"});
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "d", .birthday = "07-05-2004"});
    }

public:
    virtual ~ClientServiceTests() override {
        clientService.process(ClientDeleteAllReq{});
    }
};

class ServiceTestsClientServiceRequestId
        : public ClientServiceTests, public ::testing::WithParamInterface<uint64_t>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceRegistrationReqestId, ServiceTestsClientServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceTestsClientServiceRequestId, ClientServiceRegistrationReqestId) {
    auto const& id = GetParam();
    auto request = ClientRegistrReq{.requestId = id, .nickname = "name", .birthday = "07-05-2004"};
    auto responce = clientService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceTestsClientServiceRegistrationValid
        : public  ClientServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceRegistrationValid, ServiceTestsClientServiceRegistrationValid,
        ::testing::Values(
                std::make_tuple("Liza", "07-05-2004"),
                std::make_tuple("Лиза", "07-05-2004"),
                std::make_tuple("Л", "07-05-2004"),
                std::make_tuple("Makar", "04-05-1070")
                ));

TEST_P(ServiceTestsClientServiceRegistrationValid, ClientServiceRegistrationValid) {
    auto const& [name, birthday] = GetParam();
    auto request = ClientRegistrReq{.requestId = 0, .nickname = name, .birthday = birthday};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceTestsClientServiceRegistrationInvalid
        : public  ClientServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceRegistrationInvalid, ServiceTestsClientServiceRegistrationInvalid,
        ::testing::Values(
                std::make_tuple("Liza", "07-24-2004"),
                std::make_tuple("Liza", "2004-07-05"),
                std::make_tuple("Liza", ""),
                std::make_tuple("Liza", " "),
                std::make_tuple("", "07-05-2004"),
                std::make_tuple(" ", "07-05-2004"),
                std::make_tuple("", ""),
                std::make_tuple("     ", " ")
        ));

TEST_P(ServiceTestsClientServiceRegistrationInvalid, ClientServiceRegistrationInvalid) {
    auto const& [name, birthday] = GetParam();
    auto request = ClientRegistrReq{.requestId = 0, .nickname = name, .birthday = birthday};
    auto response = clientService.process(request);
    ASSERT_EQ(response.success, false);
}


INSTANTIATE_TEST_SUITE_P(
        ClientServiceDeleteReqestId, ServiceTestsClientServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceTestsClientServiceRequestId, ClientServiceDeleteReqestId) {
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = id, .clientId = 0};
    auto responce = clientService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceTestsClientServiceDeleteValid
        : public  ClientServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceDeleteValid, ServiceTestsClientServiceDeleteValid,
        ::testing::Values(0,1,2,3));

TEST_P(ServiceTestsClientServiceDeleteValid, ClientServiceDeleteValid) {
    addClients();
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = 0, .clientId = id};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceTestsClientServiceDeleteInvalid
        : public  ClientServiceTests, public ::testing::WithParamInterface<uint64_t>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceDeleteInvalid, ServiceTestsClientServiceDeleteInvalid,
        ::testing::Values(4,5,6,7));

TEST_P(ServiceTestsClientServiceDeleteInvalid, ClientServiceDeleteInvalid) {
    addClients();
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = 0, .clientId = id};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, false);
}

class ServiceTestsClientServiceDeleteChangeLenValid
        : public  ClientServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceDeleteChangeLenValid, ServiceTestsClientServiceDeleteChangeLenValid,
        ::testing::Values(0,1,2,3));

TEST_P(ServiceTestsClientServiceDeleteChangeLenValid, ClientServiceDeleteChangeLenValid) {
    addClients();
    auto const& id = GetParam();
    auto n1 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    auto responce = clientService.process(ClientDeleteReq{.requestId = 0, .clientId = id});
    auto n2 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(responce.success, true);
    ASSERT_EQ(n2, n1-1);
}

class ServiceTestsClientServiceDeleteChangeLenInvalid
        : public  ClientServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceDeleteChangeLenInvalid, ServiceTestsClientServiceDeleteChangeLenInvalid,
        ::testing::Values(4,5,6,7));

TEST_P(ServiceTestsClientServiceDeleteChangeLenInvalid, ClientServiceDeleteChangeLenInvalid) {
    addClients();
    auto const& id = GetParam();
    auto n1 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    auto responce = clientService.process(ClientDeleteReq{.requestId = 0, .clientId = id});
    auto n2 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(responce.success, false);
    ASSERT_EQ(n2, n1);
}

TEST_F( ClientServiceTests, GetAllTest) {
    addClients();
    auto n = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(n, 4);
}

TEST_F( ClientServiceTests, GetAllTestWithAdding) {
    addClients();
    auto n1 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
    clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
    clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
    auto n2 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(n2, n1+3);
}

