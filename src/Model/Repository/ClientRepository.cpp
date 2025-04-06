//
// Created by e.kravchenkova on 25.03.2025.
//

#include "ClientRepository.h"

uint64_t ClientRepository::last_id = 0;

std::pair<bool,uint64_t> ClientRepository::Add(const Client& model) {
    if (clients.contains(model.id)){
        return std::pair<bool,uint64_t>(false,undefId);
    }
    uint64_t newId = last_id;
    last_id++;
    clients[newId] = model;
    clients[newId].id = newId;
    return std::make_pair(true,newId);
}
bool ClientRepository::Delete(uint64_t id) {
    if (!clients.contains(id)){
        return false;
    }
    size_t deleted = clients.erase(id);
    if (deleted == 0){
        return false;
    }
    return true;
}
bool ClientRepository::Modify(const Client& model) {
    if (!clients.contains(model.id)){
        return false;
    }
    clients[model.id] = model;
    return true;
}
std::vector<Client> ClientRepository::getAll() {
    std::vector<Client> ans;
    ans.reserve(clients.size());
    for (auto const& [id, client] : clients) {
        ans.push_back(client);
    }
    return ans;
}

std::pair<bool,Client> ClientRepository::getById(uint64_t id) {
    if (!clients.contains(id)){
        return std::make_pair(false, Client());
    }
    return std::make_pair(true, clients[id]);
}

uint64_t ClientRepository::DeleteAll() {
    uint64_t ans = clients.size();
    clients.clear();
    last_id = 0;
    return ans;
}