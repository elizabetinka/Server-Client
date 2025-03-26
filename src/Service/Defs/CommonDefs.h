//
// Created by e.kravchenkova on 23.03.2025.
//

#pragma once
#include <string>

struct Response
{
    virtual ~Response() = default;
    virtual std::string getResultStr() const = 0;
};

inline bool isEmptyOrOnlySpaceString(const std::string& str){
    return str.empty() || (str.find_first_not_of(' ') == std::string::npos);
}