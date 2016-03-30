//map.h

#ifndef NGE_CNTR_MAP_H
#define NGE_CNTR_MAP_H

#include <functional>
#include <engine/utility/hasher.h>

#include "engine/intdef.h"
#include "engine/memory/allocator_guard.h"
#include "dynamic_array.h"
#include "list.h"

namespace nge
{
    
namespace cntr
{
    
template <typename K, typename V>
class Map
{
  public:
    // STRUCTURES
    struct KVPair
    {
        K key;
        V value;
    };

    // CLASSES
    /**
     * Defines a constant iterator for the map.
     */
    class ConstIterator
    {
      private:
        // MEMBERS
        /**
         * The map key-value pairs that are being iterated.
         */
        const DynamicArray<KVPair>* _iterValues;

        /**
         * The current position in the map.
         */
        uint32 _iterIndex;

      public:
        // CONSTRUCTORS
        /**
         * Constructs a new iterator.
         */
        ConstIterator();

        /**
         * Constructs an iterator for the map with the given index.
         */
        ConstIterator( const Map<K, V>* map, uint32 index );

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
        const KVPair& operator*() const;

        /**
         * Gets the element at the current position.
         *
         * The value must not be modified in a way that it's hash code would
         * change. To do so will cause undefined behavior.
         */
        const KVPair* operator->() const;

        /**
         * Checks if the other iterator is at the same position.
         */
        bool operator==( const ConstIterator& iter ) const;

        /**
         * Checks if the other iterator is not at the same position.
         */
        bool operator!=( const ConstIterator& iter ) const;
    };

    // CONSTRUCTORS
    /**
     * Constructs a new map.
     */
    Map();

     /**
      * Constructs a new map with the given intial capacity.
      */
    Map( uint32 capacity );

    /**
     * Constructs a new map with the given hash function.
     */
    Map( std::function<uint32( const K& )> hashFunc );

    /**
     * Constructs a new map with the given initial capacity that uses
     * the given hash function.
     */
    Map( uint32 capacity, std::function<uint32( const K& )> hashFunc );

    /**
     * Constructs a new map with the given allocators.
    */
    Map( mem::IAllocator<KVPair>* pairAlloc, mem::IAllocator<uint32>* intAlloc );

    /**
     * Constructs a new map with the given allocators and initial capacity.
    */
    Map( mem::IAllocator<KVPair>* pairAlloc, mem::IAllocator<uint32>* intAlloc,
         uint32 capacity );

    /**
     * Constructs a new map with the given allocators and hash function.
    */
    Map( mem::IAllocator<KVPair>* pairAlloc, mem::IAllocator<uint32>* intAlloc,
         std::function<uint32( const K& )> hashFunc );

    /**
     * Constructs a new map using the given allocators, initial capacity, and
     * hash function.
    */
     Map( mem::IAllocator<KVPair>* pairAlloc, mem::IAllocator<uint32>* intAlloc,
          uint32 capacity, std::function<uint32( const K& )> hashFunc );

    /**
     * Constructs a copy of the given map
    */
    Map( const Map<K, V>& map );

    /**
     * Moves the map to a new instance.
    */
    Map( Map<K, V>&& map );

    /**
     * Destructs the map.
    */
    ~Map();

    // OPERATORS
    /**
     * Assigns this as a copy of the given map.
    */
    Map<K, V>& operator=( const Map<K, V>& map );

    /**
     * Moves the map data to this instance.
    */
    Map<K, V>& operator=( const Map<K, V>&& map );

    // MEMBER FUNCTIONS
    /**
     * assigns a copy of the given value to the given key in the map.
     *
     * assigns will replace any already existing value associated with the key.
     */
    void assign( const K& key, const V& value );

    /**
     * Sets a copy of the given value to the given key in the map.
     *
     * This will replace any already existing value associated with the key.
     */
    void assign( K&& key, V&& value );

    /**
     * Removes the given key and its associated value from the map.
     */
    void remove( const K& key );

    /**
     * Checks if the key exists in the map.
     */
    bool hasKey( const K& key ) const;

    /**
    * Checks if the value exists in the map.
    */
    bool hasValue( const V& value ) const;

    /**
     * Removes all of the items from the map.
     */
    void clear();

    /**
     * Gets the number of items in the map.
     */
    uint32 size() const;

