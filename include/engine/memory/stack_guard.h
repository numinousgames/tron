// stack_guard.h
#ifndef NGE_MEM_STACK_GUARD_H
#define NGE_MEM_STACK_GUARD_H

#include "engine/memory/allocator_guard.h"

namespace nge
{

namespace mem
{

template <typename T>
class StackGuard
{
  private:
    /**
     * The allocator that should release the memroy.
     */
    AllocatorGuard<T> _allocator;

    /**
     * The data that is being guarded.
     */
    T* _guarded;

    /**
     * The number of items that are being guarded.
     */
    uint32 _count;

    /**
     * Constructs a copy of the given guard.
     *
     * This is not a supported operation for stack guards.
     */
    StackGuard( const StackGuard<T>& guard );

    /**
     * Assigns this as a copy of the other guard.
     *
     * This is not a supported operation for stack guards.
     */
    StackGuard<T>& operator=( const StackGuard<T>& guard );

  public:
    // CONSTRUCTORS
    /**
     * Constructs an invalid stack guard.
     */
    StackGuard();

    /**
     * Constructs a stack guard for the given allocation using the default
     * allocator.
     *
     * This assumes the allocation is only one instance (not an array).
     */
    StackGuard( T* guarded );

    /**
     * Constructs a stack guard for the given allocation of the specified
     * number of instances using the default allocator.
     */
    StackGuard( T* guarded, uint32 count );

    /**
     * Constructs a stack guard for the given allocation.
     *
     * This assumes the allocation is only one instance (not an array).
     */
    StackGuard( IAllocator<T>* alloc, T* guarded );

    /**
     * Constructs a stack guard for an allocation of the specified number of
     * instances.
     */
    StackGuard( IAllocator<T>* alloc, T* guarded, uint32 count );

    /**
     * Moves the guarded memory to this instance.
     */
    StackGuard( StackGuard<T>&& guard );

    /**
     * Destructs the stack guard and releases the guarded memory.
     */
    ~StackGuard();

    // OPERATORS
    /**
     * Moves the stack guard to this instance.
     *
     * Behavior is undefined when:
     * Already guarding an allocation.
     */
    StackGuard<T>& operator=( StackGuard<T>&& guard );

    /**
     * Checks if the right hand side is equal to the underlying pointer.
     */
    bool operator==( bool rhs ) const;

    /**
     * Checks if the right hand side  is not equal to the underlying pointer.
     */
    bool operator!=( bool rhs ) const;

    /**
     * Checks if the guarded memory is null.
     */
    bool operator!() const;

    /**
     * Checks if the guarded memory is not null.
     */
    operator bool() const;

    /**
     * Dereferences the guarded content.
     *
     * Behaviour is defined when:
     * There is no guarded allocation.
     */
    T* operator->() const;

    /**
     * Dereferences the guarded content.
     *
     * Behavior is undefined when:
     * There is no guarded allocation.
     */
    T& operator*() const;

    /**
     * Gets the guarded content at the given index.
     *
     * Behavior is undefined when:
     * The index is out of bounds.
     */
    T& operator[]( uint32 index ) const;
};

// CONSTRUCTORS
template <typename T>
inline
StackGuard<T>::StackGuard() : _allocator( nullptr ), _guarded( nullptr ),
                              _count( 0 )
{
}

template <typename T>
inline
StackGuard<T>::StackGuard( T* guarded )
    : _allocator( nullptr ), _guarded( guarded ), _count( 1 )
{
}


template <typename T>
inline
StackGuard<T>::StackGuard( T* guarded, uint32 count )
    : _allocator( nullptr ), _guarded( guarded ), _count( count )
{
}

template <typename T>
inline
StackGuard<T>::StackGuard( IAllocator <T>* alloc, T* guarded )
    : _allocator( alloc ), _guarded( guarded ), _count( 1 )
{
}

template <typename T>
inline
StackGuard<T>::StackGuard( IAllocator <T>* alloc, T* guarded, uint32 count )
    : _allocator( alloc ), _guarded( guarded ), _count( count )
{
}

template <typename T>
inline
StackGuard<T>::StackGuard( const mem::StackGuard<T>& guard )
    : _allocator( guard._allocator ), _guarded( guard._guarded ),
      _count( guard._count )
{
}


template <typename T>
inline
StackGuard<T>::StackGuard( mem::StackGuard<T>&& guard )
    : _allocator( guard._allocator ), _guarded( guard._guarded ),
      _count( guard._count )
{
    guard._allocator = nullptr;
    guard._guarded = nullptr;
    _count = 0;
}

template <typename T>
inline
StackGuard<T>::~StackGuard()
{
    if ( _guarded == nullptr )
    {
        assert( _count == 0 );
        return;
    }

    assert( _count > 0 );

    _allocator.release( _guarded, _count );
    _guarded = nullptr;
    _count = 0;
}

// OPERATORS
template <typename T>
inline
StackGuard<T>& StackGuard<T>::operator=( const mem::StackGuard<T>& guard )
{
    _allocator = guard._allocator;
    _guarded = guard._guarded;
    _count = guard._count;

    return *this;
}

template <typename T>
inline
StackGuard<T>& StackGuard<T>::operator=( mem::StackGuard<T>&& guard )
{
    assert( _guarded == nullptr );
    assert( _count <= 0 );

    _allocator = guard._allocator;
    _guarded = guard._guarded;
    _count = guard._count;

    guard._allocator = nullptr;
    guard._guarded = nullptr;
    guard._count = 0;

    return *this;
}

template <typename T>
inline
bool StackGuard<T>::operator==( bool rhs ) const
{
    return ( _guarded != nullptr ) == rhs;
}

template <typename T>
inline
bool StackGuard<T>::operator!=( bool rhs ) const
{
    return ( _guarded != nullptr ) != rhs;
}

template <typename T>
inline
bool StackGuard<T>::operator!() const
{
    return ( _guarded == nullptr );
}

template <typename T>
inline
StackGuard<T>::operator bool() const
{
    return _guarded != nullptr;
}

template <typename T>
inline
T* StackGuard<T>::operator->() const
{
    return _guarded;
}

template <typename T>
inline
T& StackGuard<T>::operator*() const
{
    return *_guarded;
}

template <typename T>
inline
T& StackGuard<T>::operator[]( uint32 index ) const
{
    assert( index < _count );
    return _guarded[index];
}

} // End nspc mem

} // End nspc nge

#endif
