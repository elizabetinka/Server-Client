//
// Created by e.kravchenkova on 25.03.2025.
//

#pragma once

#include "View.h"
#include "../Service/ClientService.h"
#include "../Service/ItemService.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class ConsoleView : View {
public:
    ConsoleView();
    void Start() override;
private:
    static uint64_t request_id;
    std::vector<po::options_description> desc;
//    int operationId{};
    ItemService itemService;
    ClientService clientService;

    void operation1(const po::variables_map&);
    void operation2(const po::variables_map&);
    void operation3(const po::variables_map&);
    void operation4(const po::variables_map&);
    void operation5(const po::variables_map&);
    void operation6(const po::variables_map&);
};