    /**
     * Checks if the map is empty.
     */
    bool isEmpty() const;

    /**
     * Gets an iterator for the map
     */
    ConstIterator cbegin() const;

    /**
     * Gets an iterator for the end of the map.
     */
    ConstIterator cend() const;

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
    /*
    * Key-Value pair array
    */
    DynamicArray<KVPair> _pairs;

    /**
     * The bin allocator.
     */
    mem::AllocatorGuard<uint32> _binAlloc;

    /**
     * The hash function.
     */
    std::function<uint32( const K& )> _hashFunc;

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
    uint32 findBinForKey( const K& key ) const;

    /**
     * Computes the hash for the given key.
     */
    uint32 hash( const K& key ) const;

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
     * Checks if the bin at the given index contains the given key.
     *
     * This will return false if binIndex is invalid.
     */
    bool doesBinContain( uint32 binIndex, const K& key ) const;

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
};

// CONSTANTS
template<typename K, typename V>
constexpr uint32 Map<K>::MIN_BINS;

template<typename K, typename V>
constexpr uint32 Map<K>::BIN_EMPTY;

template<typename K, typename V>
constexpr uint32 Map<K>::GROW_THRESHOLD;

template<typename K, typename V>
constexpr uint32 Map<K>::SHRINK_THRESHOLD;



// CONSTRUCTORS
template<typename K, typename V>
inline
Map<K, V>::Map() : Map( MIN_BINS )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( uint32 capacity ) : Map( capcity, &util::Hasher<K>::hash )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( std::function<uint32( const K& )> hashFunc )
    : Map ( MIN_BINS, hashFunc )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( uint32 capacity, std::function<uint32( const K& )> hashFunc )
    : Map( _pairs(), _bins( nullptr ), capacity, hashFunc )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( mem::IAllocator<KVPair>* pairAlloc,
                mem::IAllocator<uint32>* intAlloc )
    : Map( pairAlloc, intAlloc, MIN_BINS )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( mem::IAllocator<KVPair>* pairAlloc,
                mem::IAllocator<uint32>* intAlloc, uint32 capacity )
    : Map( pairAlloc, intAlloc, capacity, &util::Hasher<K>::hash )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( mem::IAllocator<KVPair>* pairAlloc,
                mem::IAllocator<uint32>* intAlloc,
                std::function<uint32( const K& )> hashFunc )
    : Map( pairAlloc, intAlloc, MIN_BINS, hashFunc )
{
}

template<typename K, typename V>
inline
Map<K, V>::Map( mem::IAllocator<KVPair>* pairAlloc,
                mem::IAllocator<uint32>* intAlloc,
                uint32 capacity, std::function<uint32( const K& )> hashFunc )
    : _binAlloc( intAlloc ), _pairs( pairAlloc, capacity ),
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

template<typename K, typename V>
inline
Map<K, V>::Map( const Map<K, V>& map )
    : _binAlloc( map._binAlloc ), _pairs( map._pairs ),
      _hashFunc( map._hashFunc ), _bins( nullptr ),
      _binsInUse( map._binsInUse ), _binCount( map._binCount )
{
    _bins = _binAlloc.get( _binCount );
    mem::MemoryUtils::copy( _bins, map._bins, _binCount );
}

template<typename K, typename V>
inline
Map<K, V>Map( Map<K, V>&& map )
    : _binAlloc( std::move( map._binAlloc ) ),
      _pairs( std::move( map._pairs ) ),
      _hashFunc( std::move( map._hashFunc ) ), _bins( map._bins ),
      _binsInUse( map._binsInUse ), _binCount( map._binCount )
{
    map._bins = nullptr;
    map._binsInUse = 0;
    map._binCount = 0;
}

template<typename K, typename V>
inline
Map<K, V>~Map()
{
    if ( _bins != nullptr )
    {
        _binAlloc.release( _bins, _binCount );
        _bins = nullptr;
    }
    _binsInUse = 0;
    _binCount = 0;
}

// ITERATOR CONSTRUCTORS
template <typename K, typename V>
inline
Map<K, V>::ConstIterator::ConstIterator()
    : _iterValues( nullptr ), _iterIndex( 0 )
{
}

template <typename K, typename V>
inline
Map<K, V>::ConstIterator::ConstIterator( const Map<K, V>* map, uint32 index )
    : _iterValues( &map->_pairs ), _iterIndex( index )
{
}

template <typename K, typename V>
inline
Map<K, V>::ConstIterator::ConstIterator( const ConstIterator& iter )
    : _iterValues( iter._iterValues ), _iterIndex( iter._iterIndex )
{
}

template <typename K, typename V>
inline
Map<K, V>::ConstIterator::~ConstIterator()
{
    _iterValues = nullptr;
    _iterIndex = static_cast<uint32>( -1 );
}

// OPERATORS
template<typename K, typename V>
inline
Map<K, V>& Map<K, V>::operator=( const Map<K, V>& map )
{
    if ( _bins != nullptr )
    {
        _binAlloc.release( _bins, _binCount );
    }

    _binAlloc = map._binAlloc;
    _pairs = map._pairs;
    _hasFunc = map._hashFunc;
    _bins = _binAlloc.get( _binCount );
    _binCount = map._binCount;
    _binsInUse = map._binsInUse;
    mem::MemoryUtils::copy( _bins, map._bins, _binCount );

    return *this;
}

template <typename K, typename V>
inline
Map<K, V>& Map<K, V>::operator=( Map<K, V>&& map )
{
    if ( _bins != nullptr )
    {
        _binAlloc.release( _bins, _binCount );
    }

    _binAlloc = std::move( map.binAlloc );
    _pairs = std::move( map._pairs );
    _hashFunc = std::move( map._hashFunc );
    _bins = map._bins;
    _binCount = map._binCount;
    _binsInUse = map._binsInUse;

    map._bins = nullptr;
    map._binsInUse = 0;
    map._binCount = 0;

    return *this;
}

// ITERATOR OPERATORS
template <typename K, typename V>
inline
typename Map<K, V>::ConstIterator& Map<K, V>::ConstIterator::operator=(
    const ConstIterator& iter)
{
    _iterValues = iter._iterValues;
    _iterIndex = iter._iterIndex;

    return *this;
}

template <typename K, typename V>
inline
typename Map<K, V>::Constiterator& Map<K, V>::ConstIterator::operator++()
{
    ++_iterIndex;

    return *this;
}

template <typename K, typename V>
inline
typename Map<K, V>::Constiterator& Map<K, V>::ConstIterator::operator--()
{
    _iterIndex = _iterIndex > 0 ? _iterIndex - 1 : _iterValues->size();

    return *this;
}

template <typename K, typename V>
inline
typename Map<K, V>::Constiterator& Map<K, V>::ConstIterator::operator--( int32 )
{
    _iterIndex = _iterIndex > 0 ? _iterIndex - 1 : _iterValues ->size();

    return *this;
}

template <typename K, typename V>
inline
const typename Map<K, V>::KVPair& Map<K, V>::ConstIterator::operator*() const
{
    return ( *_iterValues )[_iterIndex];
}

template <typename K, typename V>
inline
const typename Map<K, V>::KVPair* Map<K, V>::ConstIterator::operator->() const
{
    return &( *_iterValues )[_iterIndex];
}

template <typename K, typename V>
inline
bool Map<K, V>::ConstIterator::operator==( const ConstIterator& iter ) const
{
    return _iterValues == iter._iterValues && _iterIndex == iter._iterIndex;
}

template <typename K, typename V>
inline
bool Map<K, V>::ConstIterator::operator!=( const ConstIterator& iter ) const
{
    return _iterValues != iter._iterValues || _iterIndex != iter._iterIndex;
}

// MEMBER FUNCTIONS
template <typename K, typename V>
inline
void Map<K, V>::assign( const K& key, const V& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    uint32 binIndex = findBinForKey( key );
    if ( isBinEmpty( binIndex ) )
    {
        ++_binsInUse;
        _bins[binIndex] = _pairs.size();
        _pairs.push( KVPair( std::move( key ), std::move( value ) ) );
    }
    else
    {
        _pairs[binIndex].value = std::move( value );
    }
}

template <typename K, typename V>
inline
void Map<K, V>::assign ( K&& key, V&& value )
{
    if ( shouldGrow() )
    {
        grow();
    }

    uint32 binIndex = findBinForKey( key );
    if ( isBinEmpty( binIndex ) )
    {
        ++_binsInUse;
        _bins[binIndex] = _pairs.size();
        _pairs.push( KVPair( std::move( key ), std::move( value ) ) );
    }
    else
    {
        _pairs[binIndex].value = std::move( value );
    }
}

template <typename K, typename V>
inline
void Map<K, V>::remove( const K& key )
{
    if ( shouldShrink() )
    {
        shrink();
    }

    uint32 binIndex = findBinForKey( value );
    uint32 i;
    if ( !isBinEmpty( binIndex ) )
    {
        --_binsInUse;
        _pairs.removeAt( _bins[binIndex] );

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

template <typename K, typename V>
inline
bool Map<K, V>::hasKey( const K& key ) const
{
    const uint32 binIndex = findBinForKey( key );
    return binIndex != -1 && doesBinContain( binIndex, key );
}

template <typename K, typename V>
inline
bool Map<K, V>::hasValue( const V& value ) const
{
    for (ConstIterator iter = cbegin(); iter != cend(); ++iter)
    {
        if (iter->value == value)
        {
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
inline
bool Map<K, V>::isEmpty()
{
    return _pairs.isEmpty();
}

template <typename K, typename V>
inline
uint32 Map<K, V>::size()
{
    return _values.size();
}

template <typename K, typename V>
inline
void Map<K, V>::clear()
{
    _pairs.clear();
    mem::MemoryUtils::map( _bins, BIN_EMPTY, _binCount );
    _binsInUse = 0;
}

template <typename K, typename V>
inline
typename Map<K, V>::ConstIterator Map<K, V>::cbegin() const
{
    return ConstIterator( this, 0 );
}

template <typename K, typename V>
inline
typename Map<K, V>::ConstIterator Map<K, V>::cend() const
{
    return ConstIterator( this, _pairs.size() );
}

// HELPER FUNCTIONS
template <typename K, typename V>
inline
uint32 Map<K, V>::findBinForKey( const T& key ) const
{
    const uint32 hashCode = hash( key );
    uint32 i;
    uint32 probes;
    for ( i = wrap( hashCode ), probes = 0;
          !isBinEmpty( i ) && !doesBinContain( i, key );
          i = wrap( i + probe( ++probes ) ) )
    {
        // do nothing
    }

    return i;
}

template<typename K, typename V>
inline
uint32 Map<K, V>::hash( const K& key ) const
{
    return _hashFunc( key );
}

template<typename K, typename V>
inline
uint32 Map<K, V>::probe( uint32 probes ) const
{
    return probes;
}

template<typename K, typename V>
inline
uint32 Map<K, V>::wrap( uint32 index ) const
{
    return index & ( _binCount - 1 );
}

template<typename K, typename V>
inline
bool Map<K, V>::isBinEmpty( uint32 binIndex ) const
{
    assert( binIndex < _binCount );
    return _bins[binIndex] == BIN_EMPTY;
}

template <typename K, typename V>
inline
bool Map<K, V>::doesBinContain( uint32 binIndex, const K& key ) const
{
    return binIndex < _binCount && !isBinEmpty( binIndex ) &&
        hash( _pairs[_bins[binIndex]] ) == hash( key );
}

template<typename K, typename V>
inline
bool Map<K, V>::shouldShrink() const
{
    return ( ( _binsInUse * 100 ) / _binCount ) <= SHRINK_THRESHOLD &&
        _binCount > MIN_BINS;
}

template<typename K, typename V>
inline
bool Map<K, V>::shouldGrow() const
{
    return ( ( _binsInUse * 100 ) / _binCount ) >= GROW_THRESHOLD;
}

template <typename K, typename V>
inline
void Map<K, V>::grow()
{
    resize( _binCount << 1 );
}

template <typename K, typename V>
inline
void Map<K, V>::shrink()
{
    resize( _binCount >> 1 );
}

template <typename K, typename V>
inline
void Map<K, V>::resize( uint32 newSize )
{
    assert( _bins != nullptr );
    _binAlloc.release( _bins, newSize );
    _bins = _binAlloc.get( newSize );
    _binCount = newSize;
    clearBins();

    uint32 i;
    uint32 pos;
    for ( i = 0; i < _pairs.size(); ++i )
    {
        pos = findBinForKey( _pairs[i] );
        _bins[pos] = i;
    }
}

template <typename K, typename V>
inline
void Map<K, V>::clearBins()
{
    mem::MemoryUtils::map( _bins, BIN_EMPTY, _binCount );
}


} // end nspc cntr

} // end nspc nge

#endif