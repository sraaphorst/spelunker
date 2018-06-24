/**
 * Travis.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This is because Travis is completely incompetent and missing a number of things.
 */

#pragma once

#include <iostream>

#ifdef TRAVIS
namespace std {
    template<typename T, typename... Args>
    unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif
