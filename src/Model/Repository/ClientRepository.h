//
// Created by e.kravchenkova on 25.03.2025.
//

#pragma once
#include "Repository.h"
#include "../Client.h"
#include <cstdint>
#include <unordered_map>


class ClientRepository : Repository<Client> {
public:
    bool Add(const Client& model) override;
    bool Delete(uint64_t id) override;
    bool Modify(const Client& model) override;
    std::vector<Client> getAll() override;
    std::pair<bool,Client> getById(uint64_t id) override ;
    uint64_t DeleteAll() override;
private:
    static uint64_t last_id;
    std::unordered_map<uint64_t,Client> clients;
};

