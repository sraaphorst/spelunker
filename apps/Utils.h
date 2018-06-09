/**
 * Utils.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cstdlib>
#include <functional>

class Utils final {
private:
    Utils() = default;

public:
    template<typename P>
    static P parse(std::function<P(const char*, char**)> parser, const char *str) {
        char *temp = nullptr;
        P val = parser(str, &temp);

        if (temp == nullptr || *temp != '\0')
            return -1;

        return val;
    }

    static long parseLong(const char *str) {
        return parse<long>([](const char *s, char **temp) { return std::strtol(s, temp, 0); }, str);
    }

    static double parseDouble(const char *str) {
        return parse<double>([](const char *s, char **temp) { return std::strtold(s, temp); }, str);
    }
};
