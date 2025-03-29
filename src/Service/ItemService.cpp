//
// Created by e.kravchenkova on 24.03.2025.
//

#include "ItemService.h"


AddItemRes ItemService::process(AddItemReq const& req){
    if (isEmptyOrOnlySpaceString(req.name) || req.count == 0){
        return {req.requestId,  false};
    }

    auto item = Item(req.name, req.description, req.category);
    bool success = itemRepository.Add(ItemInfo(item, req.count));
    return {req.requestId,  success};
}

DeleteItemRes ItemService::process(DeleteItemReq const& req){
    if (req.count == 0){
        return {req.requestId,false};
    }

    auto itemPair = itemRepository.getById(req.itemId);
    if (!itemPair.first){
        return {req.requestId,false};
    }

    uint64_t deleted = std::min(req.count, itemPair.second.count);
    itemPair.second.count-=deleted;

    bool success = false;

    if (itemPair.second.count==0){
        success = itemRepository.Delete(req.itemId);
    }
    else{
        success = itemRepository.Modify(itemPair.second);
    }

//    clients_count--;
    return {req.requestId, success, itemPair.second.item, deleted};
}
GetAllItemRes ItemService::process(GetAllItemReq const& req){
    return {req.requestId, itemRepository.getAll()};
}

ItemDeleteAllRes ItemService::process(const ItemDeleteAllReq & req) {
    return {req.requestId, itemRepository.DeleteAll()};
}

ItemModifyRes ItemService::process(const ItemModifyReq & req) {
    return {req.requestId,itemRepository.Modify(req.new_item)};
}
