//
// Created by e.kravchenkova on 30.03.2025.
//

#pragma once
#include "../../src/Model/Repository/Repository.h"
#include <gmock/gmock.h>
#include "../../src/Model/Item.h"
#include "../../src/Service/Defs/ItemDefs.h"
#include "../../src/Service/IItemService.h"

class MockItemService : public IItemService {
public:
    MOCK_METHOD(AddItemRes, process,(AddItemReq const&));
    MOCK_METHOD(ItemModifyRes, process,(ItemModifyReq const&));
    MOCK_METHOD(DeleteItemRes, process,(DeleteItemReq const&));
    MOCK_METHOD(GetAllItemRes, process,(GetAllItemReq const&));
    MOCK_METHOD(ItemDeleteAllRes, process,(ItemDeleteAllReq const&));
};
