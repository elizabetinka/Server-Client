//
// Created by e.kravchenkova on 25.03.2025.
//

#pragma once
#include "Repository.h"
#include "../Item.h"
#include <cstdint>
#include <unordered_map>


class ItemRepository : public Repository<ItemInfo> {
public:
    bool Add(const ItemInfo& model) override;
    bool Delete(uint64_t id) override;
    bool Modify(const ItemInfo& model) override;
    std::vector<ItemInfo> getAll() override;
    std::pair<bool,ItemInfo> getById(uint64_t id) override ;
    uint64_t DeleteAll() override;
private:
    static uint64_t last_id;
    std::unordered_map<uint64_t,ItemInfo> items;
};



