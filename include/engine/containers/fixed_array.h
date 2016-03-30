// fixed_array.h
//
// This defines a runtime sized fixed size array. This provides
// convenient methods for adding/removing items.
//
// This is different from DynamicArray in that accessing the internal
// array is a valid operation. This also differs from DynamicArray
// in that it guarantees that items are contiguous in memory and
// start at the beginning of the array.
//
#ifndef NGE_CNTR_FIXED_ARRAY_H
#define NGE_CNTR_FIXED_ARRAY_H

#include <stdexcept>
#include <engine/port.h>

#include "engine/intdef.h"
#include "engine/memory/allocator_guard.h"
#include "engine/memory/memory_utils.h"

namespace nge
{

namespace cntr
{

template <typename T>
class FixedArray
{
  private:
    // CLASSES
    /**
     * Defines an iterator for the array.
     */
    template <typename APTR, typename TREF, typename CTREF, typename TPTR>
    class ArrayIterator
    {
        // MEMBERS
        /**
         * The array that is being iterated.
         */
        APTR _iterArray;

        /**
         * The current index in the array.
         */
        uint32 _iterIndex;

      public:
        // CONSTRUCTORS
        /**
         * Constructs a new iterator.
         */
        ArrayIterator();

        /**
         * Constructs an iterator for an array starting at the given index.
         */
        ArrayIterator( APTR array, uint32 index );

        /**
         * Constructs a copy of the given iterator.
         */
        ArrayIterator( const ArrayIterator& iter );

        /**
         * Destructs the iterator.
         */
        ~ArrayIterator();

        // OPERATORS
        /**
         * Assigns this as a copy of the other iterator.
         */
        ArrayIterator& operator=( const ArrayIterator& iter );

        /**
         * Moves to the next item.
         */
        ArrayIterator& operator++();

        /**
         * Moves to the next item.
         */
        ArrayIterator& operator++( int32 );

        /**
         * Moves to the previous item.
         */
        ArrayIterator& operator--();

        /**
         * Moves to the previous item.
         */
        ArrayIterator& operator--( int32 );

        /**
         * Gets the element at the current position.
         */
        CTREF operator*() const;

        /**
         * Gets the element at the current position.
         */
        TREF operator*();

        /**
         * Gets the element at the current position.
         */
        TPTR operator->() const;

        /**
         * Gets the element at the current position.
         */
        TPTR operator->();

        /**
         * Checks if the other iterator is at the same position.
         */
        bool operator==( const ArrayIterator& iter ) const;

        /**
         * Checks if the other iterator is not at the same position.
         */
        bool operator!=( const ArrayIterator& iter ) const;
    };

    // CONSTANTS
    /**
     * The default array capacity.
     */
    static constexpr uint32 DEFAULT_CAPACITY = 32;

    // MEMBERS
    /**
     * The allocator.
     */
    mem::AllocatorGuard<T> _allocator;

    /**
     * The underlying array.
     */
    T* _values;

    /**
     * The size.
     */
    uint32 _size;

    /**
     * The capacity.
     */
    uint32 _capacity;

    /**
     * If the data is external.
     *
     * This occurs when an array is wrapped.
     */
    bool _isDataExternal;

    // HELPER FUNCTIONS
    /**
     * Shifts the given number of items forward one spot starting at the
     * given index
     */
    void shiftForward( uint32 start );

    /**
     * Shifts the given number of items backward one spot starting at the
     * given index.
     */
    void shiftBackward( uint32 start );

    // CONSTRUCTORS
    /**
     * Constructs a new fixed array that wraps pre-allocated data.
     */
    FixedArray( T* data, uint32 size, uint32 capacity );

  public:
    // TYPES
    /**
     * Defines an iterator for the array.
     */
    typedef ArrayIterator<FixedArray<T>*, T&, const T&, T*> Iterator;

    /**
     * Defines a constant iterator for the array.
     */
    typedef ArrayIterator<const FixedArray<T>*, const T&, const T&, const T*>
        ConstIterator;

