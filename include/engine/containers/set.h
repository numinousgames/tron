// set.h
#ifndef NGE_CNTR_SET_H
#define NGE_CNTR_SET_H

#include <functional>
#include <engine/utility/hasher.h>

#include "engine/intdef.h"
#include "engine/containers/dynamic_array.h"
#include "engine/memory/allocator_guard.h"
#include "list.h"

namespace nge
{

namespace cntr
{

// TODO: consider using progressive bin copy after resize if necessary
// TODO: cache hashes in separate array if necessary
template <typename T>
class Set
{
  public:
    // CLASSES
    /**
     * Defines a constant iterator for the set.
     */
    class ConstIterator
    {
      private:
        // MEMBERS
        /**
         * The set values that are being iterated.
         */
        const DynamicArray<T>* _iterValues;

        /**
         * The current position in the set.
         */
        uint32 _iterIndex;

      public:
        // CONSTRUCTORS
        /**
         * Constructs a new iterator.
         */
        ConstIterator();

        /**
         * Constructs an iterator for the set with the given index.
         */
        ConstIterator( const Set<T>* set, uint32 index );

        /**
         * Constructs a copy of the given iterator.
         */
        ConstIterator( const ConstIterator& iter );

        /**
         * Destructs the iterator.
         */
        ~ConstIterator();

        // OPERATORS
        /**
         * Assigns this as a copy of the other iterator.
         */
        ConstIterator& operator=( const ConstIterator& iter );

        /**
         * Moves to the next item.
         */
        ConstIterator& operator++();

        /**
         * Moves to the next item.
         */
        ConstIterator& operator++( int32 );

        /**
         * Moves to the previous item.
         */
        ConstIterator& operator--();

        /**
         * Moves to the previous item.
         */
        ConstIterator& operator--( int32 );

        /**
         * Gets the element at the current position.
         */
        const T& operator*() const;

        /**
         * Gets the element at the current position.
         *
         * The value must not be modified in a way that it's hash code would
         * change. To do so will cause undefined behavior.
         */
        const T* operator->() const;

        /**
         * Checks if the other iterator is at the same position.
         */
        bool operator==( const ConstIterator& iter ) const;

        /**
         * Checks if the other iterator is not at the same position.
         */
        bool operator!=( const ConstIterator& iter ) const;
    };

  private:
    // CONSTANTS
    /**
     * The minimum number of bins.
     */
    static constexpr uint32 MIN_BINS = 32;

    /**
     * Defines a bin that is empty and does not hold a value.
     */
    static constexpr uint32 BIN_EMPTY = static_cast<uint32>( -1 );

    /**
     * The threshold percentage at which the map grows.
     */
    static constexpr uint32 GROW_THRESHOLD = 70;

    /**
     * The threshold percentage at which the map shrinks.
     */
    static constexpr uint32 SHRINK_THRESHOLD = 30;

    // MEMBERS
    /**
     * The bin allocator.
     */
    mem::AllocatorGuard<uint32> _binAlloc;

    /**
     * The values in the array.
     */
    DynamicArray<T> _values;

    /**
     * The hash function.
     */
    std::function<uint32( const T& )> _hashFunc;

    /**
     * The bins.
     */
    uint32* _bins;

    /**
     * The number of bins currently in use.
     */
    uint32 _binsInUse;

    /**
     * The total number of bins.
     */
    uint32 _binCount;

    // HELPER FUNCTIONS
    /**
     * Gets the index of a bin that should hold the given value.
     *
     * The returned index may already hold that value and needs to be
     * checked that it is empty.
     */
    uint32 findBinForValue( const T& value ) const;

    /**
     * Computes the hash for the given value.
     */
    uint32 hash( const T& value ) const;

    /**
     * Linearly probes for the next position.
     */
    uint32 probe( uint32 probes ) const;

    /**
     * Wraps the bin index to be within the bounds.
     */
    uint32 wrap( uint32 index ) const;

    /**
     * Checks if the bin at the given index is empty.
     *
     * Behavior is undefined when:
     * binIndex is invalid.
     */
    bool isBinEmpty( uint32 binIndex ) const;

