// dynamic_array.h
// The dynamic array is a resizable array that automatically grows and shrinks
// as items are added or removed respectively.
//
// The implementation is circular meaning that the internally the index zero
// may actually mean the 5th position in the memory block and the last item
// may actually be in the 4th position in memory.
//
// When the array grows or shrinks it gradually copies over the old items to
// reduce the overhead of resizing.

#ifndef NGE_CNTR_DYNAMIC_ARRAY_H
#define NGE_CNTR_DYNAMIC_ARRAY_H

#include <algorithm>
#include <assert.h>
#include <stdexcept>
#include <utility>

#include "engine/memory/allocator_guard.h"
#include "engine/memory/iallocator.h"
#include "engine/memory/memory_utils.h"

namespace nge
{

namespace cntr
{

template <typename T>
class DynamicArray
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
     * The minimum array capacity.
     */
    static constexpr uint32 MIN_CAPACITY = 32;

    // MEMBERS
    /**
     * The allocator.
     */
    mem::AllocatorGuard<T> _allocator;

    /**
     * The array of values.
     */
    T* _values;

    /**
     * The index of the first element in the array.
     */
    uint32 _first;

    /**
     * The number of items in the array.
     */
    uint32 _size;

    /**
     * The capacity of the array.
     */
    uint32 _capacity;

    // HELPER FUNCTIONS
    /**
     * Doubles the capacity of the array.
     */
    void grow();

    /**
     * Halves the capacity of the array.
     */
    void shrink();

    /**
     * Resizes the array.
     */
    void resize(uint32 newCapacity);

    /**
     * Shifts the given number of items forward one spot starting at the given
     * index.
     */
    void shiftForward( uint32 start );

    /**
     * Shifts the given number of items backward one spot starting at the given
     * index.
     */
    void shiftBackward( uint32 start );

    /**
     * Wraps the index inside of the circular bounds.
     */
    uint32 wrap( uint32 index ) const;

    /**
     * Checks if the underlying array should grow.
     */
    bool shouldGrow() const;

    /**
     * Checks if the underlying array should shrink.
     */
    bool shouldShrink() const;

  public:
    // TYPES
    /**
     * Defines an iterator for the array.
     */
    typedef ArrayIterator<DynamicArray<T>*, T&, const T&, T*> Iterator;

    /**
     * Defines a constant iterator for the array.
     */
    typedef ArrayIterator<const DynamicArray<T>*, const T&, const T&, const T*>
        ConstIterator;

    // CONSTRUCTORS
    /**
     * Constructs a new DynamicArray.
     */
    DynamicArray();

    /**
     * Constructs a new DynamicArray using the given allocator.
     */
    DynamicArray( mem::IAllocator<T>* allocator );

    /**
     * Constructs a new DynamicArray using the given initial capacity.
     *
     * This should be used when filling a large array. This should not be
     * used if items will be removed soon after construction as it will cause
     * the array to shrink.
     */
    DynamicArray( uint32 capacity );

    /**
     * Constructs a new DynamicArray using the given allocator and initial
     * capacity.
     */
    DynamicArray( mem::IAllocator<T>* allocator, uint32 capacity );

    /**
     * Constructs a copy of the given array.
     */
    DynamicArray( const DynamicArray<T>& array );

    /**
     * Moves the array data to a new instance.
     */
    DynamicArray( DynamicArray<T>&& array );

    /**
     * Destructs the array.
     */
    ~DynamicArray();

    // OPERATORS
    /**
     * Makes this array a copy of another.
     */
    DynamicArray<T>& operator=( const DynamicArray<T>& array );

    /**
     * Moves the data from the other array to this one.
     *
     * Deletes this array in the process.
     */
    DynamicArray<T>& operator=( DynamicArray<T>&& array );

    /**
     * Gets the value at the given index.
     *
     * Behavior is undefined when:
     * index is out of bounds
     */
    const T& operator[]( uint32 index ) const;

    /**
     * Gets the value at the given index.
     *
     * Behavior is undefined when:
     * index is out of bounds
     */
    T& operator[]( uint32 index );

    // MEMBER FUNCTIONS
    /**
     * Gets the value at the given index.
     *
     * Throws a runtime_error when:
     * index is out of bounds
     */
    T& at( uint32 index ) const;

    /**
     * Adds the value to the end of the array.
     */
    void push( const T& value );

    /**
     * Moves the value to the end of the array.
     */
    void push( T&& value );

    /**
     * Adds the value to the front of the array.
     */
    void pushFront( const T& value );

    /**
     * Moves the value to the front of the array.
     */
    void pushFront( T&& value );

    /**
     * Inserts the value at the given index in the array.
     */
    void insertAt( uint32 index, const T& value );

