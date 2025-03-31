//
// Created by e.kravchenkova on 27.03.2025.
//

#include "HttpServer.h"
#include <boost/asio/thread_pool.hpp>
#include <iostream>
#include <nlohmann/json.hpp>


HttpServer::HttpServer(int threadCount) : threadCount(threadCount),tp(threadCount),io_context(threadCount), work_guard(boost::asio::make_work_guard(io_context)) {}
HttpServer::HttpServer(Controller controller, int threadCount) : threadCount(threadCount),tp(threadCount),io_context(threadCount), work_guard(boost::asio::make_work_guard(io_context)),controller(controller) {}

void HttpServer::stop() {
    work_guard.reset();
    io_context.stop();
    //tp.join();
    tp.stop();
}

void HttpServer::run(const boost::asio::ip::tcp& address, uint16_t port){

    boost::system::error_code ec;

    for (int i = 0; i < threadCount; ++i) {
        boost::asio::post(tp, [&io_context = this ->io_context](){
            //boost::asio::io_service::work w(io_context); // нужен для того, чтобы .run() не завершался когда очередь задач пуста.
            io_context.run();
        });
    }

    boost::asio::ip::tcp::acceptor acceptor(io_context); // используется для приема новых подключений через сокеты.
    boost::asio::ip::tcp::endpoint endpoint(address, port);

    acceptor.open(endpoint.protocol(),ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true),ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    acceptor.bind(endpoint,ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    acceptor.listen(boost::asio::socket_base::max_listen_connections,ec);
    if (ec) {
        std::cerr << ec.message() << "\n";
    }

    for(int i = 0; i < threadCount; ++i) {
        auto sock = std::make_shared<boost::asio::ip::tcp::socket>(io_context);
        acceptor.async_accept(*sock, std::bind(&HttpServer::onAcceptAsync, this, std::ref(acceptor), std::ref(io_context), sock, std::placeholders::_1));
    }

    tp.join();
    //tp.stop();
}

void HttpServer::doAccept(ip::tcp::acceptor &acceptor) {
    for(int i = 0; i < threadCount; ++i) {
        auto sock = std::make_shared<ip::tcp::socket>(io_context);
        acceptor.async_accept(*sock, std::bind(&HttpServer::onAcceptAsync, this, std::ref(acceptor), std::ref(io_context), sock, std::placeholders::_1));
    }
}

void HttpServer::onAcceptAsync(ip::tcp::acceptor &acceptor, boost::asio::io_service &io_context,
                               std::shared_ptr<ip::tcp::socket> sock, const boost::system::error_code &ec) {
    if (ec) {
        if (ec == boost::asio::error::operation_aborted) {
            return;
        }

        std::cerr << ec.message() << std::endl;
        return;
    }

    auto buf = std::make_shared<boost::beast::flat_buffer>();
    auto req = std::make_shared<http::request<http::string_body>>();
    http::async_read(*sock, *buf, *req, std::bind(&HttpServer::onReadAsync,this, sock, buf, req, std::placeholders::_1, std::placeholders::_2));

    auto acceptSock = std::make_shared<ip::tcp::socket>(io_context);
    acceptor.async_accept(*acceptSock, std::bind(&HttpServer::onAcceptAsync,this, std::ref(acceptor), std::ref(io_context), acceptSock, std::placeholders::_1));
}

void HttpServer::onReadAsync(std::shared_ptr<ip::tcp::socket> sock, std::shared_ptr<boost::beast::flat_buffer> buf,
                             std::shared_ptr<http::request<http::string_body>> req, boost::beast::error_code ec,
                             std::size_t bytes_transferred) {

    if (ec) {
        if (ec == boost::asio::error::operation_aborted) {
            return;
        }
        if (ec == http::error::end_of_stream) {
            std::cout << "client socket shutdown" << std::endl;
            return;
        }
        std::cerr << ec.message() << std::endl; // will be http::error::partial_message
        return;
    }

//    auto resp = std::make_shared<http::response<http::string_body>>();
//    resp->result(http::status::ok);
//    resp->version(11);
//    resp->set(http::field::server, BOOST_BEAST_VERSION_STRING);
//    resp->set("X-test-key", "my-test-key");
//    resp->body() = req->body();
    auto resp = std::make_shared<http::response<http::string_body>>(controller.handleRequest(req));
    //resp->prepare_payload();
    http::async_write(*sock, *resp, std::bind(&HttpServer::onWriteAsync,this, sock, resp, req->keep_alive(), std::placeholders::_1, std::placeholders::_2));
}

void
HttpServer::onWriteAsync(std::shared_ptr<ip::tcp::socket> sock, std::shared_ptr<http::response<http::string_body>> req,
                         bool keepAlive, boost::beast::error_code ec, std::size_t bytes_transferred) {

    if (ec) {
        if (ec == boost::asio::error::operation_aborted) {
            return;
        }
        std::cerr << ec.message() << std::endl;
        return;
    }

    if (keepAlive) {
        auto buf = std::make_shared<boost::beast::flat_buffer>();
        auto req = std::make_shared<http::request<http::string_body>>();
        http::async_read(*sock, *buf, *req, std::bind(&HttpServer::onReadAsync,this, sock, buf, req, std::placeholders::_1, std::placeholders::_2));
    } else {
        sock->shutdown(ip::tcp::socket::shutdown_both);
    }
}

http::response<http::string_body> HttpServer::getResponce(std::shared_ptr<http::request<http::string_body>> const& req) {
    if (req->method() == http::verb::get && req->target() == "/api/data") {
        // Обработка GET запроса
        nlohmann::json json_response = {{"message", "Это GET запрос"}};
        http::response<http::string_body> res{http::status::ok, req->version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req->keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    } else if (req->method() == http::verb::post && req->target() == "/api/data") {
        // Обработка POST запроса
        auto json_request = nlohmann::json::parse(req->body());
        std::string response_message = "Получено: " + json_request.dump();
        nlohmann::json json_response = {{"message", response_message}};
        http::response<http::string_body> res{http::status::ok, req->version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req->keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    } else if (req->method() == http::verb::put && req->target() == "/api/data") {
        // Обработка PUT запроса
        auto json_request = nlohmann::json::parse(req->body());
        std::string response_message = "Updated: " + json_request.dump();
        nlohmann::json json_response = {{"message", response_message}};
        http::response<http::string_body> res{http::status::ok, req->version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req->keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    } else if (req->method() == http::verb::delete_ && req->target() == "/api/data") {
        // Обработка DELETE запроса
        nlohmann::json json_response = {{"message", "Ресурс удален"}};
        http::response<http::string_body> res{http::status::ok, req->version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.keep_alive(req->keep_alive());
        res.body() = json_response.dump();
        res.prepare_payload();
        return res;
    }

    // Базовый ответ для остальных методов
    auto res = http::response<http::string_body>{http::status::bad_request, req->version()};
    //res.set(http::field::connection,"close");
    res.body() = "Некорректный путь к запросу";
    res.prepare_payload();
    //res.keep_alive(false);
    return res;
}