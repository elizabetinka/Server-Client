//
// Created by e.kravchenkova on 23.03.2025.
//

#pragma once

//#include "vector"
#include "Defs/ClientDefs.h"
#include "../Model/Repository/ClientRepository.h"
//#include "../Model/Client.h"
#include <unordered_map>

class ClientService {
public:
    ClientRegistrRes process(ClientRegistrReq const&);
    ClientModifyRes process(ClientModifyReq const&);
    ClientGetALlRes process(ClientGetALlReq const&);
    ClientDeleteRes process(ClientDeleteReq const&);
    ClientDeleteAllRes process(ClientDeleteAllReq const&);

private:
//    static uint64_t clients_count;
    static uint64_t last_id;
    ClientRepository clientRepository;
//    std::unordered_map<uint64_t,Client> clients;
};

