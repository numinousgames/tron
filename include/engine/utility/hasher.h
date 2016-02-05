// hasher.h
//
// Defines default hash function implementations. To add new default hash
// implementations specialize hasher.
//
#ifndef NGE_UTIL_HASHER_H
#define NGE_UTIL_HASHER_H

#include <string>

#include "engine/intdef.h"
#include "engine/utility/hash_utils.h"

namespace nge
{

namespace util
{

template <typename T>
struct Hasher
{
    /**
     * Computes the hash for a give value.
     */
    static uint32 hash( const T& value );
};

template <typename T>
inline
uint32 Hasher<T>::hash( const T& value )
{
    return 0;
}

template <>
inline
uint32 Hasher<std::string>::hash( const std::string& value )
{
    return HashUtils::fnv1a( value );
}

template <>
inline
uint32 Hasher<uint8>::hash( const uint8& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ static_cast<uint32>( value ) ) *
           HashUtils::FNV_PRIME_32;
}

template <>
inline
uint32 Hasher<int8>::hash( const int8& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ static_cast<uint32>( value ) ) *
           HashUtils::FNV_PRIME_32;
}

template <>
inline
uint32 Hasher<uint16>::hash( const uint16& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ static_cast<uint32>( value ) ) *
           HashUtils::FNV_PRIME_32;
}

template <>
inline
uint32 Hasher<int16>::hash( const int16& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ static_cast<uint32>( value ) ) *
           HashUtils::FNV_PRIME_32;
}

template <>
inline
uint32 Hasher<uint32>::hash( const uint32& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ value ) * HashUtils::FNV_PRIME_32;
}

template <>
inline
uint32 Hasher<int32>::hash( const int32& value )
{
    return ( HashUtils::FNV_OFFSET_32 ^ static_cast<uint32>( value ) ) *
        HashUtils::FNV_PRIME_32;
}

template <>
inline
uint32 Hasher<uint64>::hash( const uint64& value )
{
    uint64 hashCode = ( HashUtils::FNV_OFFSET_64 ^ value ) *
        HashUtils::FNV_PRIME_64;

    return static_cast<uint32>(
        ( static_cast<uint64>( 0xFFFFFFFF ) & hashCode ) ^ hashCode >> 32 );
}

template <>
inline
uint32 Hasher<int64>::hash( const int64& value )
{
    uint64 hashCode = ( HashUtils::FNV_OFFSET_64 ^
        static_cast<uint64>( value ) ) * HashUtils::FNV_PRIME_64;

    return static_cast<uint32>(
        ( static_cast<uint64>( 0xFFFFFFFF ) & hashCode ) ^ hashCode >> 32 );
}

} // End nspc util

} // End nspc nge

#endif
