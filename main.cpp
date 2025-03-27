//#include <boost/asio.hpp>

#include <locale>

#include "src/ViewConsole/ConsoleView.h"
#include <boost/asio.hpp>
#include <iostream>

int main() {
    std::setlocale(LC_ALL, "ru_RU.UTF-8");
    std::uint16_t port = 1500;

    boost::asio::io_context io_context;
    boost::asio::ip::udp::endpoint receiver(boost::asio::ip::udp::v4(), port);
    boost::asio::ip::udp::socket socket(io_context, receiver);

    while (true) {
        char buffer[65536];
        boost::asio::ip::udp::endpoint sender;
        std::size_t bytes_transferred =
                socket.receive_from(boost::asio::buffer(buffer), sender);
        std::cout<<"recieve and send: "<<"bytes count: "<<bytes_transferred<<" value: "<<buffer<<std::endl;
        socket.send_to(boost::asio::buffer(buffer, bytes_transferred), sender);
    }

    return 0;
}

//int main() {
//
//    ConsoleView view = ConsoleView();
//    view.Start();
//
//
//
////    ClientService service = ClientService();
////    auto res = service.process({.requestId=0,.nickname="Liza",.birthday="07-05-2004"});
////    std::cout<<res<<std::endl;
////    res = service.process({.requestId=1,.nickname="Makar",.birthday="04-07-2004"});
////    std::cout<<res<<std::endl;
////    auto res2 = service.process(ClientGetALlReq{.requestId=0});
////    std::cout<<res2<<std::endl;
////    auto res3 = service.process({.requestId=1,.clientId = 1});
////    std::cout<<res3<<std::endl;
////    res3 = service.process({.requestId=1,.clientId = 1});
////    std::cout<<res3<<std::endl;
////    res2 = service.process(ClientGetALlReq{.requestId=0});
////    std::cout<<res2<<std::endl;
////
////    ItemService itemService = ItemService();
////    auto req4 = itemService.process({.requestId = 0, .name = "dress", .description="my", .count = 2, .category = Category::clothes});
////
////
//
//    return 0;
//}