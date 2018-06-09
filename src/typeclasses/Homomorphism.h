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
         // std::T morph(S);
         static constexpr bool is_instance = false;
         using src = S;
         using type = T;
     };
}
