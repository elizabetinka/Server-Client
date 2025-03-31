//
// Created by e.kravchenkova on 30.03.2025.
//

#include <gtest/gtest.h>
#include "../../src/Service/ClientService.h"
#include "../mock/MockClientService.h"
#include "../mock/MockItemService.h"
#include "../../src/HttpServer/HttpServer.h"
#include "../../src/HttpServer/jsonModelDefs.h"
#include <gmock/gmock.h>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::A;

// Тестовый клиент для отправки HTTP-запросов
std::string send_request(const std::string& request, const ip::tcp address, unsigned short port) {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve("127.0.0.1", std::to_string(port));

    boost::asio::connect(socket, endpoints);

    boost::asio::write(socket, boost::asio::buffer(request));

    boost::asio::streambuf response;
    boost::asio::read_until(socket, response, "\r\n\r\n");

    return boost::asio::buffer_cast<const char*>(response.data());
}

class HttpServerTests : public testing::Test {

protected:
    std::shared_ptr<MockClientService> clientService = std::make_shared<MockClientService>();
    std::shared_ptr<MockItemService> itemService = std::make_shared<MockItemService>();
    Controller controller{clientService, itemService};

    const ip::tcp address = ip::tcp::v4();
    unsigned short port = 8080;
    HttpServer server{controller,4};
    std::thread server_thread{[this] () {server.run(address,port);}};

public:
    virtual ~HttpServerTests() override {
        server.stop();
        server_thread.join();
    }

};

TEST_F(HttpServerTests, HandleGetClientsRequest) {

    ClientGetALlRes res(0,{});
    EXPECT_CALL(*clientService, process(A<const ClientGetALlReq&>())).WillOnce(Return(res));

    nlohmann::json request(ClientGetALlReq{.requestId = 0});
    std::string jsonBody = request.dump();

    std::string request_str =
            "GET /api/client HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(jsonBody.size()) + "\r\n"
                                                                   "\r\n" +
            jsonBody;

    std::string response = send_request(request_str, address, port);
    EXPECT_NE(response.find("200 OK"), std::string::npos);
}

TEST_F(HttpServerTests, HandleAddClientsRequest) {

    ClientRegistrRes res(0,true);
    EXPECT_CALL(*clientService, process(A<const ClientRegistrReq&>())).WillOnce(Return(res));

    nlohmann::json request(ClientRegistrReq{.requestId = 0, .nickname="LIZA", .birthday="07-05-2004"});
    std::string jsonBody = request.dump();

    std::string request_str =
            "POST /api/client HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(jsonBody.size()) + "\r\n"
                                                                   "\r\n" +
            jsonBody;

    std::string response = send_request(request_str, address, port);
    EXPECT_NE(response.find("200 OK"), std::string::npos);
}