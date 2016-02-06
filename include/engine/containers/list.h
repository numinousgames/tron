// list.h
//
// Defines a circularly linked list.
//
#ifndef NGE_CNTR_LIST_H
#define NGE_CNTR_LIST_H

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
class List
{
  public:
    // STRUCTURES
    /**
     * Defines a node in the list.
     */
    struct Node
    {
        uint32 next;
        uint32 prev;
        T value;
    };

  private:
    // CLASSES
    /**
     * Defines an iterator for the list.
     */
    template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
    class SetIterator
    {
      private:
        // MEMBERS
        /**
         * The list that is being iterated.
         */
        LPTR _iterList;

        /**
         * The current index in the internal array.
         *
         * This is the memory location of the current index.
         */
        uint32 _iterIndex;

        /**
         * The current position in the list.
         *
         * This is the current index in the list..
         */
        uint32 _iterPos;

      public:
        // CONSTRUCTORS
        /**
         * Constructs a new iterator.
         */
        SetIterator();

        /**
         * Constructs an iterator for a list at the given internal index and
         * list position.
         */
        SetIterator( LPTR list, uint32 index, uint32 pos );

        /**
         * Constructs a copy of the given iterator.
         */
        SetIterator( const SetIterator& iter );

        /**
         * Destructs the iterator.
         */
        ~SetIterator();

        // OPERATORS
        /**
         * Assigns this as a copy of the other iterator.
         */
        SetIterator& operator=( const SetIterator& iter );

        /**
         * Moves to the next item.
         */
        SetIterator& operator++();

        /**
         * Moves to the next item.
         */
        SetIterator& operator++( int32 );

        /**
         * Moves to the previous item.
         */
        SetIterator& operator--();

        /**
         * Moves to the previous item.
         */
        SetIterator& operator--( int32 );

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
        bool operator==( const SetIterator& iter ) const;

        /**
         * Checks if the other iterator is not at the same position.
         */
        bool operator!=( const SetIterator& iter ) const;
    };

    // CONSTANTS
    /**
     * The minimum internal array capacity
     */
    static constexpr uint32 MIN_CAPACITY = 32;

    // MEMBERS
    /**
     * The node allocator.
     */
    mem::AllocatorGuard<Node> _alloc;

    /**
     * The internal list representation.
     * <p>
     * This array contains both the used and free items. There are two linked
     * lists within the array. The first links all of the items that are in
     * use. The second includes all items that are free. When the number of
     * items in use becomes less than 1/4 of the total items then the list
     * will be reallocated.
     */
    Node* _nodes;

    /**
     * The index of the first node in the list.
     */
    uint32 _first;

    /**
     * The number of nodes in use.
     * <p>
     * This translates to the size of the accessible list.
     */
    uint32 _count;

    /**
     * The index of the first free item in the array.
     */
    uint32 _firstFree;

    /**
     * The number of free items in the free list.
     */
    uint32 _freeCount;

    /**
     * The internal array capacity.
     */
    uint32 _capacity;

    // HELPER FUNCTIONS
    /**
     * Removes a free node from the front of the free list and gets
     * its index.
     * <p>
     * Behavior is undefined when:
     * there are no free nodes
     */
    uint32 popFreeNodeAndGetPos();

    /**
     * Inserts the node at the given position.
     */
    void insertAtPos( uint32 index, Node& node );

    /**
     * Pushes the node at the given index to the free list.
     */
    void pushFree( uint32 index );

    /**
     * Halves the size of the internal array.
     */
    void shrink();

    /**
     * Doubles the size of the internal array.
     */
    void grow();

    /**
     * Resizes the internal array to the specified size.
     */
    void resize( uint32 size );

    /**
     * Gets the node at the given index.
     */
    Node& getNodeAt( uint32 index ) const;

    /**
     * Gets the internal index of the given index.
     * <p>
     * This is designed such that it will iterate through at most
     * N/2 items changing the direction it iterates depending on the index.
     * <p>
     * TODO: check performnce impact due to cache incoherency
     */
    uint32 getNodePos( uint32 index ) const;

    /**
     * Checks if the internal array should be halved in size.
     */
    bool shouldShrink() const;

    /**
     * Checks if the internal array should be doubled in size.
     */
    bool shouldGrow() const;

    /**
     * Checks if there is a free node.
     */
    bool hasFree() const;

  public:
    // TYPES
    /**
     * Defines an iterator for the list.
     */
    typedef SetIterator<List<T>*, T&, const T&, T*> Iterator;

