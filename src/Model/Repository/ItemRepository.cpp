//
// Created by e.kravchenkova on 25.03.2025.
//

#include "ItemRepository.h"

uint64_t ItemRepository::last_id = 0;

bool ItemRepository::Add(const ItemInfo& model) {
    if (items.contains(model.item.id)){
        return false;
    }
    uint64_t newId = last_id;
    last_id++;
    items[newId] = model;
    items[newId].item.id = newId;
    return true;
}
bool ItemRepository::Delete(uint64_t id) {
    if (!items.contains(id)){
        return false;
    }
    size_t deleted = items.erase(id);
    if (deleted == 0){
        return false;
    }
    return true;
}
bool ItemRepository::Modify(const ItemInfo& model){
    if (!items.contains(model.item.id)){
        return false;
    }
    items[model.item.id] = model;
    return true;
}
std::vector<ItemInfo> ItemRepository::getAll(){
    std::vector<ItemInfo> ans;
    ans.reserve(items.size());
    for (auto const& [id, item] : items) {
        ans.push_back(item);
    }
    return ans;
}

std::pair<bool,ItemInfo> ItemRepository::getById(uint64_t id) {
    if (!items.contains(id)){
        return std::make_pair(false, ItemInfo());
    }
    return std::make_pair(true, items[id]);
}


uint64_t ItemRepository::DeleteAll() {
    uint64_t ans = items.size();
    items.clear();
    last_id = 0;
    return ans;
}