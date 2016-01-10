// iallocator.h
#ifndef NGE_MEM_IALLOCATOR_H
#define NGE_MEM_IALLOCATOR_H

namespace nge
{

namespace mem
{

template <typename T>
class IAllocator
{
  public:
    // CONSTRUCTORS
    /**
     * Destructs the allocator.
     */
    virtual ~IAllocator() = 0;

    // MEMBER FUNCTIONS
    /**
     * Allocates the given number of instances.
     *
     * Behavior is undefined when:
     * T is void
     * count is less than or equal to zero
     * out of memory
     */
    virtual T* get( unsigned int count ) = 0;

    /**
     * Releases the allocation with the given number of instances.
     *
     * Behavior is undefined when:
     * T is void
     * pointer is invalid
     * count is less than or equal to zero
     * out of memory
     */
    virtual void release( T* pointer, unsigned int count ) = 0;
};

// CONSTRUCTORS
template <typename T>
inline
IAllocator<T>::~IAllocator()
{
}

} // End nspc mem

} // End nspc nge

#endif
