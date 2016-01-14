// memory_utils.h
#ifndef NGE_MEM_MEMORY_UTILS_H
#define NGE_MEM_MEMORY_UTILS_H

#include <utility>

#include "engine/intdef.h"

namespace nge
{

namespace mem
{

struct MemoryUtils
{
  public:
    /**
     * Copies items from the source to the destination.
     */
    template <typename T>
    static void copy( T* src, T* dst, uint32 count );

    /**
     * Moves items from the source to the destination.
     */
    template <typename T>
    static void move( T* src, T* dst, uint32 count );
};

template <typename T>
void MemoryUtils::copy( T* src, T* dst, uint32 count )
{
    uint32 i;
    for ( i = 0; i < count; ++i )
    {
        dst[i] = src[i];
    }
}

template <typename T>
void MemoryUtils::move( T* src, T* dst, uint32 count )
{
    uint32 i;
    for ( i = 0; i < count; ++i )
    {
        dst[i] = std::move( src[i] );
    }
}

} // End nspc mem

} // End nspc nge

#endif