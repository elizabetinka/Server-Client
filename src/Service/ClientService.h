//
// Created by e.kravchenkova on 23.03.2025.
//

#pragma once

//#include "vector"
#include "Defs/ClientDefs.h"
#include "../Model/Repository/ClientRepository.h"
//#include "../Model/Client.h"
#include <unordered_map>
#include <memory>
#include "IClientService.h"

class ClientService : public IClientService {
public:
    explicit ClientService( std::shared_ptr<Repository<Client>> clientRepository2);
    ClientService() = default;

    ClientRegistrRes process(ClientRegistrReq const&) override;
    ClientModifyRes process(ClientModifyReq const&) override;
    ClientGetALlRes process(ClientGetALlReq const&) override;
    ClientDeleteRes process(ClientDeleteReq const&) override;
    ClientDeleteAllRes process(ClientDeleteAllReq const&) override;

private:
//    static uint64_t clients_count;
    static uint64_t last_id;
    std::shared_ptr<Repository<Client>> clientRepository = std::make_shared<ClientRepository>();
//    std::unordered_map<uint64_t,Client> clients;
};

