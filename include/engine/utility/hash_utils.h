// hash_utils.h
#ifndef INCLUDED_hash_utils
#define INCLUDED_hash_utils

#include <string>

#include "engine/intdef.h"

/**
 * chash is a compile-time function that computes the hash code for the given
 * string.
 *
 * This macro is defined for convenience but also to provide hashing
 * flexibility for os dependent hash functions. For example, the CRC function
 * although incredibly fast is not available on all systems so this provides
 * a wrapper in which such checks could be performed. For the the time being,
 * however, only the fnv-1a hashing function is utilized.
 */
#define chash( string ) \
nge::util::HashUtils::compileTimeHash<sizeof( string ) - 1>( string )

namespace nge
{

namespace util
{

struct HashUtils
{
    /**
     * Defines the initial offset used by the FNV-1A hashing function.
     * From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a
     */
    static constexpr uint32 FNV_OFFSET_32 = 2166136261;

    /**
     * Defines the prime number used by the FNV-1A hashing function.
     * From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a
     */
    static constexpr uint32 FNV_PRIME_32 = 16777619;

    /**
     * Defines the initial offset used by the FNV-1A hashing function.
     * From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a
     */
    static constexpr uint64 FNV_OFFSET_64 = 14695981039346656037ULL;

    /**
     * Defines the prime number used by the FNV-1A hashing function.
     * From: http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-1a
     */
    static constexpr uint64 FNV_PRIME_64 = 1099511628211ULL;

    /**
     * Computes the fnv1a hash of a string value.
     */
    static uint32 fnv1a( const std::string& value );

    /**
     * Compiles the fnv hash code at compile time.
     */
    template <uint32 length>
    static constexpr uint32 compileTimeHash( const char* value );
};

template <>
inline
constexpr uint32 HashUtils::compileTimeHash<uint32( 0 )>( const char* value )
{
    return FNV_OFFSET_32;
}

template <uint32 rem>
inline
constexpr uint32 HashUtils::compileTimeHash( const char* value )
{
    return ( compileTimeHash<rem - 1>( value ) ^ value[rem - 1] ) *
        FNV_PRIME_32;
}

} // End nspc util

} // End nspc nge

#endif
