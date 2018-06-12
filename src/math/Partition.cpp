/**
 * Partition.cpp
 *
 * Algorithms by Michael Orlov:
 * https://www.cs.bgu.ac.il/~orlovm/papers
 */

#include <stdexcept>

#include "Partition.h"

namespace spelunker::math {
    partition::iterator::
    iterator(int n, bool first)
            : kappa(n), M(n) {
        if (n <= 0)
            throw std::invalid_argument
                    ("partition::iterator: n must be positive");

        if (!first)
            for (int i = 1; i < n; ++i) {
                kappa[i] = i;
                M[i] = i;
            }

        // integrityCheck();
    }

    partition::iterator &
    partition::iterator::
    operator++() {
        const int n = size();

        for (int i = n - 1; i > 0; --i)
            if (kappa[i] <= M[i - 1]) {
                ++kappa[i];

                const int new_max = std::max(M[i], kappa[i]);
                M[i] = new_max;
                for (int j = i + 1; j < n; ++j) {
                    kappa[j] = 0;
                    M[j] = new_max;
                }

                // integrityCheck();
                return *this;
            }

        throw std::overflow_error("no following partitions");
    }

    partition::iterator &
    partition::iterator::
    operator--() {
        const int n = size();

        for (int i = n - 1; i > 0; --i)
            if (kappa[i] > 0) {
                --kappa[i];

                const int m_i = M[i - 1];
                M[i] = m_i;
                for (int j = i + 1; j < n; ++j) {
                    const int new_max = m_i + j - i;
                    kappa[j] = new_max;
                    M[j] = new_max;
                }

                // integrityCheck();
                return *this;
            }

        throw std::underflow_error("no preceding partitions");
    }

    void
    partition::iterator::
    integrityCheck() {
        const int n = size();
        int max = kappa[0];

        for (int i = 0; i < n; ++i) {
            max = std::max(max, kappa[i]);

            if (max != M[i])
                throw std::domain_error("integrity check failed");
        }
    }

    std::ostream &
    operator<<(std::ostream &out, partition::iterator &it) {
        out << '(';

        if (it->size() > 1)
            std::copy(it->begin(), it->end() - 1,
                      std::ostream_iterator<int>(out, " "));

        out << *(it->end() - 1) << ')';

        return out;
    }


    partition::iterator_k::
    iterator_k(int n, int psize, bool first)
            : iterator(n, first), psize(psize) {
        if (psize <= 0 || psize > n)
            throw std::invalid_argument
                    ("partition::iterator_k: psize must be in [1..n]");

        if (first) {
            const int offset = n - psize;
            for (int i = offset + 1; i < n; ++i) {
                kappa[i] = i - offset;
                M[i] = i - offset;
            }
        } else {
            std::fill(kappa.begin() + psize, kappa.end(), psize - 1);
            std::fill(M.begin() + psize, M.end(), psize - 1);
        }
    }

    partition::iterator_k &
    partition::iterator_k::
    operator++() {
        const int n = size();

        for (int i = n - 1; i > 0; --i)
            if (kappa[i] < psize - 1 && kappa[i] <= M[i - 1]) {
                ++kappa[i];

                const int new_max = std::max(M[i], kappa[i]);
                M[i] = new_max;

                for (int j = i + 1; j <= n - (psize - new_max); ++j) {
                    kappa[j] = 0;
                    M[j] = new_max;
                }

                for (int j = n - (psize - new_max) + 1; j < n; ++j) {
                    const int new_max = psize - (n - j);
                    kappa[j] = new_max;
                    M[j] = new_max;
                }

                // integrityCheck();
                return *this;
            }

        throw std::overflow_error("no following k-partitions");
    }

    partition::iterator_k &
    partition::iterator_k::
    operator--() {
        const int n = size();

        for (int i = n - 1; i > 0; --i)
            if (kappa[i] > 0 && psize - M[i - 1] <= n - i) {
                --kappa[i];

                const int m_i = M[i - 1];
                M[i] = m_i;

                for (int j = i + 1; j < i + (psize - m_i); ++j) {
                    const int new_max = m_i + j - i;
                    kappa[j] = new_max;
                    M[j] = new_max;
                }

                for (int j = i + (psize - m_i); j < n; ++j) {
                    kappa[j] = psize - 1;
                    M[j] = psize - 1;
                }

                // integrityCheck();
                return *this;
            }

        throw std::underflow_error("no preceding k-partitions");
    }

    void
    partition::iterator_k::
    integrityCheck() {
        iterator::integrityCheck();

        if (subsets() != iterator::subsets())
            throw std::domain_error("integrity check 2 failed");
    }
};