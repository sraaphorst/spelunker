/**
 * Partition.h
 *
 * Generate all the partitions of an n-set.
 *
 * Algorithms by Michael Orlov:
 * https://www.cs.bgu.ac.il/~orlovm/papers
 */

#pragma once

#include <iterator>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

namespace spelunker::math {
    class partition {
    public:
        class iterator
                : public std::iterator<std::bidirectional_iterator_tag,
                        const std::vector<int> > {
        public:
            iterator(int n, bool first = true);

            reference operator*() const { return kappa; }

            pointer operator->() const { return &kappa; }

            int size() const { return kappa.size(); }

            int subsets() const { return M[size() - 1] + 1; }

            iterator &operator++();

            iterator &operator--();

            template<typename Elem>
            std::unique_ptr<std::vector<std::vector<Elem> > >
            operator[](const std::vector<Elem> &v) const;

        protected:
            std::vector<int> kappa, M;

            void integrityCheck();
        };

        class iterator_k
                : public iterator {
        public:
            iterator_k(int n, int psize, bool first = true);

            // optimized version
            int subsets() const { return psize; }

            iterator_k &operator++();

            iterator_k &operator--();

        private:
            const int psize;

            void integrityCheck();
        };
    };

    extern std::ostream &operator<<(std::ostream &out,
                                    partition::iterator &it);

    template<typename Elem>
    std::unique_ptr<std::vector<std::vector<Elem> > >
    partition::iterator::
    operator[](const std::vector<Elem> &v)
    const {
        auto part = std::make_unique<std::vector<std::vector<Elem>>>(subsets());
        for (auto i = 0; i < size(); ++i)
            (*part)[kappa[i]].push_back(v[i]);
        return part;
    }

    template<typename Elem>
    std::ostream &
    operator<<(std::ostream &out, std::vector<Elem> &s) {
        out << '(';

        for (int i = 0; i < s.size() - 1; ++i) {
            out << s[i] << ' ';
        }
        out << *(s.end() - 1) << ')';

        return out;
    }

    template<typename Elem>
    std::ostream &
    operator<<(std::ostream &out,
               std::vector<std::vector<Elem> > &part) {
        out << '(';

        for (int i = 0; i < part.size() - 1; ++i) {
            out << part[i] << ' ';
        }
        out << *(part.end() - 1) << ')';

        return out;
    }
};
