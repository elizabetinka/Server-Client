//
// Created by e.kravchenkova on 29.03.2025.
//

#pragma once
#include "../../src/Model/Repository/Repository.h"
#include <gmock/gmock.h>
#include "../../src/Model/Item.h"

class MockItemRepository : public Repository<ItemInfo> {
public:
    MOCK_METHOD(bool, Add,(const ItemInfo&), (override));
    MOCK_METHOD(bool, Delete,(uint64_t id), (override));
    MOCK_METHOD(bool, Modify,(const ItemInfo& model), (override));
    MOCK_METHOD(std::vector<ItemInfo>, getAll,(), (override));
    MOCK_METHOD((std::pair<bool,ItemInfo>), getById,(uint64_t id), (override));
    MOCK_METHOD(uint64_t , DeleteAll,(), (override));
};
