/**
 * Show.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <string>
#include <tuple>

namespace spelunker::typeclasses {
    /**
     * An interface for objects that can show themselves, i.e. create a string representation of themselves.
     */
    template<typename T>
    struct Show {
        // std::string show(T);
        static constexpr bool is_instance = false;
        using type = T;
    };

    template<>
    struct Show<int> {
        static std::string show(const int i) {
            return std::to_string(i);
        }

        static constexpr bool is_instance = true;
        using type = int;
    };

    template<>
    struct Show<std::string> {
        static std::string show(std::string const &s) {
            return s;
        }

        static constexpr bool is_instance = true;
        using type = std::string;
    };

    template<typename A, template<typename, typename> typename Container>
    struct Show<Container<A, std::allocator<A> > > {
        static std::string show(const Container<A, std::allocator<A> > &c) {
            std::string s = "[";
            bool notFirst = false;
            for (auto a : c) {
                if (notFirst)
                    s += ",";
                notFirst = true;
                s += Show<A>::show(a);
            }
            s += "]";
            return s;
        }

        static constexpr bool is_instance = true;
        using type = Container<A, std::allocator<A> >;
    };

    template<typename A, typename B>
    struct Show<std::pair<A, B>> {
        static std::string show(const std::pair<A, B> &p) {
            return '(' + Show<A>::show(p.first) + "," + Show<B>::show(p.second) + ')';
        }

        static constexpr bool is_instance = true;
        using type = std::pair<A, B>;
    };
}
