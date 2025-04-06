//
// Created by e.kravchenkova on 23.03.2025.
//

#include "ClientService.h"
#include <iostream>

//uint64_t ClientService::clients_count = 0;


ClientRegistrRes ClientService::process(ClientRegistrReq const& req){
    if (isEmptyOrOnlySpaceString(req.birthday) || isEmptyOrOnlySpaceString(req.nickname)){
        return {req.requestId,  false};
    }

    auto timePair = stringToTime(req.birthday,kTimeFormat);
    if (!timePair.first){
        return {req.requestId,  false};
    }
    auto client = Client(req.nickname, timePair.second);
    auto status = clientRepository->Add(client);
    return {req.requestId, status.second,status.first};
}

ClientGetALlRes ClientService::process(ClientGetALlReq const& req){

    return {req.requestId,clientRepository->getAll()};
}

ClientDeleteRes ClientService::process(ClientDeleteReq const& req){
    auto success = clientRepository->Delete(req.clientId);
//    clients_count--;
    return {req.requestId, success};
}

ClientDeleteAllRes ClientService::process(const ClientDeleteAllReq & req) {
    return {req.requestId, clientRepository->DeleteAll()};
}

ClientModifyRes ClientService::process(const ClientModifyReq & req) {
    if (isEmptyOrOnlySpaceString(req.new_client.birthday) || isEmptyOrOnlySpaceString(req.new_client.nickname)){
        return {req.requestId,  false};
    }

    auto timePair = stringToTime(req.new_client.birthday,kTimeFormat);
    if (!timePair.first){
        return {req.requestId,  false};
    }
    Client client= Client(req.new_client.nickname, timePair.second);
    client.id = req.new_client.id;
    return {req.requestId,clientRepository->Modify(client)};
}

ClientService::ClientService(std::shared_ptr<Repository<Client>> clientRepository2) : clientRepository(std::move(clientRepository2)) {}
