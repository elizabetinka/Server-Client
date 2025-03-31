//
// Created by e.kravchenkova on 30.03.2025.
//

#pragma once
#include "../../src/Model/Repository/Repository.h"
#include <gmock/gmock.h>
#include "../../src/Service/IClientService.h"
#include "../../src/Service/Defs/ClientDefs.h"

class MockClientService : public IClientService {
public:
    MOCK_METHOD(ClientRegistrRes, process,(ClientRegistrReq const&));
    MOCK_METHOD(ClientModifyRes, process,(ClientModifyReq const&));
    MOCK_METHOD(ClientGetALlRes, process,(ClientGetALlReq const&));
    MOCK_METHOD(ClientDeleteRes, process,(ClientDeleteReq const&));
    MOCK_METHOD(ClientDeleteAllRes, process,(ClientDeleteAllReq const&));
};

