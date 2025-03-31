//
// Created by e.kravchenkova on 25.03.2025.
//

#include <gtest/gtest.h>
#include "../../src/Service/ClientService.h"
#include "../mock/MockClientRepository.h"
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Return;

class ClientServiceTests : public testing::Test {

protected:
    std::shared_ptr<MockClientRepository> clientRepository = std::make_shared<MockClientRepository>();
    ClientService clientService{clientRepository};

public:
    virtual ~ClientServiceTests() override = default;
};

class ServiceTestsClientServiceValid
        : public ClientServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceRegistrationValid, ServiceTestsClientServiceValid,
        ::testing::Values(
                std::make_tuple("Liza", "07-05-2004"),
                std::make_tuple("Лиза", "07-05-2004"),
                std::make_tuple("Л", "07-05-2004"),
                std::make_tuple("Makar", "04-05-1070")
                ));

TEST_P(ServiceTestsClientServiceValid, ClientServiceRegistrationValid) {

    EXPECT_CALL(*clientRepository, Add(_)).Times(1);

    auto const& [name, birthday] = GetParam();
    auto request = ClientRegistrReq{.requestId = 0, .nickname = name, .birthday = birthday};
    clientService.process(request);
}

class ServiceTestsClientServiceInvalid
        : public ClientServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceRegistrationInvalid, ServiceTestsClientServiceInvalid,
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

TEST_P(ServiceTestsClientServiceInvalid, ClientServiceRegistrationInvalid) {

    EXPECT_CALL(*clientRepository, Add(_)).Times(0);

    auto const& [name, birthday] = GetParam();
    auto request = ClientRegistrReq{.requestId = 0, .nickname = name, .birthday = birthday};
    clientService.process(request);
}

class ServiceTestsClientServiceDeleteValid
        : public  ClientServiceTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceDeleteValid, ServiceTestsClientServiceDeleteValid,
        ::testing::Values(0,1,2,3));

TEST_P(ServiceTestsClientServiceDeleteValid, ClientServiceDeleteValid) {
    EXPECT_CALL(*clientRepository, Delete(_)).WillOnce(Return(true));
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
    EXPECT_CALL(*clientRepository, Delete(_)).WillOnce(Return(false));
    auto const& id = GetParam();
    auto request = ClientDeleteReq{.requestId = 0, .clientId = id};
    auto responce = clientService.process(request);
    ASSERT_EQ(responce.success, false);
}

class ServiceTestsClientModifyValid
        : public ClientServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceModifyValid, ServiceTestsClientModifyValid,
        ::testing::Values(
                std::make_tuple("Liza", "07-05-2004"),
                std::make_tuple("Лиза", "07-05-2004"),
                std::make_tuple("Л", "07-05-2004"),
                std::make_tuple("Makar", "04-05-1070")
        ));

TEST_P(ServiceTestsClientModifyValid, ClientServiceModifyValid) {
    EXPECT_CALL(*clientRepository, Modify(_)).Times(1);

    auto const& [name, birthday] = GetParam();
    auto request = ClientModifyReq{.requestId = 0, .new_client = ClientDto(0,name, birthday)};
    clientService.process(request);
}

class ServiceTestsClientModifyInvalid
        : public ClientServiceTests, public ::testing::WithParamInterface<std::tuple<std::string, std::string>>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientServiceModifyInvalid, ServiceTestsClientModifyInvalid,
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

TEST_P(ServiceTestsClientModifyInvalid, ClientServiceModifyInvalid) {
    EXPECT_CALL(*clientRepository, Modify(_)).Times(0);

    auto const& [name, birthday] = GetParam();
    auto request = ClientModifyReq{.requestId = 0, .new_client = ClientDto(0,name, birthday)};
    clientService.process(request);
}

TEST_F( ClientServiceTests, GetAllTest) {
    EXPECT_CALL(*clientRepository, getAll()).Times(1);
    clientService.process(ClientGetALlReq{.requestId = 0});
}
