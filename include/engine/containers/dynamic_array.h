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
    // CONSTANTS
    /**
     * The minimum array capacity.
     */
    static constexpr uint32 MIN_CAPACITY = 32;

    // MEMBERS
    /**
     * The allocator.
     */
    // todo: replace with AllocatorGuard, see gdev implementation
    mem::AllocatorGuard<T> _allocator;

    /**
     * The array of values.
     */
    T* _values;

    /**
     * The old values from before the array was resized.
     */
    T* _oldValues;

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

    /**
     * The old first item before the array was resized.
     */
    uint32 _oldFirst;

    /**
     * The old size from before the array was resized.
     */
    uint32 _oldSize;

    /**
     * The old capacity from before the array was resized.
     */
    uint32 _oldCapacity;

    /**
     * The next item to be copied in the old array.
     */
    uint32 _oldCopyPos;

    // HELPER FUNCTIONS
    /**
     * Doubles the capacity of the array.
     */
    void grow();

    /**
     * Halves the capacity fo the array.
     */
    void shrink();

    /**
     * Transfers some of the old items to the new memory block.
     */
    void transferOld();

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
     * Wraps the index inside of the old circular bounds.
     */
    uint32 wrapOld( uint32 index ) const;

    /**
     * Determines how many items should be copied.
     */
    uint32 getCopyCount() const;

    /**
     * Checks if the underlying array should grow.
     */
    bool shouldGrow() const;

    /**
     * Checks if the underlying array should shrink.
     */
    bool shouldShrink() const;

    /**
     * Checks if there are still more old items to transfer.
     */
    bool hasMoreToTransfer() const;

  public:
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
     * Removes all items from the array.
     */
    void clear();

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
    : _allocator(), _values( nullptr ), _oldValues( nullptr ), _first( 0 ),
      _size( 0 ), _capacity( MIN_CAPACITY ), _oldFirst( 0 ), _oldSize( 0 ),
      _oldCapacity( 0 ), _oldCopyPos( 0 )
{
    _values = _allocator.get( _capacity );
}

template <typename T>
inline
DynamicArray<T>::DynamicArray( mem::IAllocator<T>* allocator )
    : _allocator( allocator ), _values( nullptr ), _oldValues( nullptr ),
      _first( 0 ), _size( 0 ), _capacity( MIN_CAPACITY ), _oldFirst( 0 ),
      _oldSize( 0 ), _oldCapacity( 0 ), _oldCopyPos( 0 )
{
    _values = _allocator.get( _capacity );
}

template <typename T>
inline
DynamicArray<T>::DynamicArray( uint32 capacity )
    : _allocator(), _values( nullptr ), _oldValues( nullptr ),
      _first( 0 ), _size( 0 ), _capacity( MIN_CAPACITY ), _oldFirst( 0 ),
      _oldSize( 0 ), _oldCapacity( 0 ), _oldCopyPos( 0 )
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
    : _allocator( allocator ), _values( nullptr ), _oldValues( nullptr ),
      _first( 0 ), _size( 0 ), _capacity( MIN_CAPACITY ), _oldFirst( 0 ),
      _oldSize( 0 ), _oldCapacity( 0 ), _oldCopyPos( 0 )
{
    while ( _capacity < capacity )
    {
        _capacity <<= 1;
    }

    _values = _allocator.get( _capacity );
}

template <typename T>
DynamicArray<T>::DynamicArray( const DynamicArray<T>& array )
    : _allocator( array._allocator ), _values( nullptr ),
      _oldValues( nullptr ), _first( 0 ), _size( array._size ),
      _capacity( array._capacity ), _oldFirst( 0 ),
      _oldSize( array._oldSize ), _oldCapacity( array._oldCapacity ),
      _oldCopyPos( array._oldCopyPos )
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

    if ( array._oldValues != nullptr )
    {
        _oldValues = _allocator.get( _oldCapacity );
        a = array._oldCapacity - ( array._oldFirst + array._oldSize );
        b = ( array._oldFirst + array._oldSize ) % array._oldCapacity;

        if ( a >= array._oldSize )
        {
            MemoryUtils::copy( _oldValues, array._oldValues + array._oldFirst,
                  array._oldSize );
        }
        else
        {
            MemoryUtils::copy( _oldValues, array._oldValues + array._oldFirst,
                               a );
            MemoryUtils::copy( _oldValues + a, array._oldValues, b );
        }
    }
}

