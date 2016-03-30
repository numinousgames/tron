// ngudef.h
#ifndef NGE_WRLD_NGUDEF_H
#define NGE_WRLD_NGUDEF_H

namespace nge
{

namespace wrld
{

/**
 * Defines numinous game units.
 */
typedef float ngu;

typedef float MM;

typedef float CM;

typedef float M;

typedef float KM;

constexpr KM operator "" _km( long double value )
{
    return KM( value );
}

} // End nspc wrld

} // End nspc nge

#endif