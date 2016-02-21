// timer.cpp
#include "engine/utility/timer.h"

#include <assert.h>

namespace nge
{

namespace util
{

// MEMBER FUNCTIONS
void Timer::start()
{
    if ( _hasStarted )
    {
        return;
    }

    _hasStarted = true;
    _isPaused = false;
    _elapsed = HighResTicks( 0 );
    _lastAccumulated = _clock.now();
}

float Timer::pause()
{
    if ( !_hasStarted || _isPaused )
    {
        return 0.0f;
    }

    HighResTime last = _lastAccumulated;
    _lastAccumulated = _clock.now();

    HighResTicks ticks = toTicks( _lastAccumulated - last );

    accumulateTicks( ticks );

    _isPaused = true;

    return toSeconds( ticks ).count();
}

float Timer::lap()
{
    if ( !_hasStarted || _isPaused )
    {
        return 0.0f;
    }

    HighResTime last = _lastAccumulated;
    _lastAccumulated = _clock.now();

    HighResTicks ticks = toTicks( _lastAccumulated - last );

    accumulateTicks( ticks );

    return toSeconds( ticks ).count();
}

void Timer::resume()
{
    if ( !_hasStarted || !_isPaused )
    {
        return;
    }

    _lastAccumulated = _clock.now();
    _isPaused = false;
}

} // End nspc util

} // End nspc nge
