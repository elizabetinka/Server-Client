//
// Created by e.kravchenkova on 24.03.2025.
//

#include "ItemService.h"


AddItemRes ItemService::process(AddItemReq const& req){
    if (isEmptyOrOnlySpaceString(req.name) || req.count == 0){
        return {req.requestId,  false};
    }

    auto item = Item(req.name, req.description, req.category);
    auto status = itemRepository->Add(ItemInfo(item, req.count));
    return {req.requestId,  status.second,status.first};
}

DeleteItemRes ItemService::process(DeleteItemReq const& req){
    if (req.count == 0){
        return {req.requestId,false};
    }

    auto itemPair = itemRepository->getById(req.itemId);
    if (!itemPair.first){
        return {req.requestId,false};
    }

    uint64_t deleted = std::min(req.count, itemPair.second.count);
    itemPair.second.count-=deleted;

    bool success = false;

    if (itemPair.second.count==0){
        success = itemRepository->Delete(req.itemId);
    }
    else{
        success = itemRepository->Modify(itemPair.second);
    }

//    clients_count--;
    return {req.requestId, success, itemPair.second.item, deleted};
}
GetAllItemRes ItemService::process(GetAllItemReq const& req){
    return {req.requestId, itemRepository->getAll()};
}

ItemDeleteAllRes ItemService::process(const ItemDeleteAllReq & req) {
    return {req.requestId, itemRepository->DeleteAll()};
}

ItemModifyRes ItemService::process(const ItemModifyReq & req) {
    if (isEmptyOrOnlySpaceString(req.new_item.item.name) || req.new_item.count == 0){
        return {req.requestId,  false};
    }

    return {req.requestId,itemRepository->Modify(req.new_item)};
}

ItemService::ItemService(std::shared_ptr<Repository<ItemInfo>> itemRepository2): itemRepository(std::move(itemRepository2)) {}
