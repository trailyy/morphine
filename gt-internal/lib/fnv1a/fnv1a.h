#pragma once
#include <stdint.h>

using hash32_t = uint32_t;

namespace hash
{
    static constexpr hash32_t fnv_prime_value = 0x01000193;
    static constexpr hash32_t fnv_offset_basis = 0x811c9dc5;

    // compile time fnv1a algorithm hashing.
    template< typename s >
    struct fnv1a;

    template< >
    struct fnv1a< hash32_t >
    {
        static constexpr hash32_t hash_const(const char* input, hash32_t val = fnv_offset_basis)
        {
            return input[0] == '\0' ? val : hash_const(input + 1, (val ^ *input) * fnv_prime_value);
        }
    };
};// namespace hash

using fnv1a = hash::fnv1a< hash32_t >;

#define HASH( x ) fnv1a::hash_const( x )