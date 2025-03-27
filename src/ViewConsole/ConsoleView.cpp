//
// Created by e.kravchenkova on 25.03.2025.
//

#include "ConsoleView.h"
#include <iostream>

uint64_t ConsoleView::request_id = 0;

Category castCategoryFromString(const std::string& category){
    if (category == "clothes") {
        return Category::clothes;
    } else if (category == "technic") {
        return Category::technic;
    } else if (category == "beauty") {
        return Category::beauty;
    } else {
        return Category::unknown;
    }
}

ConsoleView::ConsoleView() : View() {
    desc.reserve(7);

    po::options_description desc0("General options");
    desc0.add_options()
            ("operation,o", po::value<int>(), "Select operation: \n 1 - registration of new client \n 2 - delete client \n 3 - get all clients info \n 4 - add new item \n 5 - delete item \n 6 - get all items info \n 7 - exit");

    po::options_description desc1("Options for operation 1. Registration of new client");
    desc1.add_options()
            ("name,n", po::value<std::string>(), "name of client")
            ("birthday,b", po::value<std::string>(), "birthday date in format %d-%m-%Y. Ex: 07-05-2004")
            ;
    po::options_description desc2("Options for operation 2. Delete client");
    desc2.add_options()
            ("id", po::value<uint64_t >(), "client id")
            ;
    po::options_description desc3("Options for operation 3. Get all clients info");
    po::options_description desc4("Options for operation 4. Add new item");
    desc4.add_options()
            ("name,n", po::value<std::string>(), "name of item")
            ("desc,d", po::value<std::string>(), "description of item")
            ("count,c", po::value<uint64_t>(), "count of items")
            ("cat", po::value<std::string>(), "category of item; Support value: clothes, technic, beauty")
            ;
    po::options_description desc5("Options for operation 5. Delete item");
    desc5.add_options()
            ("id", po::value<uint64_t >(), "item id")
            ("count, n", po::value<uint64_t >(), "count of items")
            ;
    po::options_description desc6("Options for operation 6. Get all items info");

    desc.push_back(desc0);
    desc.push_back(desc1);
    desc.push_back(desc2);
    desc.push_back(desc3);
    desc.push_back(desc4);
    desc.push_back(desc5);
    desc.push_back(desc6);
}

void ConsoleView::Start() {
    std::string input;
    bool flag = true;

    while (flag){
        std::cout <<"Print command: "<<std::endl;
        std::getline(std::cin, input);

        std::vector<std::string> args = po::split_unix(input);

        try {

        po::variables_map vm;
        po::parsed_options parsed = po::command_line_parser(args).options(desc[0]).allow_unregistered().run();
        po::store(parsed, vm);
        po::notify(vm);

        int operationId = 0;
//            std::cout<<"get operation"<<std::endl;
        operationId = vm["operation"].as<int>();

        switch(operationId)
        {
            case 1:
//                desc[0].add(desc[1]);
//                parsed = po::command_line_parser(args).options(desc[0]).run();
                po::store(po::command_line_parser(args).options(desc[1]).allow_unregistered().run(), vm);
                operation1(vm);
                break;
            case 2:
//                desc[0].add(desc[2]);
//                parsed = po::command_line_parser(args).options(desc[0]).run();
//                po::store(parsed, vm);
                po::store(po::command_line_parser(args).options(desc[2]).allow_unregistered().run(), vm);
                operation2(vm);
                break;
            case 3:
//                desc[0].add(desc[3]);
//                parsed = po::command_line_parser(args).options(desc[0]).run();
//                po::store(parsed, vm);
                po::store(po::command_line_parser(args).options(desc[3]).allow_unregistered().run(), vm);
                operation3(vm);
                break;
            case 4:
//                desc[0].add(desc[4]);
//                parsed = po::command_line_parser(args).options(desc[0]).run();
//                po::store(parsed, vm);
                po::store(po::command_line_parser(args).options(desc[4]).allow_unregistered().run(), vm);
                operation4(vm);
                break;
            case 5:
//                desc[0].add(desc[5]);
//                parsed = po::command_line_parser(args).options(desc[0]).run();
//                po::store(parsed, vm);
                po::store(po::command_line_parser(args).options(desc[5]).allow_unregistered().run(), vm);
                operation5(vm);
                break;
            case 6:
//                desc[0].add(desc[6]);
//                parsed = po::command_line_parser(args).options(desc[0]).run();
//                po::store(parsed, vm);
                po::store(po::command_line_parser(args).options(desc[6]).allow_unregistered().run(), vm);
                operation6(vm);
                break;
            case 7:
                flag = false;
                break;
            default:
                std::cout << desc[0] << "\n";
                break;
        }
        }
        catch (...){
            std::cout<<"After -o need number of operation"<< std::endl;
            std::cout<<desc[0]<<std::endl;
            continue;
        }

    }
}


void ConsoleView::operation1(const po::variables_map & vm) {
    std::string name, birthday;
//    std::cout<<"Зашли сюда "<<name<<birthday<<std::endl;

    try {
        name = vm["name"].as<std::string>();
        birthday = vm["birthday"].as<std::string>();
    }
    catch (...){
        std::cout<<"For operation 1 need options: name, birthday"<< std::endl;
        std::cout<<desc[1]<<std::endl;
        return;
    }
//    std::cout<<"get: "<<name<<birthday<<std::endl;

    auto res = clientService.process(ClientRegistrReq{request_id, name, birthday});
    std::cout<<res<<std::endl;
    request_id++;
}

void ConsoleView::operation2(const po::variables_map & vm) {
    uint64_t id = 0;

    try {
        id = vm["id"].as<uint64_t>();
    }
    catch (...){
        std::cout<<"For operation 2 need options: id" << std::endl;
        std::cout<<desc[2]<<std::endl;
        return;
    }

    auto res = clientService.process(ClientDeleteReq{request_id, id});
    std::cout<<res<<std::endl;
    request_id++;
}

void ConsoleView::operation3(const po::variables_map & vm) {
    auto res = clientService.process(ClientGetALlReq{request_id});
    std::cout<<res<<std::endl;
    request_id++;
}

void ConsoleView::operation4(const po::variables_map & vm) {
    std::string name, description, category;
    uint64_t count;

    try {
        name = vm["name"].as<std::string>();
        description = vm["desc"].as<std::string>();
        category = vm["cat"].as<std::string>();
        count = vm["count"].as<uint64_t>();
    }
    catch (...){
        std::cout<<"For operation 4 need options: name, description, category, count"<< std::endl;
        std::cout<<desc[4]<<std::endl;
        return;
    }

    auto res = itemService.process(AddItemReq{request_id, name, description, count, castCategoryFromString(category)});
    std::cout<<res<<std::endl;
    request_id++;
}

void ConsoleView::operation5(const po::variables_map & vm) {
    uint64_t id = 0;
    uint64_t count = 0;

    try {
        id = vm["id"].as<uint64_t>();
        count = vm["count"].as<uint64_t>();
    }
    catch (...){
        std::cout<<"For operation 5 need options: id"<< std::endl;
        std::cout<<desc[5]<<std::endl;
        return;
    }

    auto res = itemService.process(DeleteItemReq{request_id, id, count});
    std::cout<<res<<std::endl;
    request_id++;
}

void ConsoleView::operation6(const po::variables_map & vm) {
    auto res = itemService.process(GetAllItemReq{request_id});
    std::cout<<res<<std::endl;
    request_id++;
}