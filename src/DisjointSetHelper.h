/**
 * DisjointSetHelper.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Modified by code distributed under the MIT License and written by:
 * \author Alejandro Mallea <janoma@gmail.com>
 */

#ifndef SPELUNKER_DISJOINTSETHELPER_H
#define SPELUNKER_DISJOINTSETHELPER_H

#include <vector>
#include <boost/pending/property.hpp>

            /// Element of a set of disjoint sets, represented by an int. Do a lookup into a vector if necessary.
            struct Element {
            public:
                explicit Element(int id) : dsID(id) {};

                size_t dsID;
                size_t dsRank;
                size_t dsParent;
            };

            inline bool
            operator==(Element const& lhs, Element const& rhs)
            {
                return lhs.dsID == rhs.dsID;
            }

            inline bool
            operator!=(Element const& lhs, Element const& rhs)
            {
                return ! operator==(lhs, rhs);
            }

            class Parent
            {
            public:
                Parent(std::vector<Element>& e) : mElements(e) { }
                std::vector<Element>& mElements;
            };

            class Rank
            {
            public:
                Rank(std::vector<Element>& e) : mElements(e) { }
                std::vector<Element>& mElements;
            };

            template <>
            struct boost::property_traits<Rank*>
            {
                typedef size_t value_type;
            };

            inline Element const&
            get(Parent* pa, Element const& k)
            {
                return pa->mElements.at(k.dsParent);
            }

            inline void
            put(Parent* pa, Element k, Element const& val)
            {
                pa->mElements.at(k.dsID).dsParent = val.dsID;
            }

            inline size_t const&
            get(Rank*, Element const& k)
            {
                return k.dsRank;
            }

            inline void
            put(Rank* pa, Element k, size_t const& val)
            {
                pa->mElements.at(k.dsID).dsRank = val;
            }

            inline bool
            compareByParent(Element const& lhs, Element const& rhs)
            {
                return lhs.dsParent < rhs.dsParent;
            }

            inline bool
            compareByID(Element const& lhs, Element const& rhs)
            {
                return lhs.dsID < rhs.dsID;
            }
#endif //SPELUNKER_DISJOINTSETHELPER_H
