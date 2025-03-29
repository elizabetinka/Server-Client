//
// Created by e.kravchenkova on 24.03.2025.
//

#pragma once
#include <unordered_map>
#include "../Model/Item.h"
#include "Defs/ItemDefs.h"
#include "../Model/Repository/ItemRepository.h"

class ItemService {
public:
    AddItemRes process(AddItemReq const&);
    ItemModifyRes process(ItemModifyReq const&);
    DeleteItemRes process(DeleteItemReq const&);
    GetAllItemRes process(GetAllItemReq const&);
    ItemDeleteAllRes process(ItemDeleteAllReq const&);

private:
    static uint64_t last_id;
    ItemRepository itemRepository;
    //std::unordered_map<uint64_t,ItemInfo> items;
};

