// port.h
//
// Defines preprocessor constants that provide code portability.
//
#ifndef NGE_PORT_H
#define NGE_PORT_H

/**
 * Visual Studio requires typename before the declaration of any
 * nested templated type variable, a templated classes within a templated
 * class, however, by C++98 standards this is an error. Thus this constant
 * is only defined when building with visual studio.
 *
 * To keep function parameter type declaration lengths from getting out of
 * hand be sure NOT to use the fully qualified type for nested templated
 * classes. For example if using:
 * const DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&y
 * as a parameter in a function definition within the class
 * DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>
 * use the following to declare a variable instead:
 * const ArrayIterator&
 * This will make it such that vc_typename does nto need to be inserted.
 *
 * If you are using vc_typename note that it should appear in the following
 * location:
 * const typename DynamicArray<T>::ArrayIterator<APTR, TREF, CTREF, TPTR>&
 */
#ifndef _MSC_VER
#define vc_typename
#else
#define vc_typename typename
#endif

#endif