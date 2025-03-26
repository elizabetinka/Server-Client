//
// Created by e.kravchenkova on 24.03.2025.
//

#pragma once
#include <cstdint>
#include <string>
#include "ModelDefs.h"

enum Category{
    clothes,
    technic,
    beauty,
    unknown
};

struct Item {
    uint64_t id = undefId;
    std::string name = "";
    std::string description = "";
    Category category;

    Item() = default;

    Item(const std::string &name, const std::string &description, Category category);
};


struct ItemInfo {
    Item item;
    uint64_t count;
};