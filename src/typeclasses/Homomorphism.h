/**
 * Homomorphism.h
 *
 * By Sebastian Raaphorst, 2018.
 */

 // This looks to be what I want:
 // http://coliru.stacked-crooked.com/a/715420454b817658
 // or this:
 // http://coliru.stacked-crooked.com/a/2ebd2109f7dfe365

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

    template<typename S, typename T>
    struct Monomorphism : Homomorphism<S,T> {
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
     * Composable homomorphisms.
     */
    template<typename, typename>
    struct CompositionMorphism;

    template<typename S, typename T, typename U>
    struct CompositionMorphism<Homomorphism<S,T>, Homomorphism<T,U>> {
        static const U morph(const S &s) {
            return Homomorphism<T,U>::morph(Homomorphism<S,T>::morph(s));
        }

        static constexpr bool is_instance = true;
        using src  = S;
        using dest = U;
    };


    template<typename S, typename T, typename U>
    struct CompositionMorphism<Monomorphism<S,T>, Monomorphism<T,U>> {
        static const U morph(const S &s) {
            return Monomorphism<T,U>::morph(Monomorphism<S,T>::morph(s));
        }

        static constexpr bool is_instance = true;
        using src  = S;
        using dest = U;
    };
}