template <typename T>
DynamicArray<T>::DynamicArray( DynamicArray<T>&& array )
    : _allocator( array._allocator ), _values( nullptr ),
      _oldValues( nullptr ), _first( 0 ), _size( array._size ),
      _capacity( array._capacity ), _oldFirst( 0 ),
      _oldSize( array._oldSize ), _oldCapacity( array._oldCapacity ),
      _oldCopyPos( array._oldCopyPos )
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

    if ( array._oldValues != nullptr )
    {
        _oldValues = _allocator.get( _oldCapacity );
        a = array._oldCapacity - ( array._oldFirst + array._oldSize );
        b = ( array._oldFirst + array._oldSize ) % array._oldCapacity;

        if ( a >= array._oldSize )
        {
            MemoryUtils::move( _oldValues, array._oldValues + array._oldFirst,
                  array._oldSize );
        }
        else
        {
            MemoryUtils::move( _oldValues, array._oldValues + array._oldFirst,
                               a );
            MemoryUtils::move( _oldValues + a, array._oldValues, b );
        }
    }

    array._allocator = nullptr;
    array._values = nullptr;
    array._oldValues = nullptr;
    array._first = 0;
    array._size = 0;
    array._capacity = 0;
    array._oldFirst = 0;
    array._oldSize = 0;
    array._oldCapacity = 0;
    array._oldCopyPos = 0;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    if ( _values != nullptr )
    {
        _allocator.release( _values, _capacity );
    }

    if ( _oldValues != nullptr )
    {
        _allocator.release( _oldValues, _oldCapacity );
    }

    _values = nullptr;
    _oldValues = nullptr;
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
    }

    if ( _oldValues != nullptr )
    {
        _allocator.release( _oldValues, _oldCapacity );
    }

    _allocator = array._allocator;
    _values = nullptr;
    _oldValues = nullptr;
    _first = 0;
    _size = array._size;
    _capacity = array._capacity;
    _oldFirst = 0;
    _oldSize = array._oldSize;
    _oldCapacity = array._oldCapacity;
    _oldCopyPos = array._oldCopyPos;

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

    if ( array._oldValues != nullptr )
    {
        _oldValues = _allocator.get( _oldCapacity );
        a = array._oldCapacity - ( array._oldFirst + array._oldSize );
        b = ( array._oldFirst + array._oldSize ) % array._oldCapacity;

        if ( a >= array._oldSize )
        {
            MemoryUtils::copy( _oldValues, array._oldValues + array._oldFirst,
                  array._oldSize );
        }
        else
        {
            MemoryUtils::copy( _oldValues, array._oldValues + array._oldFirst,
                               a );
            MemoryUtils::copy( _oldValues + a, array._oldValues, b );
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
    _oldValues = nullptr;
    _first = 0;
    _size = array._size;
    _capacity = array._capacity;
    _oldFirst = 0;
    _oldSize = array._oldSize;
    _oldCapacity = array._oldCapacity;
    _oldCopyPos = array._oldCopyPos;

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

    if ( array._oldValues != nullptr )
    {
        _oldValues = _allocator.get( _oldCapacity );
        a = array._oldCapacity - ( array._oldFirst + array._oldSize );
        b = ( array._oldFirst + array._oldSize ) % array._oldCapacity;

        if ( a >= array._oldSize )
        {
            MemoryUtils::move( _oldValues, array._oldValues + array._oldFirst,
                  array._oldSize );
        }
        else
        {
            MemoryUtils::move( _oldValues, array._oldValues + array._oldFirst,
                               a );
            MemoryUtils::move( _oldValues + a, array._oldValues, b );
        }
    }

    array._allocator = nullptr;
    array._values = nullptr;
    array._oldValues = nullptr;
    array._first = 0;
    array._size = 0;
    array._capacity = 0;
    array._oldFirst = 0;
    array._oldSize = 0;
    array._oldCapacity = 0;
    array._oldCopyPos = 0;

    return *this;
}

template <typename T>
inline
const T& DynamicArray<T>::operator[]( uint32 index ) const
{
    assert( index < _size );

    if ( index < _oldSize && index >= _oldCopyPos )
    {
        return _oldValues[wrapOld( index )];
    }

    return _values[wrap( index )];
}

template <typename T>
inline
T& DynamicArray<T>::operator[]( uint32 index )
{
    assert( index < _size );

    if ( index < _oldSize && index >= _oldCopyPos )
    {
        return _oldValues[wrapOld( index )];
    }

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
    if ( index < _oldSize )
    {
        return _oldValues[wrapOld( index )];
    }

    return _values[wrap( index )];
}

template <typename T>
inline
void DynamicArray<T>::push( const T& value )
{
    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

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
    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

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
    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

    if ( shouldGrow() )
    {
        grow();
    }

    --_first;
    ++_size;
    ( *this )[0] = value;
}

template <typename T>
void DynamicArray<T>::pushFront( T&& value )
{
    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

    if ( shouldGrow() )
    {
        grow();
    }

    --_first;
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

    if ( hasMoreToTransfer() )
    {
        transferOld();
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

    if ( hasMoreToTransfer() )
    {
        transferOld();
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

    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

    if ( shouldShrink() )
    {
        shrink();
    }

    const T& ref = ( *this )[_size - 1];
    --_size;
    return ref;
}

template <typename T>
T DynamicArray<T>::popFront()
{
    assert( _size > 0 );

    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

    if ( shouldShrink() )
    {
        shrink();
    }

    const T& ref = ( *this )[0];
    ++_first;
    --_size;
    return ref;
}

template <typename T>
T DynamicArray<T>::removeAt( uint32 index )
{
    assert( _size > 0 );

    if ( index >= _size )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    if ( hasMoreToTransfer() )
    {
        transferOld();
    }

    if ( shouldShrink() )
    {
        shrink();
    }

    T elem = ( *this )[index];
    shiftBackward( index );
    --_size;

    return elem;
}

template <typename T>
void DynamicArray<T>::clear()
{
    _size = 0;
    _first = 0;

    if ( _oldValues != nullptr )
    {
        _allocator.release( _oldValues, _oldCapacity );
        _oldFirst = 0;
        _oldSize = 0;
        _oldCapacity = 9;
        _oldCopyPos = 0;
        _oldValues = nullptr;
    }
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
uint32 DynamicArray<T>::size() const
{
    return _size;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const
{
    return _size <= 0;
}

// HELPER FUNCTIONS
template <typename T>
void DynamicArray<T>::grow()
{
    assert( _oldValues == nullptr );
    _oldValues = _values;
    _oldFirst = _first;
    _oldSize = _size;
    _oldCapacity = _capacity;
    _oldCopyPos = 0;

    _capacity <<= 1;
    _values = _allocator.get( _capacity );
    _first = 0;
}

template <typename T>
void DynamicArray<T>::shrink()
{
    assert( _oldValues == nullptr );
}

template <typename T>
void DynamicArray<T>::transferOld()
{
    const uint32 count = getCopyCount();

    uint32 i;
    for ( i = 0; hasMoreToTransfer() && i < count; ++i, ++_oldCopyPos )
    {
        _values[wrap( _oldCopyPos )] =
            std::move( _oldValues[wrapOld( _oldCopyPos )] );
    }

    if ( !hasMoreToTransfer() )
    {
         _allocator.release( _oldValues, _oldCapacity );
        _oldValues = nullptr;
        _oldFirst = 0;
        _oldSize = 0;
        _oldCapacity = 0;
        _oldCopyPos = 0;
    }
}

template <typename T>
inline
void DynamicArray<T>::shiftForward( uint32 start )
{
    while ( hasMoreToTransfer() )
    {
        transferOld();
    }

    uint32 i;
    for ( i = ( _size - start ) - 1; i != static_cast<uint32>( -1 ); --i )
    {
        _values[wrap( i + 1 )] = std::move( wrap( _values[start + i] ) );
    }
}

template <typename T>
inline
void DynamicArray<T>::shiftBackward( uint32 start )
{
    while ( hasMoreToTransfer() )
    {
        transferOld();
    }

    uint32 i;
    for ( i = 0; i < _size - start; ++i )
    {
        _values[start + i] = std::move( _values[start + i + 1] );
    }
}

template <typename T>
inline
uint32 DynamicArray<T>::wrap( uint32 index ) const
{
    return ( _first + index ) % _capacity;
}

template <typename T>
inline
uint32 DynamicArray<T>::wrapOld( uint32 index ) const
{
    return ( _oldFirst + index ) % _oldCapacity;
}

template <typename T>
inline
uint32 DynamicArray<T>::getCopyCount() const
{
    // Transition  Count   LTH    UTH   ST
    // 32->64        32     16     64   16  64/4
    // 64->128       64     32    128   32  128/4
    // 128->256     128     64    256   64  256/4
    // 256->512     256    128    512  128  512/4
    // 512->1024    512    256   1024  256  1024/4
    // ...
    // 1024->512    256    128    512  128  512/4
    // 512->256     128     64    256   64  256/4
    // 256->128      64     32    128   32  128/4
    // 128->64       32     16     64   16  64/4
    // 64->32        16     --     32    8  32/4
    //
    // LTH = Lower Threshold, UTH = Upper Threshold, ST = Steps Available
    //
    return _size >> 2;
}

template <typename T>
inline
bool DynamicArray<T>::shouldGrow() const
{
    return !hasMoreToTransfer() && _size >= _capacity;
}

template <typename T>
inline
bool DynamicArray<T>::shouldShrink() const
{
    return !hasMoreToTransfer() && _size <= ( _capacity << 2 ) &&
        _capacity > MIN_CAPACITY;
}

template <typename T>
inline
bool DynamicArray<T>::hasMoreToTransfer() const
{
    return _oldCopyPos < _oldSize;
}

} // End nspc cntr

} // End nspc nge

#endif
