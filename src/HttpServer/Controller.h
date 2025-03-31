//
// Created by e.kravchenkova on 29.03.2025.
//

#pragma once
#include "../Service/ClientService.h"
#include "../Service/ItemService.h"
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;

class Controller {
    std::shared_ptr<IClientService> clientService = std::make_shared<ClientService>();
    std::shared_ptr<IItemService> itemService = std::make_shared<ItemService>();

public:
    Controller(const std::shared_ptr<IClientService> &clientService,
               const std::shared_ptr<IItemService> &itemService);
    Controller() = default;

    http::response<http::string_body> handleRequest(
            std::shared_ptr<http::request<http::string_body>> const& req);

private:
    http::response<http::string_body> getCallback(std::shared_ptr<http::request<http::string_body>> const& req, const std::string& message);
    http::response<http::string_body> getAllClients(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> getAllItems(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> addClient(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> addItem(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> deleteClient(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> deleteItem(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> modifyClient(std::shared_ptr<http::request<http::string_body>> const& req);
    http::response<http::string_body> modifyItem(std::shared_ptr<http::request<http::string_body>> const& req);

};

