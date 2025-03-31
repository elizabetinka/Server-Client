//
// Created by e.kravchenkova on 29.03.2025.
//

#include "Controller.h"
#include "jsonModelDefs.h"


http::response<http::string_body> Controller::handleRequest(
        std::shared_ptr<http::request<http::string_body>> const& req) {
    if (req->method() == http::verb::get) {
        if (req->target() == "/api/client") {
            return getAllClients(req);
        } else if (req->target() == "/api/item") {
            return getAllItems(req);
        }
    } else if (req->method() == http::verb::post) {
        if (req->target() == "/api/client") {
            return addClient(req);
        } else if (req->target() == "/api/item") {
            return addItem(req);
        }
    } else if (req->method() == http::verb::delete_) {
        if (req->target() == "/api/client") {
            return deleteClient(req);
        } else if (req->target() == "/api/item") {
            return deleteItem(req);
        }
    }
    return getCallback(req, "Неизвестный путь");
}

        // Обработка GET запроса
//        nlohmann::json json_response = {{"message", "Это GET запрос"}};
//        http::response<http::string_body> res{http::status::ok, req->version()};
//        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//        res.set(http::field::content_type, "application/json");
//        res.keep_alive(req->keep_alive());
//        res.body() = json_response.dump();
//        res.prepare_payload();
        //return res;
//    else if (req->method() == http::verb::post && req->target() == "/api/data") {
//        // Обработка POST запроса
//        auto json_request = nlohmann::json::parse(req->body());
//        std::string response_message = "Получено: " + json_request.dump();
//        nlohmann::json json_response = {{"message", response_message}};
//        http::response<http::string_body> res{http::status::ok, req->version()};
//        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//        res.set(http::field::content_type, "application/json");
//        res.keep_alive(req->keep_alive());
//        res.body() = json_response.dump();
//        res.prepare_payload();
//        return res;
//    } else if (req->method() == http::verb::put && req->target() == "/api/data") {
//        // Обработка PUT запроса
//        auto json_request = nlohmann::json::parse(req->body());
//        std::string response_message = "Updated: " + json_request.dump();
//        nlohmann::json json_response = {{"message", response_message}};
//        http::response<http::string_body> res{http::status::ok, req->version()};
//        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//        res.set(http::field::content_type, "application/json");
//        res.keep_alive(req->keep_alive());
//        res.body() = json_response.dump();
//        res.prepare_payload();
//        return res;
//    } else if (req->method() == http::verb::delete_ && req->target() == "/api/data") {
//        // Обработка DELETE запроса
//        nlohmann::json json_response = {{"message", "Ресурс удален"}};
//        http::response<http::string_body> res{http::status::ok, req->version()};
//        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
//        res.set(http::field::content_type, "application/json");
//        res.keep_alive(req->keep_alive());
//        res.body() = json_response.dump();
//        res.prepare_payload();
//        return res;
//    }
//
//    // Базовый ответ для остальных методов
//    auto res = http::response<http::string_body>{http::status::bad_request, req->version()};
//    //res.set(http::field::connection,"close");
//    res.body() = "Некорректный путь к запросу";
//    res.prepare_payload();
//    //res.keep_alive(false);
//    return res;
//}

http::response<http::string_body> Controller::getAllClients(std::shared_ptr<http::request<http::string_body>> const& req){
    ClientGetALlReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<ClientGetALlReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = clientService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Controller::getAllItems(const std::shared_ptr<http::request<http::string_body>> &req) {
    GetAllItemReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<GetAllItemReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = itemService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
    }


http::response<http::string_body> Controller::addClient(const std::shared_ptr<http::request<http::string_body>> &req) {
    ClientRegistrReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<ClientRegistrReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = clientService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Controller::getCallback(const std::shared_ptr<http::request<http::string_body>> &req, const std::string &message) {
    auto res = http::response<http::string_body>{http::status::bad_request, req->version()};
    res.body() = message;
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Controller::addItem(const std::shared_ptr<http::request<http::string_body>> &req) {
    AddItemReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<AddItemReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = itemService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body>
Controller::deleteClient(const std::shared_ptr<http::request<http::string_body>> &req) {
    ClientDeleteReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<ClientDeleteReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = clientService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Controller::deleteItem(const std::shared_ptr<http::request<http::string_body>> &req) {
    DeleteItemReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<DeleteItemReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = itemService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body>
Controller::modifyClient(const std::shared_ptr<http::request<http::string_body>> &req) {
    ClientModifyReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<ClientModifyReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = clientService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

http::response<http::string_body> Controller::modifyItem(const std::shared_ptr<http::request<http::string_body>> &req) {
    ItemModifyReq request;
    try{
        auto json_request = nlohmann::json::parse(req->body());
        request = json_request.template get<ItemModifyReq>();
    }
    catch (...){
        return getCallback(req,"Не удалось распознать входящий json");
    }

    auto responce = itemService->process(request);
    nlohmann::json json_response(responce);
    http::response<http::string_body> res{http::status::ok, req->version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.keep_alive(req->keep_alive());
    res.body() = json_response.dump();
    res.prepare_payload();
    return res;
}

Controller::Controller(const std::shared_ptr<IClientService> &clientService2,
                       const std::shared_ptr<IItemService> &itemService2) : clientService(std::move(clientService2)),
                                                                                itemService(std::move(itemService2)) {}
