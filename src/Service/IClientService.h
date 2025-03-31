//
// Created by e.kravchenkova on 30.03.2025.
//

#pragma once
#include "Defs/ClientDefs.h"

class IClientService {
public:
    virtual ~IClientService() = default;
    virtual ClientRegistrRes process(ClientRegistrReq const &)=0;

    virtual ClientModifyRes process(ClientModifyReq const &)=0;

    virtual ClientGetALlRes process(ClientGetALlReq const &)=0;

    virtual ClientDeleteRes process(ClientDeleteReq const &)=0;

    virtual ClientDeleteAllRes process(ClientDeleteAllReq const &)=0;
};