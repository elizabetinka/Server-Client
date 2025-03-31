//
// Created by e.kravchenkova on 24.03.2025.
//

#pragma once
#include <unordered_map>
#include "../Model/Item.h"
#include "Defs/ItemDefs.h"
#include "../Model/Repository/ItemRepository.h"
#include "IItemService.h"
#include <memory>

class ItemService : public IItemService {
public:
    explicit ItemService( std::shared_ptr<Repository<ItemInfo>> itemRepository2);
    ItemService() = default;

    AddItemRes process(AddItemReq const&) override;
    ItemModifyRes process(ItemModifyReq const&) override;
    DeleteItemRes process(DeleteItemReq const&) override;
    GetAllItemRes process(GetAllItemReq const&) override;
    ItemDeleteAllRes process(ItemDeleteAllReq const&) override;

private:
    static uint64_t last_id;
    std::shared_ptr<Repository<ItemInfo>> itemRepository = std::make_shared<ItemRepository>();
    //std::unordered_map<uint64_t,ItemInfo> items;
};

