//defaultallocator.h
#ifndef NGE_MEM_ALLOCATOR_H
#define NGE_MEM_ALLOCATOR_H

namespace nge
{

namespace mem
{

template <typename T>
class DefaultAllocator : IAllocator<T>
{
    public:
        // CONSTRUCTORS
        /**
        * Constructs the allocator.
        */
        DefaultAllocator();
        
        /**
        * Constructs a copy of an allocator.
        */
        DefaultAllocator( const DefaultAllocator<T>& copy );

        /**
        * Destructs the allocator.
        */
        virtual ~DefaultAllocator();

        // MEMBER FUNCTIONS
        /**
        * Allocates the given number of instances.
        *
        * Behavior is undefined when:
        * T is void
        * count is less than or equal to zero
        * out of memory
        */
        virtual T* get( unsigned int count );

        /**
        * Releases the allocation with the given number of instances.
        *
        * Behavior is undefined when:
        * T is void
        * pointer is invalid
        * count is less than or equal to zero
        * out of memory
        */
        virtual void release( T* pointer, unsigned int count );

        //OPERATORS
        /**
        *Assigns a copy of an allocator.
        */
        DefaultAllocator<T>& operator =( const DefaultAllocator<T>& assign );
};

//CONSTRUCTORS
template <typename T>
inline
DefaultAllocator<T>::DefaultAllocator()
{
}

template<typename T>
inline
DefaultAllocator<T>::~DefaultAllocator()
{
}

template<typename T>
inline
T* DefaultAllocator<T>::get( unsigned int count)
{
    return new T[count];
}

template<typename T>
inline
void DefaultAllocator<T>::release( T* pointer, unsigned int count)
{
    delete[] pointer;
}

//OPERATORS
template<typename T>
inline
DefaultAllocator<T>& DefaultAllocator<T>::operator =( const DefaultAllocator<T>& assign )
{
    return *(this(assign));
}

} // End nspc mem

} // End nspc nge

#endif