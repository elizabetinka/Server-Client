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

class ClientRepositoryTestsValid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<Client>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryTestsValidTest, ClientRepositoryTestsValid,
        ::testing::Values(
                Client("1", time(nullptr)),
                Client("2", time(nullptr)),
                Client("3", time(nullptr)),
                Client("4", time(nullptr))
        ));

TEST_P(ClientRepositoryTestsValid, CClientRepositoryTestsValidTest) {
    auto const& client = GetParam();
    auto responce = clientRepository.Add(client);
    ASSERT_EQ(responce, true);
}


class ClientRepositoryTestsInvalid
        : public  ClientRepositoryTests, public ::testing::WithParamInterface<Client>
{};

INSTANTIATE_TEST_SUITE_P(
        ClientRepositoryTestsInvalidTest, ClientRepositoryTestsInvalid,
        ::testing::Values(
                Client("1", time(nullptr)),
                Client("2", time(nullptr)),
                Client("3", time(nullptr)),
                Client("4", time(nullptr))
        ));

TEST_P(ClientRepositoryTestsInvalid, ClientRepositoryTestsInvalidTest) {
    auto client = GetParam();
    client.id = 0;
    auto responce = clientRepository.Add(client);
    ASSERT_EQ(responce, false);
}