    // GLOBAL METHODS
    /**
     * Wraps a pre-allocated array.
     *
     * The size must be the number of items in the array.
     * This assumes the allocated array is full.
     *
     * The fixed array will not deallocate the data during destruction.
     * If this instance is copied the copy will not reference the original
     * array. Instead it will allocate it's own copy which it will assume
     * responsibility for (it will deallocate the copy when appropriate).
     * If it is moved to a new instance the new instance will reference the
     * wrapped data.
     */
    static FixedArray<T> wrap( T* data, uint32 size );

    /**
     * Wraps a pre-allocated array.
     *
     * The size must be the number of items in use starting from index 0.
     * The capacity must be the allocated number of items in the array.
     *
     * The fixed array will not deallocate the data during destruction.
     * If this instance is copied the copy will not reference the original
     * array. Instead it will allocate it's own copy which it will assume
     * responsibility for (it will deallocate the copy when appropriate).
     * If it is moved to a new instance the new instance will reference the
     * wrapped data.
     */
    static FixedArray<T> wrap( T* data, uint32 size, uint32 capacity );

    // CONSTRUCTORS
    /**
     * Constructs a new array with a capacity of 32.
     */
    FixedArray();

    /**
     * Constructs a new array with the given capacity.
     */
    FixedArray( uint32 capacity );

    /**
     * Constructs a new array with a capacity of 32 created using the
     * given allocator.
     */
    FixedArray( mem::IAllocator<T>* allocator );

    /**
     * Constructs a new array using the given allocator and capacity.
     */
    FixedArray( mem::IAllocator<T>* allocator, uint32 capacity );

    /**
     * Constructs a copy of the given array.
     */
    FixedArray( const FixedArray<T>& array );

    /**
     * Constructs an array by moving the resources to a new instance.
     */
    FixedArray( FixedArray<T>&& array );

    /**
     * Destructs the fixed array.
     */
    ~FixedArray();

    // OPERATORS
    /**
     * Assigns this as a copy of the other array.
     */
    FixedArray<T>& operator=( const FixedArray<T>& array );

    /**
     * Moves the array to this instance.
     */
    FixedArray<T>& operator=( FixedArray<T>&& array );

    /**
     * Gets the value at the given index.
     *
     * Behavior is undefined when:
     * index is out of bounds.
     */
    const T& operator[]( uint32 index ) const;

    /**
     * Gets the value at the given index.
     *
     * Behavior is undefined when:
     * index is out of bounds.
     */
    T& operator[]( uint32 index );

    // MEMBER FUNCTIONS
    /**
     * Gets the value at the given index.
     *
     * Throws a runtime_error when:
     * index is out of bounds.
     */
    T& at( uint32 index ) const;

    /**
     * Adds the value to the end of the array.
     *
     * Behavior is undefined when:
     * array is full.
     */
    void push( const T& value );

    /**
     * Moves the value to the end of the array.
     *
     * Behavior is undefined when:
     * array is full.
     */
    void push( T&& value );

    /**
     * Adds the value to the front of the array.
     *
     * Behavior is undefined when:
     * array is full.
     */
    void pushFront( const T& value );

    /**
     * Moves the value to the front of the array.
     *
     * Behavior is undefined when:
     * array is full.
     */
    void pushFront( T&& value );

    /**
     * Inserts the value at the given index in the array.
     *
     * Throws a runtime_error when:
     * index is out of bounds.
     *
     * Behavior is undefined when:
     * array is full.
     */
    void insertAt( uint32 index, const T& value );

    /**
     * Moves the value to the given index in the array.
     *
     * Throws a runtime_error when:
     * index is out of bounds.
     *
     * Behavior is undefined when:
     * array is full.
     */
    void insertAt( uint32 index, T&& value );

    /**
     * Removes the value at the back of the array.
     *
     * Behavior is undefined when:
     * array is empty.
     */
    T pop();

    /**
     * Removes the value at the back of the array.
     *
     * Behavior is undefined when:
     * array is empty.
     */
    T popFront();

