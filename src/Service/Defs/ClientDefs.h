//
// Created by e.kravchenkova on 23.03.2025.
//
# pragma once

#include "CommonDefs.h"
#include <string>
#include <cstdint>
#include <format>
#include <sstream>
#include <unordered_map>
#include "../../Utils/timeUtil.h"
#include "../../Model/Client.h"
#include <vector>

inline const char* kTimeFormat = "%d-%m-%Y";

struct ClientRegistrReq
{
    uint64_t requestId;
    std::string nickname;
    std::string birthday;
};

struct ClientRegistrRes : Response
{
    uint64_t responceId;
    bool success;
    std::string getResultStr() const override{
        return std::format("responce id: {} result of client registration status: {}", responceId, success);
    }

    ClientRegistrRes(uint64_t responceId, bool success) : responceId(responceId),  success(success) {}
};

struct ClientDeleteReq
{
    uint64_t requestId;
    uint64_t clientId;
};

struct ClientDeleteRes : Response
{
    uint64_t responceId;
    bool success;
    std::string getResultStr() const override{
        return std::format("responce id: {} result of client delete status: {}", responceId, success);
    }

    ClientDeleteRes(uint64_t responceId, bool success) : responceId(responceId), success(success) {}
};

struct ClientGetALlReq
{
    uint64_t requestId;
};


struct ClientDto {
    uint64_t id = 0;
    std::string nickname;
    std::string birthday;

    explicit ClientDto(const Client& client) : id(client.id),nickname(client.nickname),birthday(timeToString(client.birthday, kTimeFormat)) {}
    ClientDto() = default;

};

inline std::ostream& operator << (std::ostream &os, const ClientDto &client)
{
    return os << std::format("id: {} nickname: {} birthday: {}", client.id, client.nickname, client.birthday);
}

inline std::ostream& operator << (std::ostream &os, const Response &response)
{
    return os << response.getResultStr();
}


struct ClientGetALlRes : Response
{
    uint64_t responceId;
    std::vector<ClientDto> clients;
    std::string getResultStr() const override{
        std::stringstream ss;
        ss << std::format("responce id: {} clients {} \n", responceId, clients.size());
        for (auto const& client : clients) {
            ss<< client<<"\n";
        }
        return ss.str();
    }

    ClientGetALlRes(uint64_t responceId, const std::vector<Client> &clients2) : responceId(responceId){
        for (auto const& client : clients2) {
            clients.emplace_back(client);
        }
    }
};


struct ClientDeleteAllReq
{
    uint64_t requestId;
};


struct ClientDeleteAllRes : Response
{
    uint64_t responceId;
    uint64_t deleted;

    std::string getResultStr() const override{
        return std::format("delete {} clients \n", deleted);
    }

    ClientDeleteAllRes(uint64_t responceId2, uint64_t deleted2) : responceId(responceId2),deleted(deleted2){}
};

struct ClientModifyReq
{
    uint64_t requestId;
    ClientDto new_client;
};


struct ClientModifyRes : Response
{
    uint64_t responceId;
    bool success;

    std::string getResultStr() const override{
        return std::format("modify client was {} \n", success);
    }

    ClientModifyRes(uint64_t responceId2, uint64_t success2) : responceId(responceId2),success(success2){}
};