    /**
     * Defines a constant iterator for the list.
     */
    typedef SetIterator<const List<T>*, const T&, const T&, const T*>
        ConstIterator;

    // CONSTRUCTORS
    /**
     * Constructs a new list.
     */
    List();

    /**
     * Constructs a new list with the given initial capacity.
     */
    List( uint32 capacity );

    /**
     * Constructs a new list using the given allocator.
     */
    List( mem::IAllocator<Node>* alloc );

    /**
     * Constructs a new list using the given allocator and capacity.
     */
    List( mem::IAllocator<Node>* alloc, uint32 capacity );

    /**
     * Constructs a copy of the given list.
     */
    List( const List<T>& list );

    /**
     * Moves the data in the given list to a new instance.
     */
    List( List<T>&& list );

    /**
     * Destructs the list.
     */
    ~List();

    // OPERATORS
    /**
     * Assigns this as a copy of the other list.
     */
    List<T>& operator=( const List<T>& list );

    /**
     * Moves the data from the other list to this instance.
     */
    List<T>& operator=( List<T>&& list );

    /**
     * Gets the value at the given index.
     */
    const T& operator[]( uint32 index ) const;

    /**
     * Gets the value at the given index.
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
     * Gets an iterator at the start of the list.
     */
    Iterator begin();

    /**
     * Gets a constant iterator at the start of the list.
     */
    ConstIterator cbegin() const;

    /**
     * Gets an iterator at the end of the list.
     */
    Iterator end();

