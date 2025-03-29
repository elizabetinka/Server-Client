//
// Created by e.kravchenkova on 27.03.2025.
//

#pragma once


#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include "Controller.h"


namespace ip = boost::asio::ip;
namespace http = boost::beast::http;

class HttpServer {

    boost::asio::io_service io_context;
    int threadCount = 4;
    boost::asio::thread_pool tp;
    Controller controller;

public:
    HttpServer(int threadCount=4);

    void run(const boost::asio::ip::tcp& address, uint16_t port);
private:

    http::response<http::string_body> getResponce(std::shared_ptr<http::request<http::string_body>> const& req);

    void doAccept(ip::tcp::acceptor& acceptor);

    void onAcceptAsync(
            ip::tcp::acceptor& acceptor,
            boost::asio::io_service& io_context,
            std::shared_ptr<ip::tcp::socket> sock,
            const boost::system::error_code& ec);

    void onReadAsync(
            std::shared_ptr<ip::tcp::socket> sock,
            std::shared_ptr<boost::beast::flat_buffer> buf,
            std::shared_ptr<http::request<http::string_body>> req,
            boost::beast::error_code ec,
            std::size_t bytes_transferred);

    void onWriteAsync(
            std::shared_ptr<ip::tcp::socket> sock,
            std::shared_ptr<http::response<http::string_body>> req,
            bool keepAlive,
            boost::beast::error_code ec,
            std::size_t bytes_transferred);
};

