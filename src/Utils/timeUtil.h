//
// Created by e.kravchenkova on 24.03.2025.
//

#pragma once

#include <iomanip>
#include <ctime>
#include <sstream>

std::pair<bool,time_t> stringToTime(const std::string& dateTime,const char* timeFormat);

std::string timeToString(time_t time,const char* timeFormat);