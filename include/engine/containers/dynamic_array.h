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

#include <engine/memory/iallocator.h>

namespace nge
{

namespace cntr
{

template <typename T>
class DynamicArray
{
  private:
    // CONSTANTS
    static constexpr uint32 MIN_CAPACITY = 32;

    // MEMBERS
    /**
     * The allocator.
     */
    // todo: replace with AllocatorGuard, see gdev implementation
    mem::IAllocator* _allocator;

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
     * Shifts the given number of items forward one spot starting at the given
     * index.
     */
    void shiftForward( uint32 start, uint32 count );

    /**
     * Shifts the given number of items backward one spot starting at the given
     * index.
     */
    void shiftBackward( uint32 start, uint32 count );

    /**
     * Wraps the index inside of the circular bounds.
     */
    uint32 wrap( uint32 index ) const;

    /**
     * Wraps the index inside of the old circular bounds.
     */
    uint32 wrapOld( uint32 index ) const;

    /**
     * Transfers some of the old items to the new memory block.
     */
    void transferOld();

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
    DynamicArray( mem::IAllocator* allocator );

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
    DynamicArray( mem::IAllocator* allocator, uint32 capacity );

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
     * Adds the value to the end of the array.
     */
    void push( const T& value );

    /**
     * Adds the value to the front of the array.
     */
    void pushFront( const T& value );

    /**
     * Inserts the value at the given index in the array.
     */
    void insertAt( const T& value, uint32 index );

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
     * Checks if the array contains the given value.
     */
    bool has( const T& value ) const;

    /**
     * Gets the size of the array.
     */
    uint32 size() const;
};

// CONSTANTS

template <typename T>
constexpr uint32 DynamicArray<T>::MIN_CAPACITY;

// CONSTRUCTORS

// OPERATORS

// MEMBER FUNCTIONS

// HELPER FUNCTIONS
template <typename T>
bool DynamicArray<T>::hasMoreToTransfer() const
{
    return _oldSize > 0;
}

} // End nspc cntr

} // End nspc nge

#endif