    /**
     * Removes the value at the given index in the array.
     *
     * Throws a runtime_error when:
     * index is out of bounds.
     *
     * Behavior is undefined when:
     * array is empty.
     */
    T removeAt( uint32 index );

    /**
     * Removes the value and returns if it was found.
     *
     * This will only remove the first occurance.
     *
     * Behavior is undefined when:
     * array is empty.
     */
    bool remove( const T& value );

    /**
     * Removes all items from the array.
     */
    void clear();

    /**
     * Gets an iterator at the start of the array.
     */
    Iterator begin();

    /**
     * Gets a constant iterator at the start of the array.
     */
    ConstIterator cbegin() const;

    /**
     * Gets an iterator at the end of the array.
     */
    Iterator end();

    /**
     * Gets an constant iterator at the end of the array.
     */
    ConstIterator cend() const;

    /**
     * Gets the index of the given value.
     */
    uint32 indexOf( const T& value ) const;

    /**
     * Checks if the array contains the given value.
     */
    bool has( const T& value ) const;

    /*
     * Gets a pointer to the underlying array.
     *
     * This should only be used to pass the array to pass the array to
     * external libraries.
     */
    T* data() const;

    /**
     * Gets the size of the array.
     */
    uint32 size() const;

    /**
     * Gets the capacity of the array.
     */
    uint32 capacity() const;

    /**
     * Checks if the array is empty.
     */
    bool isEmpty() const;

