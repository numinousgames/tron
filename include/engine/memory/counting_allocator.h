// counting_allocator.h
#ifndef NGE_MEM_COUNTING_ALLOCATOR_H
#define NGE_MEM_COUNTING_ALLOCATOR_H

#include <assert.h>

#include "engine/intdef.h"
#include "engine/memory/allocator_guard.h"

namespace nge
{

namespace mem
{

template <typename T>
class CountingAllocator : public IAllocator<T>
{
  private:
    // GLOBALS
    /**
     * Global instance count of type T.
     */
    static uint32 g_count;

    // MEMBERS
    /**
     * The underlying allocator.
     */
    AllocatorGuard<T> _allocator;

    /**
     * Local instance count of type T.
     */
    uint32 _count;

  public:
    // CONSTRUCTORS
    /**
     * Constructs the counting allocator using the default allocator.
     */
    CountingAllocator();

    /**
     * Constructs a counting allocator that performs allocation using the
     * given allocator.
     */
    CountingAllocator( IAllocator<T>* alloc );

    /**
     * Constructs a copy of a counting allocator.
     *
     * This does not retain the local allocation count.
     */
    CountingAllocator( const CountingAllocator<T>& alloc );

    /**
     * Destructs the counting allocator
     */
    virtual ~CountingAllocator();

    // OPERATORS
    /**
     * Assigns this as a copy of the allocator.
     *
     * This does not retain the local allocation count.
     */
    CountingAllocator<T>& operator=( const CountingAllocator<T>& alloc );

    // MEMBER FUNCTIONS
    /**
     * Allocates the given number of instances.
     *
     * Behavior is undefined when:
     * T is void
     * count is less than or equal to zero
     * out of memory
     */
     virtual T* get( uint32 count );

     /**
      * Releases the allocation with the given number of instances.
      *
      * Behavior is undefined when:
      * T is void
      * pointer is invalid
      * count is less than or equal to zero
      * out of memory
      */
     virtual void release( T* pointer, uint32 count );

     // ACCESSOR FUNCTIONS
     /**
      * Gets the number of instance of T that are currently allocated locally.
      */
     uint32 getAllocationCount();

     // GLOBAL FUNCTIONS
     /**
      * Gets the global allocation count.
      */
     static uint32 getGlobalAllocationCount();
};

// GLOBALS
template <typename T>
uint32 CountingAllocator<T>::g_count = 0;

// CONSTRUCTORS
template <typename T>
inline
CountingAllocator<T>::CountingAllocator() : _allocator( nullptr ), _count( 0 )
{
}

template <typename T>
inline
CountingAllocator<T>::CountingAllocator( IAllocator<T>* alloc )
    : _allocator( alloc ), _count( 0 )
{
}

template <typename T>
inline
CountingAllocator<T>::CountingAllocator( const CountingAllocator<T>& alloc )
    : _allocator( alloc._allocator ), _count( 0 )
{
}

template <typename T>
inline
CountingAllocator<T>::~CountingAllocator()
{
    // check for memory leak
    assert( _count <= 0 );
    _allocator = nullptr;
}

// OPERATORS
template <typename T>
inline
CountingAllocator<T>& CountingAllocator<T>::operator=(
    const CountingAllocator<T>& copy )
{
    _allocator = copy._allocator;

    return *this;
}

// MEMBER FUNCTIONS
template <typename T>
inline
T* CountingAllocator<T>::get( uint32 count )
{
    assert( count > 0 );

    _count += count;
    g_count += count;

    return new T[count];
}

template <typename T>
inline
void CountingAllocator<T>::release(T* pointer, uint32 count)
{
    assert( pointer != nullptr );
    assert( count > 0 );
    assert( _count >= count );

    _count -= count;
    g_count -= count;

    delete[] pointer;
}

// ACCESSOR FUNCTIONS
template <typename T>
inline
uint32 CountingAllocator<T>::getAllocationCount()
{
    return _count;
}

// GLOBAL ACCESSOR FUNCTIONS
template <typename T>
inline
uint32 CountingAllocator<T>::getGlobalAllocationCount()
{
    return g_count;
}

} //End nspc mem

} //End nspc nge

#endif