// counting_allocator.h
#ifndef NGE_MEM_DEFAULT_ALLOCATOR_H
#define NGE_MEM_DEFAULT_ALLOCATOR_H

#include <assert.h>

namespace nge
{

namespace mem
{

template<typename T>
class CountingAllocator : DefaultAllocator<T>
{
  private:
    // DATA MEMBERS
    /**
    * Count of globally allocated instances of class T.
    */
    static uint32 _gblAllocCount;

    /**
    * Count of locally allocated instances of class T.
    */
    uint32 lclAllocCount;

  public:
    // CONSTRUCTORS
    /**
    * Constructs the counting allocator.
    */
    CountingAllocator();

    /**
    * Counstructs a copy of a counting allocator.
    */
    CountingAllocator( const CountingAllocator<T>& copy );

    /**
    * Destructs the counting allocator
    */
    virtual ~CountingAllocator();

    // OPERATORS
    /**
    * Assigns a copy of the allocator
    */
    virtual CountingAllocator<T> operator =( const CountingAllocator<T>& assign );

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
     * Gets the local allocation count
     */
     uint32 getLocalAllocationCount();

     // GLOBAL ACCESSOR FUNCTIONS
     /**
     * Gets the global allocation count.
     */
     static uint32 getGlobalAllocationCount();
}

// GLOBAL DATA MEMBERS
template<typename T>
int CountingAllocator<T>::_gblAllocCount = 0;

// CONSTRUCTORS
template<typename T>
inline
CountingAllocator<T>::CountingAllocator()
{
    lclAllocCount = 0;
}

template<typename T>
inline
CountingAllocator<T>::CountingAllocator( const CountingAllocator<T>& copy )
{
    lclAllocCount = 0;
}

// OPERATORS
template<typename T>
inline
CountingAllocator<T>& CountingAllocator<T>::operator =( const CountingAllocator<T>& copy )
{
    return *this;
}

// MEMBER FUNCTIONS
template<typename T>
inline
T* CountingAllocator<T>::get( uint32 count )
{
    assert( count > 0 );

    lclAllocCount += count;
    _gblAllocCount += count;

    return new T[count];
}

template<typename T>
inline
void CountingAllocator<T>::release(T* pointer, uint32 count)
{
    assert( pointer != nullptr );
    assert( count > 0 );

    lclAllocCount -= count;
    _gblAllocCount -= count;

    delete[] ptr;
}

// ACCESSOR FUNCTIONS
template<typename T>
inline
uint32 CountingAllocator<T>::getLocalAllocationCount()
{
    return lclAllocCount;
}

// GLOBAL ACCESSOR FUNCTIONS
template<typename T>
inline
uint32 CountingAllocator<T>::getGlobalAllocationCount()
{
    return _gblAllocCount;
}

} //End nmspc mem

} //End nmspc nge

#endif