    /**
     * Checks if the bin at the given index contains the given value.
     *
     * This will return false if binIndex is invalid.
     */
    bool doesBinContain( uint32 binIndex, const T& value ) const;

    /**
     * Checks if the bin array should shrunk.
     */
    bool shouldShrink() const;

    /**
     * Checks if the bin array should grow.
     */
    bool shouldGrow() const;

    /**
     * Grows the bin array to twice the current capacity.
     */
    void grow();

    /**
     * Shrinks the bin array to half the current capacity.
     */
    void shrink();

    /**
     * Resizes the bin array to the specified size.
     */
    void resize( uint32 newSize );

    /**
     * Resets all of the bins back to empty.
     *
     * This does not reset _binsInUse.
     */
    void clearBins();

  public:
    // CONSTRUCTORS
    /**
     * Constructs a new sset.
     */
    Set();

    /**
     * Constructs a new set with the given initial capacity.
     */
    Set( uint32 capacity );

    /**
     * Constructs a new set that uses the given hash function.
     */
    Set( std::function<uint32( const T& )> hashFunc );

    /**
     * Constructs a new set with the given initial capacity that uses
     * the given hash function.
     */
    Set( uint32 capacity, std::function<uint32( const T& )> hashFunc );

    /**
     * Constructs a new set that uses the given allocators.
     */
    Set( mem::IAllocator<T>* valueAlloc, mem::IAllocator<uint32>* intAlloc );

    /**
     * Constructs a new set using the given allocators and initial capacity.
     */
    Set( mem::IAllocator<T>* valueAlloc, mem::IAllocator<uint32>* intAlloc,
         uint32 capacity );

    /**
     * Constructs a new set using the given allocators and hash function.
     */
    Set( mem::IAllocator<T>* valueAlloc, mem::IAllocator<uint32>* intAlloc,
         std::function<uint32( const T& )> hashFunc );

    /**
     * Constructs a new set using the given allocators, initial capacity, and
     * hash function.
     */
    Set( mem::IAllocator<T>* valueAlloc, mem::IAllocator<uint32>* intAlloc,
         uint32 capacity, std::function<uint32( const T& )> hashFunc );

    /**
     * Constructs a copy of the give nset.
     */
    Set( const Set<T>& set );

    /**
     * Moves the set to a new instance.
     */
    Set( Set<T>&& set );

    /**
     * Destructs the set.
     */
    ~Set();

    // OPERATORS
    /**
     * Assigns this as a copy of the given set.
     */
    Set<T>& operator=( const Set<T>& set );

    /**
     * Moves the set data to this instance.
     */
    Set<T>& operator=( Set<T>&& set );

    /**
     * Gets the item in the set at the given index.
     * <p>
     * Item order preservation is not guaranteed and this should only be
     * used to iterate over all of the items in the set.
     */
    const T& operator[]( uint32 index ) const;

    // MEMBER FUNCTIONS
    /**
     * Adds a copy of the given value to the set.
     *
     * This will not do anything if the value already exists in the set.
     */
    void add( const T& value );

    /**
     * Moves the value into the set.
     *
     * This will not do anything if the value already exists in the set.
     */
    void add( T&& value );

    /**
     * Removes the given value from the set.
     */
    void remove( const T& value );

    /**
     * Checks if the value exists in the set.
     */
    bool has( const T& value ) const;

    /**
     * Removes all of the items from the set.
     */
    void clear();

    /**
     * Gets an iterator for the set.
     */
    ConstIterator cbegin() const;

    /**
     * Gets an iterator for the end of the set.
     */
    ConstIterator cend() const;

    /**
     * Gets the number of items in the set.
     */
    uint32 size() const;

