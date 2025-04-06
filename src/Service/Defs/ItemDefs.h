//
// Created by e.kravchenkova on 24.03.2025.
//

#pragma once
#include <cstdint>
#include "../../Model/Item.h"
#include "CommonDefs.h"
#include <format>
#include <vector>
#include <sstream>

struct AddItemReq
{
    uint64_t requestId;
    std::string name = "";
    std::string description = "";
    uint64_t count=0;
    Category category;

};

struct AddItemRes : Response
{
    uint64_t responceId;
    bool success;
    uint64_t id;

    std::string getResultStr() const override{
        return std::format("responce id: {} result of adding item  status: {}", responceId, success);
    }

    AddItemRes(uint64_t responceId, bool success) : responceId(responceId), success(success){}
    AddItemRes(uint64_t responceId,uint64_t id,bool success) : responceId(responceId), id(id), success(success) {}

};


struct DeleteItemReq
{
    uint64_t requestId;
    uint64_t itemId;
    uint64_t count=0;
};

struct DeleteItemRes : Response
{
    uint64_t responceId;
    bool success;
    Item item;
    uint64_t bought_count=0;

    std::string getResultStr() const override{
        return std::format("responce id: {} result of buying item_id {} count {} status: {}", responceId, item.id, bought_count, success);
    }

    DeleteItemRes(uint64_t responceId, bool success) : responceId(responceId),success(success){}

    DeleteItemRes(uint64_t responceId, bool success, const Item &item, uint64_t boughtCount) : responceId(responceId),
                                                                                            success(success),
                                                                                            item(item),
                                                                                            bought_count(boughtCount) {}
};


struct GetAllItemReq
{
    uint64_t requestId;
};

inline std::string castCategoryToString(Category category){
    if (category == Category::clothes) {
        return "clothes";
    } else if (category == Category::beauty) {
        return "beauty";
    } else if (category == Category::technic) {
        return "technic";
    } else {
        return "unknown";
    }
}

inline std::ostream& operator << (std::ostream &os, const ItemInfo &itemInf)
{
    return os << std::format("id: {} name: {} description: {} category {} count {}", itemInf.item.id, itemInf.item.name, itemInf.item.description,castCategoryToString(itemInf.item.category), itemInf.count);
}


struct GetAllItemRes : Response
{
    uint64_t responceId;
    std::vector<ItemInfo> items;

    std::string getResultStr() const override{
        std::stringstream ss;
        ss << std::format("responce id: {} count item {} \n", responceId, items.size());
        for (auto const& item : items) {
            ss<< item<<"\n";
        }
        return ss.str();
    }

    GetAllItemRes(uint64_t responceId, const std::vector<ItemInfo> &items2) : responceId(responceId){
            for (auto const& item : items2) {
                items.push_back(item);
            }
    }
};

struct ItemDeleteAllReq
{
    uint64_t requestId;
};


struct ItemDeleteAllRes: Response
{
    uint64_t responceId;
    uint64_t deleted;

    std::string getResultStr() const override{
        return std::format("delete {} items \n", deleted);
    }

    ItemDeleteAllRes(uint64_t responceId2,uint64_t deleted2) : responceId(responceId2),deleted(deleted2){}
};


struct ItemModifyReq
{
    uint64_t requestId;
    ItemInfo new_item;
};


struct ItemModifyRes : Response
{
    uint64_t responceId;
    bool success;

    std::string getResultStr() const override{
        return std::format("modify item was {} \n", success);
    }

    ItemModifyRes(uint64_t responceId2, uint64_t success2) : responceId(responceId2),success(success2){}
};
