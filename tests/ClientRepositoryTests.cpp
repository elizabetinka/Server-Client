//
// Created by e.kravchenkova on 27.03.2025.
//

#include "../src/Model/Repository/ClientRepository.h"
#include <gtest/gtest.h>

class ClientRepositoryTests : public testing::Test {

public:
    virtual ~ClientRepositoryTests() override {
        clientRepository.DeleteAll();
    }

protected:
    ClientRepository clientRepository;

    void addClients(){
        clientRepository.Add(Client("name", time(nullptr)));
        clientRepository.Add(Client("name", time(nullptr)));
        clientRepository.Add(Client("name", time(nullptr)));
        clientRepository.Add(Client("name", time(nullptr)));
    }
};

class ClientRepositoryAddTestsValid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<Client>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryAddTestsValidTest, ClientRepositoryAddTestsValid,
        ::testing::Values(
                Client("1", time(nullptr)),
                Client("2", time(nullptr)),
                Client("3", time(nullptr)),
                Client("4", time(nullptr))
        ));

TEST_P(ClientRepositoryAddTestsValid, CClientRepositoryAddTestsValidTest) {
    auto const& client = GetParam();
    auto responce = clientRepository.Add(client);
    ASSERT_EQ(responce, true);
}


class ClientRepositoryAddTestsInvalid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<Client>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryAddTestsInvalidTest, ClientRepositoryAddTestsInvalid,
        ::testing::Values(
                Client("1", time(nullptr)),
                Client("2", time(nullptr)),
                Client("3", time(nullptr)),
                Client("4", time(nullptr))
        ));

TEST_P(ClientRepositoryAddTestsInvalid, ClientRepositoryAddTestsInvalidTest) {
    addClients();
    auto client = GetParam();
    client.id = 0;
    auto responce = clientRepository.Add(client);
    ASSERT_EQ(responce, false);
}

class ClientRepositoryDeleteTestsValid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryDeleteTestsValidTest, ClientRepositoryDeleteTestsValid,
        ::testing::Values(0,1,2,3));

TEST_P(ClientRepositoryDeleteTestsValid, CClientRepositoryDeleteTestsValidTest) {
    addClients();
    auto const& id = GetParam();
    auto responce = clientRepository.Delete(id);
    ASSERT_EQ(responce, true);
}


class ClientRepositoryDeleteTestsInvalid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryDeleteTestsInvalidTest, ClientRepositoryDeleteTestsInvalid,
        ::testing::Values(4,5,6,7,8));

TEST_P(ClientRepositoryDeleteTestsInvalid, ClientRepositoryDeleteTestsInvalidTest) {
    addClients();
    auto const& id = GetParam();
    auto responce = clientRepository.Delete(id);
    ASSERT_EQ(responce, false);
}

class ClientRepositoryModifyTestsValid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryModifyTestsValidTest, ClientRepositoryModifyTestsValid,
        ::testing::Values(0,1,2,3));

TEST_P(ClientRepositoryModifyTestsValid, ClientRepositoryModifyTestsValidTest) {
    addClients();
    auto const& id = GetParam();
    std::string expected_name = "Liza";

    auto responce1 = clientRepository.getById(id);
    ASSERT_EQ(responce1.first, true)
    ;
    responce1.second.nickname = expected_name;
    auto responce = clientRepository.Modify(responce1.second);
    ASSERT_EQ(responce, true);

    auto responce3 = clientRepository.getById(id);
    ASSERT_EQ(responce3.first, true);
    ASSERT_EQ(responce3.second.nickname, expected_name);
}


class ClientRepositoryModifyTestsInvalid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryModifyTestsInvalidTest, ClientRepositoryModifyTestsInvalid,
        ::testing::Values(4,5,6,7,8));

TEST_P(ClientRepositoryModifyTestsInvalid, ClientRepositoryModifyTestsInvalidTest) {
    addClients();
    auto const& id = GetParam();
    std::string expected_name = "Liza";

    auto responce1 = clientRepository.getById(id);
    ASSERT_EQ(responce1.first, false);

    responce1.second.nickname = expected_name;
    auto responce = clientRepository.Modify(responce1.second);
    ASSERT_EQ(responce, false);

    auto responce3 = clientRepository.getById(id);
    ASSERT_EQ(responce3.first, false);
    ASSERT_NE(responce3.second.nickname, expected_name);
}


class ClientRepositoryGetByIdTestsValid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryGetByIdTestValid, ClientRepositoryGetByIdTestsValid,
        ::testing::Values(0,1,2,3));

TEST_P(ClientRepositoryGetByIdTestsValid, ClientRepositoryGetByIdTestValid) {
    addClients();
    auto const& id = GetParam();

    auto responce1 = clientRepository.getById(id);
    ASSERT_EQ(responce1.first, true);
    ASSERT_EQ(responce1.second.id, id);
}

class ClientRepositoryGetByIdTestsInvalid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<uint64_t >
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryGetByIdTestInvalid, ClientRepositoryGetByIdTestsInvalid,
        ::testing::Values(4,5,6,7));

TEST_P(ClientRepositoryGetByIdTestsInvalid, ClientRepositoryGetByIdTestInvalid) {
    addClients();
    auto const& id = GetParam();

    auto responce1 = clientRepository.getById(id);
    ASSERT_EQ(responce1.first, false);
    ASSERT_EQ(responce1.second.id, undefId);
}

TEST_F(ClientRepositoryTests, ClientRepositoryGetAllTest) {
    addClients();
    auto n = clientRepository.getAll().size();
    ASSERT_EQ(n,4);

    clientRepository.Add(Client("name", time(nullptr)));

    n = clientRepository.getAll().size();
    ASSERT_EQ(n,5);

    clientRepository.Delete(0);

    n = clientRepository.getAll().size();
    ASSERT_EQ(n,4);
}

TEST_F(ClientRepositoryTests, ClientRepositoryDeleteAllTest) {
    addClients();
    auto n = clientRepository.DeleteAll();
    ASSERT_EQ(n,4);

    n = clientRepository.getAll().size();
    ASSERT_EQ(n,0);
}
