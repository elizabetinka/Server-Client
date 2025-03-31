//
// Created by e.kravchenkova on 30.03.2025.
//

#pragma once
#include "Defs/ItemDefs.h"

class IItemService {
public:
    virtual ~IItemService() = default;
    virtual AddItemRes process(AddItemReq const&) = 0;
    virtual ItemModifyRes process(ItemModifyReq const&) = 0;
    virtual DeleteItemRes process(DeleteItemReq const&) = 0;
    virtual GetAllItemRes process(GetAllItemReq const&) = 0;
    virtual ItemDeleteAllRes process(ItemDeleteAllReq const&) = 0;
};