    /**
     * Gets a constant iterator at the end of the list.
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
constexpr uint32 List<T>::MIN_CAPACITY;

// CONSTRUCTORS
template <typename T>
inline
List<T>::List() : _alloc(), _nodes( nullptr ), _first( 0 ), _count( 0 ),
                  _firstFree( 0 ), _freeCount( 0 ), _capacity( MIN_CAPACITY )
{
    _nodes = _alloc.get( _capacity );
}

template <typename T>
inline
List<T>::List( uint32 capacity )
    : _alloc(), _nodes( nullptr ), _first( 0 ), _count( 0 ),
      _firstFree( 0 ), _freeCount( 0 ), _capacity( MIN_CAPACITY )
{
    while ( _capacity < capacity )
    {
        _capacity <<= 1;
    }

    _nodes = _alloc.get( _capacity );
}

template <typename T>
inline
List<T>::List( mem::IAllocator<Node>* alloc )
    : _alloc( alloc ), _nodes( nullptr ), _first( 0 ), _count( 0 ),
      _firstFree( 0 ), _freeCount( 0 ), _capacity( MIN_CAPACITY )
{
    _nodes = _alloc.get( _capacity );
}

template <typename T>
inline
List<T>::List( mem::IAllocator<Node>* alloc, uint32 capacity )
    : _alloc( alloc ), _nodes( nullptr ), _first( 0 ), _count( 0 ),
      _firstFree( 0 ), _freeCount( 0 ), _capacity( MIN_CAPACITY )
{
    while ( _capacity < capacity )
    {
        _capacity <<= 1;
    }

    _nodes = _alloc.get( _capacity );
}

template <typename T>
inline
List<T>::List( const List<T>& list )
    : _alloc( list._alloc ), _nodes( nullptr ),
      _first( 0 ), _count( list._count ),
      _firstFree( 0 ), _freeCount( 0 ),
      _capacity( MIN_CAPACITY )
{
    uint32 i;

    while( _capacity < _count )
    {
        _capacity <<= 1;
    }

    _nodes = _alloc.get( _capacity );

    if ( list._nodes != nullptr && _count > 0 )
    {
        // copy the items in order
        _nodes[0].value = list[0];
        _nodes[0].prev = _count - 1;
        _nodes[0].next = 1 % _count;
        for ( i = 1; i < _count; ++i )
        {
            _nodes[i].value = list[i];
            _nodes->next = ( i + 1 ) % _count;
            _nodes->prev = i - 1;
        }
    }
}

template <typename T>
inline
List<T>::List( cntr::List<T>&& list )
    : _alloc( list._alloc ), _nodes( nullptr ),
      _first( 0 ), _count( list._count ),
      _firstFree( 0 ), _freeCount( 0 ),
      _capacity( MIN_CAPACITY )
{
    uint32 i;

    while( _capacity < _count )
    {
        _capacity <<= 1;
    }

    _nodes = _alloc.get( _capacity );

    if ( list._nodes != nullptr && _count > 0 )
    {
        // copy the items in order
        _nodes[0].value = std::move( list[0] );
        _nodes[0].prev = _count - 1;
        _nodes[0].next = 1 % _count;
        for ( i = 1; i < _count; ++i )
        {
            _nodes[i].value = std::move( list[i] );
            _nodes->next = ( i + 1 ) % _count;
            _nodes->prev = i - 1;
        }
    }

    // release source
    if ( list._nodes != nullptr )
    {
        list._alloc.release( list._nodes, list._capacity );
    }

    list._nodes = nullptr;
    list._first = 0;
    list._count = 0;
    list._firstFree = 0;
    list._freeCount = 0;
    list._capacity = 0;
}

template <typename T>
inline
List<T>::~List()
{
    if ( _nodes != nullptr )
    {
        _alloc.release( _nodes, _capacity );
    }
    _nodes = nullptr;
    _first = 0;
    _count = 0;
    _firstFree = 0;
    _freeCount = 0;
    _capacity = 0;
}

// OPERATORS
template <typename T>
inline
List<T>& List<T>::operator=( const List<T>& list )
{
    uint32 i;

    if ( _nodes != nullptr )
    {
        _alloc.release( _nodes, _capacity );
        _nodes = nullptr;
    }

    // create minimally size internal array
    _alloc = list._alloc;
    _first = 0;
    _count = list._count;
    _firstFree = 0;
    _freeCount = 0;
    _capacity = MIN_CAPACITY;

    while( _capacity < _count )
    {
        _capacity <<= 1;
    }

    _nodes = _alloc.get( _capacity );

    if ( list._nodes != nullptr && _count > 0 )
    {
        // copy the items in order
        _nodes[0].value = list[0];
        _nodes[0].prev = _count - 1;
        _nodes[0].next = 1 % _count;

        for ( i = 1; i < _count; ++i )
        {
            _nodes[i].value = list[i];
            _nodes->next = ( i + 1 ) % _count;
            _nodes->prev = i - 1;
        }
    }

    return *this;
}

template <typename T>
inline
List<T>& List<T>::operator=( cntr::List<T>&& list )
{
    uint32 i;

    if ( _nodes != nullptr )
    {
        _alloc.release( _nodes, _capacity );
        _nodes = nullptr;
    }

    // create minimally size internal array
    _alloc = list._alloc;
    _first = 0;
    _count = list._count;
    _firstFree = 0;
    _freeCount = 0;
    _capacity = MIN_CAPACITY;

    while( _capacity < _count )
    {
        _capacity <<= 1;
    }

    _nodes = _alloc.get( _capacity );

    if ( list._nodes != nullptr && _count > 0 )
    {
        // copy the items in order
        _nodes[0].value = std::move( list[0] );
        _nodes[0].prev = _count - 1;
        _nodes[0].next = 1 % _count;

        for ( i = 1; i < _count; ++i )
        {
            _nodes[i].value = std::move( list[i] );
            _nodes->next = ( i + 1 ) % _count;
            _nodes->prev = i - 1;
        }
    }

    // release source
    if ( list._nodes != nullptr )
    {
        list._alloc.release( list._nodes, list._capacity );
    }

    list._nodes = nullptr;
    list._first = 0;
    list._count = 0;
    list._firstFree = 0;
    list._freeCount = 0;
    list._capacity = 0;

    return *this;
}

template <typename T>
inline
const T& List<T>::operator[]( uint32 index ) const
{
    assert( index < _count );
    return getNodeAt( index ).value;
}

template <typename T>
inline
T& List<T>::operator[]( uint32 index )
{
    assert( index < _count );
    return getNodeAt( index ).value;
}

// MEMBER FUNCTIONS
template <typename T>
inline
T& List<T>::at( uint32 index ) const
{
    if ( index >= _count )
    {
        throw std::runtime_error( "Index is out of bounds!" );
    }

    return getNodeAt( index ).value;
}

template <typename T>
inline
void List<T>::push( const T& value )
{
    insertAt( _count, value );
}

template <typename T>
inline
void List<T>::push( T&& value )
{
    insertAt( _count, value );
}

template <typename T>
inline
void List<T>::pushFront( const T& value )
{
    insertAt( 0, value );
}

template <typename T>
inline
void List<T>::pushFront( T&& value )
{
    insertAt( 0, value );
}

template <typename T>
inline
void List<T>::insertAt( uint32 index, const T& value )
{
    Node node;
    node.value = value;
    insertAtPos( index, node );
}

template <typename T>
inline
void List<T>::insertAt( uint32 index, T&& value )
{
    Node node;
    node.value = std::move( value );
    insertAtPos( index, node );
}

template <typename T>
inline
T List<T>::pop()
{
    assert( _count > 0 );
    return removeAt( _count - 1 );
}

template <typename T>
inline
T List<T>::popFront()
{
    assert( _count > 0 );
    return removeAt( 0 );
}

template <typename T>
T List<T>::removeAt( uint32 index )
{
    assert( _count > 0 );
    assert( index < _count );

    if ( shouldShrink() )
    {
        shrink();
    }

    const uint32 pos = getNodePos( index );
    const Node& node = _nodes[pos];

    if ( _count > 0 && _first == pos )
    {
        _first = node.next;
    }

    _nodes[node.prev].next = node.next;
    _nodes[node.next].prev = node.prev;

    pushFree( pos );

    --_count;
    return node.value;
}

template <typename T>
inline
void List<T>::clear()
{
    _first = 0;
    _count = 0;
    _firstFree = 0;
    _freeCount = 0;
}

template <typename T>
inline
typename List<T>::Iterator List<T>::begin()
{

    return Iterator( this, _first, 0 );
}

template <typename T>
inline
typename List<T>::ConstIterator List<T>::cbegin() const
{

    return ConstIterator( this, _first, 0 );
}

template <typename T>
inline
typename List<T>::Iterator List<T>::end()
{
    return Iterator( this, _first, _count );
}

template <typename T>
inline
typename List<T>::ConstIterator List<T>::cend() const
{
    return ConstIterator( this, _first, _count );
}

template <typename T>
uint32 List<T>::indexOf( const T& value ) const
{
    if ( _count <= 0 )
    {
        return static_cast<uint32>( -1 );
    }

    uint32 ret;
    uint32 pos;
    uint32 steps;
    for ( pos =  _first, steps = 0, ret = static_cast<uint32>( -1 );
          ret == static_cast<uint32>( -1 ) && steps > 0 && pos != _first;
          pos = _nodes[_first].next, ++steps )
    {
        if ( _nodes[pos].value == value )
        {
            ret = pos;
        }
    }

    return ret;
}

template <typename T>
bool List<T>::has( const T& value ) const
{
    if ( _count <= 0 )
    {
        return false;
    }

    uint32 pos;
    uint32 steps;
    bool found;
    for ( pos =  _first, steps = 0, found = false;
          !found && steps > 0 && pos != _first;
          pos = _nodes[_first].next, ++steps )
    {
        found = _nodes[pos].value == value;
    }

    return found;
}

template <typename T>
inline
uint32 List<T>::size() const
{
    return _count;
}

template <typename T>
inline
bool List<T>::isEmpty() const
{
    return _count <= 0;
}

// HELPER FUNCTIONS
template <typename T>
uint32 List<T>::popFreeNodeAndGetPos()
{
    assert( _freeCount > 0 );

    uint32 pos = _firstFree;
    Node& node = _nodes[pos];

    if ( --_freeCount <= 0 )
    {
        _firstFree = 0;
        return pos;
    }

    _nodes[node.prev].next = node.next;
    _nodes[node.next].prev = node.prev;
    _firstFree = node.next;

    return pos;
}

template <typename T>
void List<T>::insertAtPos( uint32 index, Node& node )
{
    assert( index <= _count );

    if ( shouldGrow() )
    {
        grow();
    }

    uint32 pos = hasFree() ? popFreeNodeAndGetPos() : _count;

    // insert node differently depending on position
    if ( _count > 0 && index == _count )
    {
        node.prev = _nodes[_first].prev;
        node.next = _first;

        _nodes[_nodes[_first].prev].next = pos;
        _nodes[_first].prev = pos;
    }
    else if ( _count <= 0 )
    {
        node.prev = pos;
        node.next = pos;
    }
    else
    {
        uint32 target = getNodePos( index );
        node.next = target;
        node.prev = _nodes[target].prev;

        _nodes[node.prev].next = pos;
        _nodes[node.next].prev = pos;
    }

    if ( index <= 0 )
    {
        _first = pos;
    }

    _nodes[pos] = std::move( node );
    ++_count;
}

template <typename T>
void List<T>::pushFree( uint32 index )
{
    if ( !hasFree() )
    {
        _nodes[index].prev = index;
        _nodes[index].next = index;
        _firstFree = index;
    }
    else
    {
        _nodes[_firstFree].prev = index;
        _nodes[_nodes[_firstFree].prev].next = index;
    }

    ++_freeCount;
}

template <typename T>
inline
void List<T>::grow()
{
    resize( _capacity << 1 );
}

template <typename T>
inline
void List<T>::shrink()
{
    resize( _capacity >> 1 );
}

template <typename T>
void List<T>::resize( uint32 size )
{
    Node* newList = _alloc.get( size );

    // copy only the items that are in use to the new array
    uint32 i;
    Node* cur = &getNodeAt( 0 );
    newList[0].value = std::move( cur->value );
    newList[0].prev = _count - 1;
    newList[0].next = 1;

    for ( i = 1, cur = &_nodes[cur->next];
          i < _count - 1;
          ++i, cur = &_nodes[cur->next] )
    {
        newList[i].value = std::move( cur->value );
        newList[i].next = i + 1;
        newList[i].prev = i - 1;
    }

    newList[_count - 1].value = std::move( cur->value );
    newList[_count - 1].next = 0;
    newList[_count - 1].prev = _count - 2;

    _first = 0;
    _firstFree = 0;
    _freeCount = 0;

    _alloc.release( _nodes, _capacity );
    _capacity = size;
    _nodes = newList;
}

template <typename T>
inline
typename List<T>::Node& List<T>::getNodeAt( uint32 index ) const
{
    return _nodes[getNodePos( index )];
}

template <typename T>
inline
uint32 List<T>::getNodePos( uint32 index ) const
{
    assert( index < _count );
    uint32 cur;

    // navigate to item in the optimal direction
    if ( index <= _count / 2 )
    {
        for ( cur = _first; index > 0; --index )
        {
            cur = _nodes[cur].next;
        }
    }
    else
    {
        index = _count - index;
        for ( cur = _first; index > 0; --index )
        {
            cur = _nodes[cur].prev;
        }
    }
    return cur;
}

template <typename T>
inline
bool List<T>::shouldGrow() const
{
    return _count >= _capacity;
}

template <typename T>
inline
bool List<T>::shouldShrink() const
{
    return _count <= ( _capacity / 4 ) && _capacity > MIN_CAPACITY;
}

template <typename T>
inline
bool List<T>::hasFree() const
{
    return _freeCount > 0;
}

// ITERATOR CONSTRUCTORS
template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::SetIterator()
    : _iterList( nullptr ), _iterIndex( 0 ), _iterPos( 0 )
{
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::SetIterator(
    LPTR list, uint32 index, uint32 pos )
    : _iterList( list ), _iterIndex( index ), _iterPos( pos )
{
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::SetIterator(
    const SetIterator& iter )
    : _iterList( iter._iterList ), _iterIndex( iter._iterIndex ),
      _iterPos( iter._iterPos )
{
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::~SetIterator()
{
    _iterList = nullptr;
    _iterIndex = static_cast<uint32>( -1 );
    _iterPos = static_cast<uint32>( -1 );
}

// ITERATOR OPERATORS
template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>&
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator=(
    const List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>& iter )
{
    _iterList = iter._iterList;
    _iterIndex = iter._iterIndex;
    _iterPos = iter._iterPos;

    return *this;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>&
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator++()
{
    _iterIndex = _iterList->_nodes[_iterIndex].next;
    ++_iterPos;

    return *this;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>&
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator++( int32 )
{
    _iterIndex = _iterList->_nodes[_iterIndex].next;
    ++_iterPos;

    return *this;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>&
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator--()
{
    _iterIndex = _iterList->_nodes[_iterIndex].prev;
    _iterPos = _iterPos > 0 ? _iterPos - 1 : _iterList->_count;

    return *this;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>&
List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator--( int32 )
{
    _iterIndex = _iterList->_nodes[_iterIndex].prev;
    _iterPos = _iterPos > 0 ? _iterPos - 1 : static_cast<uint32>( -1 );

    return *this;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
CTREF List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator*() const
{
    return _iterList->_nodes[_iterIndex].value;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
TREF List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator*()
{
    return _iterList->_nodes[_iterIndex].value;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
TPTR List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator->() const
{
    return &( _iterList->_nodes[_iterIndex].value );
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
TPTR List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator->()
{
    return &( _iterList->_nodes[_iterIndex].value );
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
bool List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator==(
    const SetIterator<LPTR, TREF, CTREF, TPTR>& iter ) const
{
    return _iterList == iter._iterList && _iterPos == iter._iterPos;
}

template <typename T>
template <typename LPTR, typename TREF, typename CTREF, typename TPTR>
inline
bool List<T>::SetIterator<LPTR, TREF, CTREF, TPTR>::operator!=(
    const SetIterator<LPTR, TREF, CTREF, TPTR>& iter ) const
{
    return _iterList != iter._iterList || _iterPos != iter._iterPos;
}

} // End nspc cntr

} // End nspc nge

#endif