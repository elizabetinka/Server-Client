//
// Created by e.kravchenkova on 23.03.2025.
//

#pragma once
#include <cstdint>
#include <string>
#include <ctime>
#include <utility>
#include "ModelDefs.h"


struct Client {
    uint64_t id = undefId;
    std::string nickname = "";
    time_t birthday = std::time(nullptr);

    Client(const std::string& nickname, time_t birthday) :  nickname(nickname),
                                                                        birthday(birthday) {}

    Client() = default;
};
