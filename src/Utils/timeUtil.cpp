//
// Created by e.kravchenkova on 25.03.2025.
//

#include "timeUtil.h"

std::pair<bool,time_t> stringToTime(const std::string& dateTime,const char* timeFormat) {
    std::tm tm = {};
    std::istringstream ss(dateTime);

    // Парсинг строки в формате "гггг-мм-дд"
    // ss >> std::get_time(&tm, "%Y-%m-%d");
    ss >> std::get_time(&tm, timeFormat);
    if (ss.fail()) {
        //throw std::runtime_error("Ошибка при парсинге даты и времени");
        return std::make_pair(false, time(nullptr));
    }

    // Преобразование tm в time_t (локальное время)
    return std::make_pair(true, std::mktime(&tm));
}

std::string timeToString(time_t time,const char* timeFormat) {
    // Конвертируем time_t в структуру tm
    struct tm *timeinfo = localtime(&time);

    // Буфер для строки
    char buffer[80];

    // Преобразуем в строку в нужном формате (например, "YYYY-MM-DD HH:MM:SS")
    strftime(buffer, sizeof(buffer), timeFormat, timeinfo);

    return std::string(buffer);
}

