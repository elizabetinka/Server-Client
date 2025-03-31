//
// Created by e.kravchenkova on 25.03.2025.
//

#include <gtest/gtest.h>
#include "../../src/Service/ClientService.h"

class ClientIntegrationServiceTests : public testing::Test {

protected:
    ClientService clientService;

    void addClients(){
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "b", .birthday = "07-05-2004"});
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "c", .birthday = "07-05-2004"});
        clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "d", .birthday = "07-05-2004"});
    }

public:
    virtual ~ClientIntegrationServiceTests() override {
        clientService.process(ClientDeleteAllReq{});
    }
};

class ServiceIntegrationTestsClientServiceRequestId
        : public ClientIntegrationServiceTests, public ::testing::WithParamInterface<uint64_t>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationRegistrationReqestId, ServiceIntegrationTestsClientServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceIntegrationTestsClientServiceRequestId, ClientServiceIntegrationRegistrationReqestId) {
    auto const& id = GetParam();
    auto request = ClientRegistrReq{.requestId = id, .nickname = "name", .birthday = "07-05-2004"};
    auto responce = clientService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceIntegrationTestsClientServiceRegistrationValid
        : public  ClientIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationRegistrationValid, ServiceIntegrationTestsClientServiceRegistrationValid,
        ::testing::Values(
                std::make_tuple("Liza", "07-05-2004"),
                std::make_tuple("Лиза", "07-05-2004"),
                std::make_tuple("Л", "07-05-2004"),
                std::make_tuple("Makar", "04-05-1070")
                ));

TEST_P(ServiceIntegrationTestsClientServiceRegistrationValid, ClientServiceIntegrationRegistrationValid) {
    auto const& [name, birthday] = GetParam();
    auto request = ClientRegistrReq{.requestId = 0, .nickname = name, .birthday = birthday};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceIntegrationTestsClientServiceRegistrationInvalid
        : public  ClientIntegrationServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationRegistrationInvalid, ServiceIntegrationTestsClientServiceRegistrationInvalid,
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

TEST_P(ServiceIntegrationTestsClientServiceRegistrationInvalid, ClientServiceIntegrationRegistrationInvalid) {
    auto const& [name, birthday] = GetParam();
    auto request = ClientRegistrReq{.requestId = 0, .nickname = name, .birthday = birthday};
    auto response = clientService.process(request);
    ASSERT_EQ(response.success, false);
}


INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationDeleteReqestId, ServiceIntegrationTestsClientServiceRequestId,
        ::testing::Values(0,1,10,100,1000));

TEST_P(ServiceIntegrationTestsClientServiceRequestId, ClientServiceIntegrationDeleteReqestId) {
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = id, .clientId = 0};
    auto responce = clientService.process(request);
    ASSERT_EQ(id,responce.responceId);
}

class ServiceIntegrationTestsClientServiceDeleteValid
        : public  ClientIntegrationServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationDeleteValid, ServiceIntegrationTestsClientServiceDeleteValid,
        ::testing::Values(0,1,2,3));

TEST_P(ServiceIntegrationTestsClientServiceDeleteValid, ClientServiceIntegrationDeleteValid) {
    addClients();
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = 0, .clientId = id};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, true);
}

class ServiceIntegrationTestsClientServiceDeleteInvalid
        : public  ClientIntegrationServiceTests, public ::testing::WithParamInterface<uint64_t>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationDeleteInvalid, ServiceIntegrationTestsClientServiceDeleteInvalid,
        ::testing::Values(4,5,6,7));

TEST_P(ServiceIntegrationTestsClientServiceDeleteInvalid, ClientServiceIntegrationDeleteInvalid) {
    addClients();
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = 0, .clientId = id};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, false);
}

class ServiceIntegrationTestsClientServiceDeleteChangeLenValid
        : public  ClientIntegrationServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceIntegrationDeleteChangeLenValid, ServiceIntegrationTestsClientServiceDeleteChangeLenValid,
        ::testing::Values(0,1,2,3));

TEST_P(ServiceIntegrationTestsClientServiceDeleteChangeLenValid, ClientServiceIntegrationDeleteChangeLenValid) {
    addClients();
    auto const& id = GetParam();
    auto n1 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    auto responce = clientService.process(ClientDeleteReq{.requestId = 0, .clientId = id});
    auto n2 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(responce.success, true);
    ASSERT_EQ(n2, n1-1);
}

class ServiceIntegrationTestsClientServiceDeleteChangeLenInvalid
        : public  ClientIntegrationServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientIntegrationServiceDeleteChangeLenInvalid, ServiceIntegrationTestsClientServiceDeleteChangeLenInvalid,
        ::testing::Values(4,5,6,7));

TEST_P(ServiceIntegrationTestsClientServiceDeleteChangeLenInvalid, ClientIntegrationServiceDeleteChangeLenInvalid) {
    addClients();
    auto const& id = GetParam();
    auto n1 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    auto responce = clientService.process(ClientDeleteReq{.requestId = 0, .clientId = id});
    auto n2 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(responce.success, false);
    ASSERT_EQ(n2, n1);
}

TEST_F( ClientIntegrationServiceTests, GetAllTest) {
    addClients();
    auto n = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(n, 4);
}

TEST_F( ClientIntegrationServiceTests, GetAllTestWithAdding) {
    addClients();
    auto n1 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
    clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
    clientService.process(ClientRegistrReq{.requestId = 0, .nickname = "a", .birthday = "07-05-2004"});
    auto n2 = clientService.process(ClientGetALlReq{.requestId = 0}).clients.size();
    ASSERT_EQ(n2, n1+3);
}

