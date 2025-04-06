//
// Created by e.kravchenkova on 29.03.2025.
//

#pragma once
#include "../../src/Model/Repository/Repository.h"
#include <gmock/gmock.h>
#include "../../src/Model/Client.h"

class MockClientRepository : public Repository<Client> {
public:
    MOCK_METHOD((std::pair<bool,uint64_t>), Add,(const Client&), (override));
    MOCK_METHOD(bool, Delete,(uint64_t id), (override));
    MOCK_METHOD(bool, Modify,(const Client& model), (override));
    MOCK_METHOD(std::vector<Client>, getAll,(), (override));
    MOCK_METHOD((std::pair<bool,Client>), getById,(uint64_t id), (override));
    MOCK_METHOD(uint64_t , DeleteAll,(), (override));
};
