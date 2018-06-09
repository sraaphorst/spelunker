/**
 * DisjointSetHelper.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Modified by code distributed under the MIT License and written by:
 * \author Alejandro Mallea <janoma@gmail.com>
 */

#pragma once

#include <vector>
#include <boost/pending/property.hpp>

namespace spelunker::types {
    /// Element of a set of disjoint sets, represented by an int. Do a lookup into a vector if necessary.
    struct Element final {
    public:
        explicit Element(int n)
                : mSomeInt(n), dsID(0), dsRank(0), dsParent(0) {};

        int someInt() const { return mSomeInt; }

        size_t dsID;
        size_t dsRank;
        size_t dsParent;

    private:
        int mSomeInt;
    };

    inline bool
    operator==(Element const &lhs, Element const &rhs) {
        return lhs.someInt() == rhs.someInt();
    }

    inline bool
    operator!=(Element const &lhs, Element const &rhs) {
        return !operator==(lhs, rhs);
    }

    class Parent final {
    public:
        explicit Parent(std::vector<Element> &e) : mElements(e) {}

        std::vector<Element> &mElements;
    };

    class Rank final {
    public:
        explicit Rank(std::vector<Element> &e) : mElements(e) {}

        std::vector<Element> &mElements;
    };
}

// This is an annoying workaround to a GCC problem:
// https://stackoverflow.com/questions/25594644/warning-specialization-of-template-in-different-namespace
namespace boost {
    template<>
    struct property_traits<spelunker::types::Rank *> {
        typedef size_t value_type;
    };
}

namespace spelunker::types {
    inline Element const &
    get(Parent *pa, Element const &k) {
        return pa->mElements.at(k.dsParent);
    }

    inline void
    put(Parent *pa, Element k, Element const &val) {
        pa->mElements.at(k.dsID).dsParent = val.dsID;
    }

    inline size_t const &
    get(Rank *, Element const &k) {
        return k.dsRank;
    }

    inline void
    put(Rank *pa, Element k, size_t const &val) {
        pa->mElements.at(k.dsID).dsRank = val;
    }

    inline bool
    compareByParent(Element const &lhs, Element const &rhs) {
        return lhs.dsParent < rhs.dsParent;
    }

    inline bool
    compareBySomeInt(Element const &lhs, Element const &rhs) {
        return lhs.someInt() < rhs.someInt();
    }
}