    /**
     * Checks if the set is empty.
     */
    bool isEmpty() const;
};

// CONSTANTS
template <typename T>
constexpr uint32 Set<T>::MIN_BINS;

template <typename T>
constexpr uint32 Set<T>::BIN_EMPTY;

template <typename T>
constexpr uint32 Set<T>::GROW_THRESHOLD;

template <typename T>
constexpr uint32 Set<T>::SHRINK_THRESHOLD;

// CONSTRUCTORS
template <typename T>
inline
Set<T>::Set() : _binAlloc(), _values(), _hashFunc( &util::Hasher<T>::hash ),
                _bins( nullptr ), _binsInUse( 0 ), _binCount( MIN_BINS )
{
    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( uint32 capacity )
    : _binAlloc(), _values( capacity ), _hashFunc( &util::Hasher<T>::hash ),
      _bins( nullptr ), _binsInUse( 0 ), _binCount( MIN_BINS )
{
    while ( _binCount < capacity )
    {
        _binCount <<= 1;
    }

    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( std::function<uint32( const T& )> hashFunc )
    : _binAlloc(), _values(), _hashFunc( hashFunc ), _bins( nullptr ),
      _binsInUse( 0 ), _binCount( MIN_BINS )
{
    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( uint32 capacity, std::function<uint32( const T& )> hashFunc )
    : _binAlloc(), _values( capacity ), _hashFunc( hashFunc ),
      _bins( nullptr ), _binsInUse( 0 ), _binCount( MIN_BINS )
{
    while ( _binCount < capacity )
    {
        _binCount <<= 1;
    }

    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( mem::IAllocator<T>* valueAlloc,
             mem::IAllocator<uint32>* intAlloc )
    : _binAlloc( intAlloc ), _values( valueAlloc ),
      _hashFunc( &util::Hasher<T>::hash ),  _bins( nullptr ),
      _binsInUse( 0 ), _binCount( MIN_BINS )
{
    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( mem::IAllocator<T>* valueAlloc,
             mem::IAllocator<uint32>* intAlloc, uint32 capacity )
    : _binAlloc( intAlloc ), _values( valueAlloc, capacity ),
      _hashFunc( &util::Hasher<T>::hash ), _bins( nullptr ),
      _binsInUse( 0 ), _binCount( MIN_BINS )
{
    while ( _binCount < capacity )
    {
        _binCount <<= 1;
    }

    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( mem::IAllocator<T>* valueAlloc, mem::IAllocator<uint32>* intAlloc,
             std::function<uint32( const T& )> hashFunc )
    : _binAlloc( intAlloc ), _values( valueAlloc ), _hashFunc( hashFunc ),
      _bins( nullptr ), _binsInUse( 0 ), _binCount( MIN_BINS )
{
    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( mem::IAllocator<T>* valueAlloc, mem::IAllocator<uint32>* intAlloc,
             uint32 capacity, std::function<uint32( const T& )> hashFunc )
    : _binAlloc( intAlloc ), _values( valueAlloc, capacity ),
      _hashFunc( hashFunc ), _bins( nullptr ), _binsInUse( 0 ),
      _binCount( MIN_BINS )
{
    while ( _binCount < capacity )
    {
        _binCount <<= 1;
    }

    _bins = _binAlloc.get( _binCount );
    clearBins();
}

template <typename T>
inline
Set<T>::Set( const Set<T>& set )
    : _binAlloc( set._binAlloc ), _values( set._values ),
      _hashFunc( set._hashFunc ), _bins( nullptr ),
      _binsInUse( set._binsInUse ), _binCount( set._binCount )
{
    _bins = _binAlloc.get( _binCount );
    mem::MemoryUtils::copy( _bins, set._bins, _binCount );
}

template <typename T>
inline
Set<T>::Set( Set<T>&& set )
    : _binAlloc( std::move( set._binAlloc ) ),
      _values( std::move( set._values ) ),
      _hashFunc( std::move( set._hashFunc ) ), _bins( set._bins ),
      _binsInUse( set._binsInUse ), _binCount( set._binCount )
{
    set._bins = nullptr;
    set._binsInUse = 0;
    set._binCount = 0;
}

template <typename T>
inline
Set<T>::~Set()
{
    if ( _bins != nullptr )
    {
        _binAlloc.release( _bins, _binCount );
        _bins = nullptr;
    }
    _binsInUse = 0;
    _binCount = 0;
}

// OPERATORS
template <typename T>
inline
Set<T>& Set<T>::operator=( const Set<T>& set )
{
    if ( _bins != nullptr )
    {
        _binAlloc.release( _bins, _binCount );
    }

    _binAlloc = set._binAlloc;
    _values = set._values;
    _hashFunc = set._hashFunc;
    _bins = _binAlloc.get( _binCount );
    _binCount = set._binCount;
    _binsInUse = set._binsInUse;
    mem::MemoryUtils::copy( _bins, set._bins, _binCount );

    return *this;
}

template <typename T>
inline
Set<T>& Set<T>::operator=( Set<T>&& set )
{
    if ( _bins != nullptr )
    {
        _binAlloc.release( _bins, _binCount );
    }

    _binAlloc = std::move( set._binAlloc );
    _values = std::move( set._values );
    _hashFunc = std::move( set._hashFunc );
    _bins = set._bins;
    _binCount = set._binCount;
    _binsInUse = set._binsInUse;

    set._bins = nullptr;
    set._binsInUse = 0;
    set._binCount = 0;

    return *this;
}

template <typename T>
inline
const T& Set<T>::operator[]( uint32 index ) const
{
    assert( index < size() );
    return _values[index];
}

template <typename T>
void Set<T>::add( const T& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    uint32 binIndex = findBinForValue( value );
    if ( isBinEmpty( binIndex ) )
    {
        ++_binsInUse;
        _bins[binIndex] = _values.size();
        _values.push( std::move( value ) );
    }
}

template <typename T>
void Set<T>::add( T&& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    uint32 binIndex = findBinForValue( value );
    if ( isBinEmpty( binIndex ) )
    {
        ++_binsInUse;
        _bins[binIndex] = _values.size();
        _values.push( std::move( value ) );
    }
}

template <typename T>
void Set<T>::remove( const T& value )
{
    if ( shouldShrink() )
    {
        shrink();
    }

    uint32 binIndex = findBinForValue( value );
    uint32 i;
    if ( !isBinEmpty( binIndex ) )
    {
        --_binsInUse;
        _values.removeAt( _bins[binIndex] );

        // correct bin indices
        for ( i = 0; i < _binCount; ++i )
        {
            if ( !isBinEmpty( i ) && _bins[i] > _bins[binIndex] )
            {
                --( _bins[i] );
            }
        }

        _bins[binIndex] = BIN_EMPTY;
    }
}

template <typename T>
inline
bool Set<T>::has( const T& value ) const
{
    const uint32 binIndex = findBinForValue( value );
    return binIndex != -1 && doesBinContain( binIndex, value );
}

template <typename T>
inline
void Set<T>::clear()
{
    _values.clear();
    mem::MemoryUtils::set( _bins, BIN_EMPTY, _binCount );
    _binsInUse = 0;
}

template <typename T>
inline
typename Set<T>::ConstIterator Set<T>::cbegin() const
{
    return ConstIterator( this, 0 );
}

template <typename T>
inline
typename Set<T>::ConstIterator Set<T>::cend() const
{
    return ConstIterator( this, _values.size() );
}

template <typename T>
inline
uint32 Set<T>::size() const
{
    return _values.size();
}

template <typename T>
inline
bool Set<T>::isEmpty() const
{
    return _values.isEmpty();
}

// HELPER FUNCTIONS
template <typename T>
uint32 Set<T>::findBinForValue( const T& value ) const
{
    const uint32 hashCode = hash( value );
    uint32 i;
    uint32 probes;
    for ( i = wrap( hashCode ), probes = 0;
          !isBinEmpty( i ) && !doesBinContain( i, value );
          i = probe( ++probes ) )
    {
        // do nothing
    }

    return i;
}

template <typename T>
inline
uint32 Set<T>::hash( const T& value ) const
{
    return _hashFunc( value );
}

template <typename T>
inline
uint32 Set<T>::probe( uint32 probes ) const
{
    return probes;
}

template <typename T>
inline
uint32 Set<T>::wrap( uint32 index ) const
{
    return index & ( _binCount - 1 );
}

template <typename T>
inline
bool Set<T>::isBinEmpty( uint32 binIndex ) const
{
    assert( binIndex < _binCount );
    return _bins[binIndex] == BIN_EMPTY;
}

template <typename T>
inline
bool Set<T>::doesBinContain( uint32 binIndex, const T& value ) const
{
    return binIndex < _binCount && !isBinEmpty( binIndex ) &&
        hash( _values[_bins[binIndex]] ) == hash( value );
}

template <typename T>
inline
bool Set<T>::shouldShrink() const
{
    return ( ( _binsInUse * 100 ) / _binCount ) <= SHRINK_THRESHOLD &&
        _binCount > MIN_BINS;
}

template <typename T>
inline
bool Set<T>::shouldGrow() const
{
    return ( ( _binsInUse * 100 ) / _binCount ) >= GROW_THRESHOLD;
}

template <typename T>
inline
void Set<T>::grow()
{
    resize( _binCount << 1 );
}

template <typename T>
inline
void Set<T>::shrink()
{
    resize( _binCount >> 1 );
}

template <typename T>
void Set<T>::resize( uint32 newSize )
{
    assert( _bins != nullptr );
    _binAlloc.release( _bins, newSize );
    _bins = _binAlloc.get( newSize );
    _binCount = newSize;
    clearBins();

    uint32 i;
    uint32 pos;
    for ( i = 0; i < _values.size(); ++i )
    {
        pos = findBinForValue( _values[i] );
        _bins[pos] = i;
    }
}

template <typename T>
inline
void Set<T>::clearBins()
{
    mem::MemoryUtils::set( _bins, BIN_EMPTY, _binCount );
}

// ITERATOR CONSTRUCTORS
template <typename T>
inline
Set<T>::ConstIterator::ConstIterator()
    : _iterValues( nullptr ), _iterIndex( 0 )
{
}

template <typename T>
inline
Set<T>::ConstIterator::ConstIterator( const Set<T>* set, uint32 index )
    : _iterValues( &set->_values ), _iterIndex( index )
{
}

template <typename T>
inline
Set<T>::ConstIterator::ConstIterator( const ConstIterator& iter )
    : _iterValues( iter._iterValues ), _iterIndex( iter._iterIndex )
{
}

template <typename T>
inline
Set<T>::ConstIterator::~ConstIterator()
{
    _iterValues = nullptr;
    _iterIndex = static_cast<uint32>( -1 );
}

// ITERATOR OPERATORS
template <typename T>
inline
typename Set<T>::ConstIterator& Set<T>::ConstIterator::operator=(
    const ConstIterator& iter )
{
    _iterValues = iter._iterValues;
    _iterIndex = iter._iterIndex;

    return *this;
}

template <typename T>
inline
typename Set<T>::ConstIterator& Set<T>::ConstIterator::operator++()
{
    ++_iterIndex;

    return *this;
}

template <typename T>
inline
typename Set<T>::ConstIterator& Set<T>::ConstIterator::operator++( int32 )
{
    ++_iterIndex;

    return *this;
}

template <typename T>
inline
typename Set<T>::ConstIterator& Set<T>::ConstIterator::operator--()
{
    _iterIndex = _iterIndex > 0 ? _iterIndex - 1 : _iterValues->size();

    return *this;
}

template <typename T>
inline
typename Set<T>::ConstIterator& Set<T>::ConstIterator::operator--( int32 )
{
    _iterIndex = _iterIndex > 0 ? _iterIndex - 1 : _iterValues->size();

    return *this;
}

template <typename T>
inline
const T& Set<T>::ConstIterator::operator*() const
{
    return ( *_iterValues )[_iterIndex];
}

template <typename T>
inline
const T* Set<T>::ConstIterator::operator->() const
{
    return &( *_iterValues )[_iterIndex];
}

template <typename T>
inline
bool Set<T>::ConstIterator::operator==( const ConstIterator& iter ) const
{
    return _iterValues == iter._iterValues && _iterIndex == iter._iterIndex;
}

template <typename T>
inline
bool Set<T>::ConstIterator::operator!=( const ConstIterator& iter ) const
{
    return _iterValues != iter._iterValues || _iterIndex != iter._iterIndex;
}

} // End nspc cntr

} // End nspc nge

#endif
