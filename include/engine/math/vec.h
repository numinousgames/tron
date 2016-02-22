// vec.h
#ifndef NGE_MATH_VEC_H
#define NGE_MATH_VEC_H

#include "engine/math/vec2.h"
#include "engine/math/vec3.h"
#include "engine/math/vec4.h"
#include "engine/math/vec_math.h"

namespace nge
{

namespace math
{

template <typename T>
class TRef2;

template <typename T>
class TRef3;

template <typename T>
class TRef4;

template <typename T>
class TVec2;

template <typename T>
class TVec3;

template <typename T>
class TVec4;

/**
 * Defines a default 2D vector.
 */
typedef TVec2<float> Vec2;

/**
 * Defines a double 2D vector.
 */
typedef TVec2<double> DVec2;

/**
 * Defines an integer 2D vector.
 */
typedef TVec2<int32> IVec2;

/**
 * Defines a long integer 2D vector.
 */
typedef TVec2<int64> LVec2;

/**
 * Defines a default 3D vector.
 */
typedef TVec3<float> Vec3;

/**
 * Defines a double 3D vector.
 */
typedef TVec3<double> DVec3;

/**
 * Defines an integer 3D vector.
 */
typedef TVec3<int32> IVec3;

/**
 * Defines a long integer 3D vector.
 */
typedef TVec3<int64> LVec3;

/**
 * Defines a default 4D vector.
 */
typedef TVec4<float> Vec4;

/**
 * Defines a double 4D vector.
 */
typedef TVec4<double> DVec4;

/**
 * Defines an integer 2D vector.
 */
typedef TVec4<int32> IVec4;

/**
 * Defines a long integer 2D vector.
 */
typedef TVec4<int64> LVec4;

} // End nspc math

} // End nspc nge

#endif