    /**
     * Checks if the array is full.
     */
    bool isFull() const;
};

// CONSTANTS
template <typename T>
constexpr uint32 FixedArray<T>::DEFAULT_CAPACITY;

// GLOBAL METHODS
template <typename T>
inline
FixedArray<T> FixedArray<T>::wrap( T* data, uint32 size )
{
    return FixedArray<T>::wrap( data, size, size );
}

template <typename T>
inline
FixedArray<T> FixedArray<T>::wrap( T* data, uint32 size, uint32 capacity )
{
    return FixedArray<T>( data, size, capacity );
}

// CONSTRUCTORS
template <typename T>
inline
FixedArray<T>::FixedArray() : FixedArray( nullptr, DEFAULT_CAPACITY )
{
}

template <typename T>
inline
FixedArray<T>::FixedArray( uint32 capacity ) : FixedArray( nullptr, capacity )
{
}

template <typename T>
inline
FixedArray<T>::FixedArray( mem::IAllocator<T>* allocator )
    : FixedArray( allocator, DEFAULT_CAPACITY )
{
}

template <typename T>
inline
FixedArray<T>::FixedArray( mem::IAllocator<T>* allocator, uint32 capacity )
    : _allocator( allocator ), _values( nullptr ), _size( 0 ),
      _capacity( capacity ), _isDataExternal( false )
{
    _values = _allocator.get( _capacity );
}

template <typename T>
inline
FixedArray<T>::FixedArray( T* data, uint32 size, uint32 capacity )
    : _allocator( nullptr ), _values( data ), _size( size ),
      _capacity( capacity ), _isDataExternal( true )
{
}

template <typename T>
inline
FixedArray<T>::FixedArray( const FixedArray<T>& array )
    : _allocator( array._allocator ), _values( nullptr ),
      _size( array._size ), _capacity( array._capacity ),
      _isDataExternal( false )
{
    _values = _allocator.get( _capacity );
    mem::MemoryUtils::copy( _values, array._values, _size );
}

template <typename T>
inline
FixedArray<T>::FixedArray( FixedArray<T>&& array )
    : _allocator( array._allocator ), _values( array._values ),
      _size( array._size ), _capacity( array._capacity ),
      _isDataExternal( array._isDataExternal )

{
    array._values = nullptr;
    array._size = 0;
    array._capacity = 0;
    array._isDataExternal = false;
}

template <typename T>
inline
FixedArray<T>::~FixedArray()
{
    if ( !_isDataExternal && _values != nullptr )
    {
        _allocator.release( _values, _capacity );
    }

    _values = nullptr;
}

// OPERATORS
template <typename T>
FixedArray<T>& FixedArray<T>::operator=( const FixedArray& array )
{
    if ( !_isDataExternal && _values != nullptr )
    {
        _allocator.release( _values, _capacity );
        _values = nullptr;
    }

    _allocator = array._allocator;
    _size = array._size;
    _capacity = array._capacity;
    _isDataExternal = false;

    _values = _allocator.get( _capacity );
    mem::MemoryUtils::copy( _values, array._values, _capacity );

    return *this;
}

template <typename T>
FixedArray<T>& FixedArray<T>::operator=( FixedArray<T>&& array )
{
    if ( !_isDataExternal && _values != nullptr )
    {
        _allocator.release( _values, _capacity );
        _values = nullptr;
    }

    _allocator = array._allocator;
    _values = array._values;
    _size = array._size;
    _capacity = array._capacity;
    _isDataExternal = array._isDataExternal;

    array._values = nullptr;
    array._size = 0;
    array._capacity = 0;

    return *this;
}

template <typename T>
inline
const T& FixedArray<T>::operator[]( uint32 index ) const
{
    assert( index < _size );
    return _values[index];
}

template <typename T>
inline
T& FixedArray<T>::operator[]( uint32 index )
{
    assert( index < _size );
    return _values[index];
}

// MEMBER FUNCTIONS
template <typename T>
inline
T& FixedArray<T>::at( uint32 index ) const
{
    if ( index >= _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    return _values[index];
}

template <typename T>
inline
void FixedArray<T>::push( const T& value )
{
    assert( _size < _capacity );
    _values[_size++] = value;
}

template <typename T>
inline
void FixedArray<T>::push( T&& value )
{
    assert( _size < _capacity );
    _values[_size++] = std::move( value );
}

template <typename T>
inline
void FixedArray<T>::pushFront( const T& value )
{
    assert( _size < _capacity );
    shiftForward( 0 );
    _values[0] = value;
    ++_size;
}

template <typename T>
inline
void FixedArray<T>::pushFront( T&& value )
{
    assert( _size < _capacity );
    shiftForward( 0 );
    _values[0] = std::move( value );
    ++_size;
}

template <typename T>
inline
void FixedArray<T>::insertAt( uint32 index, const T& value )
{
    assert( _size < _capacity );
    if ( index > _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    shiftForward( index );
    ++_size;
    _values[index] = value;
}

template <typename T>
inline
void FixedArray<T>::insertAt( uint32 index, T&& value )
{
    assert( _size < _capacity );
    if ( index > _size )
    {
        throw new std::runtime_error( "Index is out of bounds! ");
    }

    shiftForward( index );
    ++_size;
    _values[index] = value;
}

template <typename T>
inline
T FixedArray<T>::pop()
{
    assert( _size > 0 );
    T elem = std::move( _values[--_size] );
    return elem;
}

template <typename T>
inline
T FixedArray<T>::popFront()
{
    assert( _size > 0 );
    T elem = std::move( _values[0] );
    shiftBackward( 0 );
    --_size;

    return elem;
}

template <typename T>
inline
T FixedArray<T>::removeAt( uint32 index )
{
    assert( _size > 0 );

    if ( index >= _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    T elem = std::move( _values[index] );
    shiftBackward( index );
    --_size;

    return elem;
}

template <typename T>
inline
bool FixedArray<T>::remove( const T& value )
{
    uint32 index = indexOf( value );
    if ( index == static_cast<uint32>( -1 ) )
    {
        return false;
    }

    removeAt( index );
    return true;
}

template <typename T>
inline
void FixedArray<T>::clear()
{
    _size = 0;
}

template <typename T>
inline
typename FixedArray<T>::Iterator FixedArray<T>::begin()
{
    return Iterator( this, 0 );
}

template <typename T>
inline
typename FixedArray<T>::ConstIterator FixedArray<T>::cbegin() const
{
    return ConstIterator( this, 0 );
}

template <typename T>
inline
typename FixedArray<T>::Iterator FixedArray<T>::end()
{
    return Iterator( this, _size );
}

template <typename T>
inline
typename FixedArray<T>::ConstIterator FixedArray<T>::cend() const
{
    return ConstIterator( this, _size );
}

template <typename T>
uint32 FixedArray<T>::indexOf( const T& value ) const
{
    uint32 i;
    bool found;
    for ( i = 0, found = false; !found && i < _size; ++i )
    {
        if ( _values[i] == value )
        {
            found = true;
        }
    }

    return found ? --i : static_cast<uint32>( -1 );
}

template <typename T>
bool FixedArray<T>::has( const T& value ) const
{
    uint32 i;
    bool found;
    for ( i = 0, found = false; !found && i < _size; ++i )
    {
        if ( _values[i] == value )
        {
            found = true;
        }
    }

    return found;
}

template <typename T>
inline
T* FixedArray<T>::data() const
{
    return _values;
}

template <typename T>
inline
uint32 FixedArray<T>::size() const
{
    return _size;
}

template <typename T>
inline
uint32 FixedArray<T>::capacity() const
{
    return _capacity;
}

template <typename T>
inline
bool FixedArray<T>::isEmpty() const
{
    return _size <= 0;
}

template <typename T>
inline
bool FixedArray<T>::isFull() const
{
    return _size >= _capacity;
}

// HELPER FUNCTIONS
template <typename T>
inline
void FixedArray<T>::shiftForward( uint32 start )
{
    assert( start >= 0 && start <= _size );
    uint32 i;
    for ( i = _size; i > start; --i )
    {
        _values[i] = std::move( _values[i - 1] );
    }
}

template <typename T>
inline
void FixedArray<T>::shiftBackward( uint32 start )
{
    assert( start >= 0 && start < _size );
    uint32 i;
    for ( i = start; i < _size - 1; ++i )
    {
        _values[i] = std::move( _values[i + 1] );
    }
}

// ITERATOR CONSTRUCTORS
template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::ArrayIterator()
    : _iterArray( nullptr ), _iterIndex( 0 )
{
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::ArrayIterator(
    APTR array, uint32 index ) : _iterArray( array ), _iterIndex( index )
{
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::ArrayIterator(
    const ArrayIterator& iter )
    : _iterArray( iter._iterArray ), _iterIndex( iter._iterIndex )
{
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::~ArrayIterator()
{
    _iterArray = nullptr;
    _iterIndex = static_cast<uint32>( -1 );
}

// ITERATOR OPERATORS
template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
vc_typename FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator=(
    const ArrayIterator& iter )
{
    _iterArray = iter._iterArray;
    _iterIndex = iter._iterIndex;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
vc_typename FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator++()
{
    ++_iterIndex;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
vc_typename FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator++( int32 )
{
    ++_iterIndex;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
vc_typename FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator--()
{
    _iterIndex = ( _iterIndex > 0 ) ? _iterIndex - 1 : _iterArray->_size;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
vc_typename FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator--( int32 )
{
    _iterIndex = ( _iterIndex > 0 ) ? _iterIndex - 1 : _iterArray->_size;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
CTREF
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator*() const
{
    return ( *_iterArray )[_iterIndex];
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
TREF FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator*()
{
    return ( *_iterArray )[_iterIndex];
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
TPTR
FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator->() const
{
    return &( *_iterArray )[_iterIndex];
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
TPTR FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator->()
{
    return &( *_iterArray )[_iterIndex];
}


template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
bool FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator==(
    const ArrayIterator& iter ) const
{
    return _iterArray == iter._iterArray && _iterIndex == iter._iterIndex;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
bool FixedArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator!=(
    const ArrayIterator& iter ) const
{
    return _iterArray != iter._iterArray || _iterIndex != iter._iterIndex;
}

} // End nspc cntr

} // End nspc nge

#endif
