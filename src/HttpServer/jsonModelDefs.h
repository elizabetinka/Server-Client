//
// Created by e.kravchenkova on 29.03.2025.
//

#pragma once
#include <nlohmann/json.hpp>
#include "../Service/Defs/ClientDefs.h"
#include "../Service/Defs/ItemDefs.h"

//using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientDto, id, nickname, birthday)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientGetALlReq, requestId)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientGetALlRes, responceId, clients)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientRegistrReq, requestId, nickname, birthday)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientRegistrRes, responceId, success)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientDeleteReq, requestId, clientId)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientDeleteRes, responceId, success)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientModifyReq, requestId, new_client)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClientModifyRes, responceId, success)


NLOHMANN_JSON_SERIALIZE_ENUM( Category, {
    {unknown, nullptr},
    {beauty, "beauty"},
    {technic, "technic"},
    {clothes, "clothes"},
})
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Item, id, name, description,category)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ItemInfo, item, count)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddItemReq, requestId,name,description,count, category)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AddItemRes, responceId, success)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeleteItemReq, requestId,itemId,count)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(DeleteItemRes, responceId, success, item, bought_count)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetAllItemReq, requestId)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetAllItemRes, responceId, items)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ItemModifyReq, requestId, new_item)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ItemModifyRes, responceId, success)

//void to_json(json& j, const ClientDto& p) {
//    j = json{{"id", p.id}, {"nickname", p.nickname}, {"birthday", p.birthday}};
//}
//
//void from_json(const json& j, ClientDto& p) {
//    j.at("id").get_to(p.id);
//    j.at("nickname").get_to(p.nickname);
//    j.at("birthday").get_to(p.birthday);
//}

//void to_json(json& j, const ClientGetALlRes& p) {
//    j = json{{"responce_id", p.responceId}, {"clients", p.clients}};
//}
//
//void from_json(const json& j, ClientGetALlRes& p) {
//    j.at("responce_id").get_to(p.responceId);
//    j.at("clients").get_to(p.clients);
//    p.clients = j.at("clients").get<std::vector<ClientDto>>();
//
//    j["clients"].get_to<std::vector<ClientDto>>();
//    get_to(p.clients);
//}
