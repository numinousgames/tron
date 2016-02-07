// allocator_guard.h
#ifndef NGE_MEM_ALLOCATOR_GUARD_H
#define NGE_MEM_ALLOCATOR_GUARD_H

#include "engine/memory/iallocator.h"
#include "engine/memory/default_allocator.h"

namespace nge
{

namespace mem
{

template <typename T>
class AllocatorGuard : public IAllocator<T>
{
  private:
    // MEMBERS
    /**
     * The default allocator that is used if none is provided.
     */
    DefaultAllocator<T> _default;

    /**
     * The underlying allocator.
     */
    IAllocator<T>* _allocator;

  public:
    // CONSTRUCTORS
    /**
     * Constructs a wrapper that uses the default allocator.
     */
    AllocatorGuard();

    /**
     * Constructs a wrapper that uses the given allocator.
     */
    AllocatorGuard( IAllocator<T>* allocator );

    /**
     * Constructs a copy of the given wrapper.
     */
    AllocatorGuard( const AllocatorGuard<T>& other );

    /**
     * Deconstructs the wrapper.
     */
    ~AllocatorGuard();

    // MEMBER FUNCTIONS
    /**
     * Assigns this as a copy of the other wrapper.
     */
    AllocatorGuard<T>& operator=( const AllocatorGuard<T>& other );

    // FREE OPERATORS
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

    /**
     * Gets the underlying allocator.
     */
    IAllocator<T>* allocator() const;
};

// CONSTRUCTORS
template <typename T>
inline
AllocatorGuard<T>::AllocatorGuard() : _default(), _allocator( &_default )
{
}

template <typename T>
inline
AllocatorGuard<T>::AllocatorGuard( IAllocator<T>* allocator )
    : _default(), _allocator( allocator )
{
    if ( allocator == nullptr )
    {
        _allocator = &_default;
    }
}

template <typename T>
inline
AllocatorGuard<T>::AllocatorGuard( const mem::AllocatorGuard<T>& other )
    : _default()
{
    if ( other._allocator != &other._default )
    {
        _allocator = other._allocator;
    }
    else
    {
        _allocator = &_default;
    }
}

template <typename T>
inline
AllocatorGuard<T>::~AllocatorGuard()
{
    _allocator = nullptr;
}

// OPERATORS
template <typename T>
inline
AllocatorGuard<T>& AllocatorGuard<T>::operator=(
    const mem::AllocatorGuard<T>& other )
{
    if ( other._allocator != &other._default )
    {
        _allocator = other._allocator;
    }
    else
    {
        _allocator = &_default;
    }

    return *this;
}

// MEMBER FUNCTIONS
template <typename T>
inline
T* AllocatorGuard<T>::get( uint32 count )
{
    return _allocator->get( count );
}

template <typename T>
inline
void AllocatorGuard<T>::release( T* pointer, uint32 count )
{
    _allocator->release( pointer, count );
}

template <typename T>
inline
IAllocator<T>* AllocatorGuard<T>::allocator() const
{
    return _allocator;
}

} // End nspc mem

} // End nspc nge

#endif
