/**
 * Homomorphism.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::typeclasses {
    /**
     * A homomorphism is a structure-preserving map.
     * This type class represents a map from one class to another.
     */
    template<typename S, typename T>
    struct Homomorphism {
        //static const T morph(const S&);
        static constexpr bool is_instance = false;
        using src = S;
        using dest = T;
    };

    /**
     * The trivial homomorphism from an object to itself.
     */
    template<typename T>
    struct Homomorphism<T, T> {
        static const T morph(const T &t) {
            return t;
        }

        static constexpr bool is_instance = true;
        using src  = T;
        using dest = T;
    };

    /**
     * A composition homomorphism.
     */
    template<typename S, typename T, typename U,
        typename HST = Homomorphism<S, T>,
        typename HTU = Homomorphism<T, U> >
    struct CompositionHomomorphism : public Homomorphism<S, U> {
        static const U morph(const S &s) {
            return HTU::morph(HST::morph(s));
        }

        static constexpr bool is_instance = true;
        using src  = S;
        using dest = U;
    };
}