    /**
     * Moves the value to the given index in the array.
     */
    void insertAt( uint32 index, T&& value );

    /**
     * Removes the value at the back of the array.
     */
    T pop();

    /**
     * Removes the value at the front of the array.
     */
    T popFront();

    /**
     * Removes the value at the given index in the array.
     */
    T removeAt( uint32 index );

    /**
     * Removes the given value from the array and returns if it was found.
     *
     * This will only remove the first occurance.
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
     * Gets a constant iterator at the end of the array.
     */
    ConstIterator cend() const;

    /**
     * Finds the index of the first occurrence of the value in the array.
     *
     * Returns (uint32)-1 if not found.
     */
    uint32 indexOf( const T& value ) const;

    /**
     * Checks if the array contains the given value.
     */
    bool has( const T& value ) const;

    /**
     * Gets the size of the array.
     */
    uint32 size() const;

    /**
     * Checks if the array is empty.
     */
    bool isEmpty() const;
};

// CONSTANTS
template <typename T>
constexpr uint32 DynamicArray<T>::MIN_CAPACITY;

// CONSTRUCTORS
template <typename T>
inline
DynamicArray<T>::DynamicArray()
    : _allocator(), _values( nullptr ), _first( 0 ), _size( 0 ),
      _capacity( MIN_CAPACITY )
{
    _values = _allocator.get( _capacity );
}

template <typename T>
inline
DynamicArray<T>::DynamicArray( mem::IAllocator<T>* allocator )
    : _allocator( allocator ), _values( nullptr ), _first( 0 ),
      _size( 0 ), _capacity( MIN_CAPACITY )
{
    _values = _allocator.get( _capacity );
}

template <typename T>
inline
DynamicArray<T>::DynamicArray( uint32 capacity )
    : _allocator(), _values( nullptr ), _first( 0 ), _size( 0 ),
      _capacity( MIN_CAPACITY )
{
    while ( _capacity < capacity )
    {
        _capacity <<= 1;
    }

    _values = _allocator.get( _capacity );
}

template <typename T>
inline
DynamicArray<T>::DynamicArray( mem::IAllocator<T>* allocator, uint32 capacity )
    : _allocator( allocator ), _values( nullptr ), _first( 0 ),
      _size( 0 ), _capacity( MIN_CAPACITY )
{
    while ( _capacity < capacity )
    {
        _capacity <<= 1;
    }

    _values = _allocator.get( _capacity );
}

template <typename T>
DynamicArray<T>::DynamicArray( const DynamicArray<T>& array )
    : _allocator( array._allocator ), _values( nullptr ), _first( 0 ),
      _size( array._size ), _capacity( array._capacity )
{
    using namespace mem;

    uint32 a;
    uint32 b;
    if ( array._values != nullptr )
    {
        _values = _allocator.get( _capacity );
        a = array._capacity - ( array._first + array._size );
        b = ( array._first + array._size ) % array._capacity;

        if ( a >= array._size )
        {
            MemoryUtils::copy( _values, array._values + array._first,
                               array._size );
        }
        else
        {
            MemoryUtils::copy( _values, array._values + array._first, a );
            MemoryUtils::copy( _values + a, array._values, b );
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray( DynamicArray<T>&& array )
    : _allocator( array._allocator ), _values( nullptr ), _first( 0 ),
      _size( array._size ), _capacity( array._capacity )
{
    using namespace mem;

    uint32 a;
    uint32 b;
    if ( array._values != nullptr )
    {
        _values = _allocator.get( _capacity );
        a = array._capacity - ( array._first + array._size );
        b = ( array._first + array._size ) % array._capacity;

        if ( a >= array._size )
        {
            MemoryUtils::move( _values, array._values + array._first,
                               array._size );
        }
        else
        {
            MemoryUtils::move( _values, array._values + array._first, a );
            MemoryUtils::move( _values + a, array._values, b );
        }
    }

    array._allocator = nullptr;
    array._values = nullptr;
    array._first = 0;
    array._size = 0;
    array._capacity = 0;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    if ( _values != nullptr )
    {
        _allocator.release( _values, _capacity );
        _values = nullptr;
    }

    _allocator = nullptr;
}

// OPERATORS
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(
    const cntr::DynamicArray<T>& array )
{
    using namespace mem;

    if ( _values != nullptr )
    {
        _allocator.release( _values, _capacity );
        _values = nullptr;
    }

    _allocator = array._allocator;
    _first = 0;
    _size = array._size;
    _capacity = array._capacity;

    uint32 a;
    uint32 b;
    if ( array._values != nullptr )
    {
        _values = _allocator.get( _capacity );
        a = array._capacity - ( array._first + array._size );
        b = ( array._first + array._size ) % array._capacity;

        if ( a >= array._size )
        {
            MemoryUtils::copy( _values, array._values + array._first,
                               array._size );
        }
        else
        {
            MemoryUtils::copy( _values, array._values + array._first, a );
            MemoryUtils::copy( _values + a, array._values, b );
        }
    }

    return *this;
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=( cntr::DynamicArray<T>&& array )
{
    using namespace mem;

    _allocator = array._allocator;
    _values = nullptr;
    _first = 0;
    _size = array._size;
    _capacity = array._capacity;

    uint32 a;
    uint32 b;
    if ( array._values != nullptr )
    {
        _values = _allocator.get( _capacity );
        a = array._capacity - ( array._first + array._size );
        b = ( array._first + array._size ) % array._capacity;

        if ( a >= array._size )
        {
            MemoryUtils::move( _values, array._values + array._first,
                               array._size );
        }
        else
        {
            MemoryUtils::move( _values, array._values + array._first, a );
            MemoryUtils::move( _values + a, array._values, b );
        }
    }

    array._allocator = nullptr;
    array._values = nullptr;
    array._first = 0;
    array._size = 0;
    array._capacity = 0;

    return *this;
}

template <typename T>
inline
const T& DynamicArray<T>::operator[]( uint32 index ) const
{
    assert( index < _size );
    return _values[wrap( index )];
}

template <typename T>
inline
T& DynamicArray<T>::operator[]( uint32 index )
{
    assert( index < _size );
    return _values[wrap( index )];
}

// MEMBER FUNCTIONS
template <typename T>
inline
T& DynamicArray<T>::at( uint32 index ) const
{
    if ( index >= _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    return _values[wrap( index )];
}

template <typename T>
inline
void DynamicArray<T>::push( const T& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    ++_size;
    ( *this )[_size - 1] = value;
}

template <typename T>
inline
void DynamicArray<T>::push( T&& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    ++_size;
    ( *this )[_size - 1] = std::move( value );
}

template <typename T>
inline
void DynamicArray<T>::pushFront( const T& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    _first = (_first > 0) ? _first - 1 : _capacity - 1;
    ++_size;
    ( *this )[0] = value;
}

template <typename T>
void DynamicArray<T>::pushFront( T&& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    _first = (_first > 0) ? _first - 1 : _capacity - 1;
    ++_size;
    ( *this )[0] = std::move( value );
}

template <typename T>
void DynamicArray<T>::insertAt( uint32 index, const T& value )
{
    if ( index > _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    if ( shouldGrow() )
    {
        grow();
    }

    shiftForward( index );
    ++_size;
    ( *this )[index] = value;
}

template <typename T>
void DynamicArray<T>::insertAt( uint32 index, T&& value )
{
    if ( index > _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    if ( shouldGrow() )
    {
        grow();
    }

    shiftForward( index );
    ++_size;
    ( *this )[index] = std::move( value );
}

template <typename T>
T DynamicArray<T>::pop()
{
    assert( _size > 0 );

    if ( shouldShrink() )
    {
        shrink();
    }

    T elem = std::move( ( *this )[_size - 1] );
    --_size;
    return elem;
}

template <typename T>
T DynamicArray<T>::popFront()
{
    assert( _size > 0 );

    if ( shouldShrink() )
    {
        shrink();
    }

    T elem = std::move( ( *this )[0] );
    ++_first;
    --_size;
    return elem;
}

template <typename T>
T DynamicArray<T>::removeAt( uint32 index )
{
    assert( _size > 0 );

    if ( index >= _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    if ( shouldShrink() )
    {
        shrink();
    }

    T elem = std::move( ( *this )[index] );
    shiftBackward( index );
    --_size;

    return elem;
}

template <typename T>
inline
bool DynamicArray<T>::remove( const T& value )
{
    uint32 index = indexOf( value );
    if ( value == static_cast<uint32>( -1 ) )
    {
        return false;
    }

    removeAt( index );
    return true;
}

template <typename T>
void DynamicArray<T>::clear()
{
    _size = 0;
    _first = 0;
}

template <typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::begin()
{
    return Iterator( this, 0 );
}

template <typename T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::cbegin() const
{
    return ConstIterator( this, 0 );
}

template <typename T>
typename DynamicArray<T>::Iterator DynamicArray<T>::end()
{
    return Iterator( this, _size );
}

template <typename T>
typename DynamicArray<T>::ConstIterator DynamicArray<T>::cend() const
{
    return ConstIterator( this, _size );
}

template <typename T>
uint32 DynamicArray<T>::indexOf( const T& value ) const
 {
    uint32 i;
    bool found;
    for ( i = 0, found = false; !found && i < _size; ++i )
    {
        if ( ( *this )[i] == value )
        {
            found = true;
        }
    }

    return found ? --i : static_cast<uint32>( -1 );
}

template <typename T>
bool DynamicArray<T>::has( const T& value ) const
{
    uint32 i;
    bool found;
    for ( i = 0, found = false; !found && i < _size; ++i )
    {
        if ( ( *this )[i] == value )
        {
            found = true;
        }
    }

    return found;
}

template <typename T>
inline
uint32 DynamicArray<T>::size() const
{
    return _size;
}

template <typename T>
inline
bool DynamicArray<T>::isEmpty() const
{
    return _size <= 0;
}

// HELPER FUNCTIONS
template <typename T>
inline
void DynamicArray<T>::grow()
{
    resize(_capacity << 1);


}

template <typename T>
inline
void DynamicArray<T>::shrink()
{
    resize(_capacity >> 1);
}

template <typename T>
void DynamicArray<T>::resize( uint32 newCapacity )
{
    assert( _values != nullptr );
    uint32 oldCapacity = _capacity;
    uint32 oldFirst = _first;
    T* oldValues = _values;

    _capacity = newCapacity;
    _values = _allocator.get( _capacity );
    _first = 0;

    uint32 i;
    for( i = 0; i < _size; ++i)
    {
        _values[i] = std::move( oldValues[( oldFirst + i ) % oldCapacity] );
    }

    _allocator.release( oldValues, oldCapacity );
}

template <typename T>
inline
void DynamicArray<T>::shiftForward( uint32 start )
{
    assert( start >= 0 && start <= _size );
    uint32 i;
    for ( i = _size; i > start; --i )
    {
        _values[wrap( i )] = std::move( _values[wrap( i - 1 )] );
    }
}

template <typename T>
inline
void DynamicArray<T>::shiftBackward( uint32 start )
{
    assert( start >= 0 && start < _size );
    uint32 i;
    for ( i = start; i < _size - 1; ++i )
    {
        _values[wrap( i )] = std::move( _values[wrap( i + 1 )] );
    }
}

template <typename T>
inline
uint32 DynamicArray<T>::wrap( uint32 index ) const
{
    return ( _first + index ) & ( _capacity - 1 );
}

template <typename T>
inline
bool DynamicArray<T>::shouldGrow() const
{
    return _size >= _capacity;
}

template <typename T>
inline
bool DynamicArray<T>::shouldShrink() const
{
    return _size <= ( _capacity >> 2 ) && _capacity > MIN_CAPACITY;
}

// ITERATOR CONSTRUCTORS
template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::ArrayIterator()
    : _iterArray( nullptr ), _iterIndex( 0 )
{
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::ArrayIterator(
    APTR array, uint32 index ) : _iterArray( array ), _iterIndex( index )
{
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::ArrayIterator(
    const ArrayIterator<APTR, TREF, CTREF, TPTR>& iter )
    : _iterArray( iter._iterArray ), _iterIndex( iter._iterIndex )
{
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::~ArrayIterator()
{
    _iterArray = nullptr;
    _iterIndex = static_cast<uint32>( -1 );
}

// ITERATOR OPERATORS
template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator=(
    const ArrayIterator<APTR, TREF, CTREF, TPTR>& iter )
{
    _iterArray = iter._iterArray;
    _iterIndex = iter._iterIndex;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator++()
{
    ++_iterIndex;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator++( int32 )
{
    ++_iterIndex;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator--()
{
    _iterIndex = ( _iterIndex > 0 ) ? _iterIndex - 1 : _iterArray->_size;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator--( int32 )
{
    _iterIndex = ( _iterIndex > 0 ) ? _iterIndex - 1 : _iterArray->_size;

    return *this;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
CTREF
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator*() const
{
    return ( *_iterArray )[_iterIndex];
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
TREF DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator*()
{
    return ( *_iterArray )[_iterIndex];
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
TPTR
DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator->() const
{
    return &( *_iterArray )[_iterIndex];
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
TPTR DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator->()
{
    return &( *_iterArray )[_iterIndex];
}


template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
bool DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator==(
    const ArrayIterator<APTR, TREF, CTREF, TPTR>& iter ) const
{
    return _iterArray == iter._iterArray && _iterIndex == iter._iterIndex;
}

template <typename T>
template <typename APTR, typename TREF, typename CTREF, typename TPTR>
inline
bool DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>::operator!=(
    const DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>& iter ) const
{
    return _iterArray != iter._iterArray || _iterIndex != iter._iterIndex;
}

} // End nspc cntr

} // End nspc nge

#endif
