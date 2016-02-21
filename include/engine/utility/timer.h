// clock.h
#ifndef NGE_UTIL_TIMER_H
#define NGE_UTIL_TIMER_H

#include <chrono>

#include "engine/intdef.h"

namespace nge
{

namespace util
{

class Timer
{
  private:
    // CONSTANTS
    /**
     * Defines the high resolution timer resolution.
     *
     * This is equal the number of ticks per second.
     */
    static constexpr int64 RES = 1000000000;

    // TYPES
    /**
     * High resolution clock.
     */
    typedef std::chrono::high_resolution_clock HighResClock;

    /**
     * High resolution clock time reading.
     */
    typedef std::chrono::high_resolution_clock::time_point HighResTime;

    /**
     * High resolution ticks.
     *
     * This forces a time ratio to create consistency between implementations.
     */
    // TODO: determine what ratio I want to use
    typedef std::chrono::duration<uint64, std::ratio<1, RES>> HighResTicks;

    /**
     * Duration in seconds.
     */
    typedef std::chrono::duration<float> Seconds;

    // MEMBERS
    /**
     * The underlying high resolution clock.
     */
    HighResClock _clock;

    /**
     * The time the clock accumulated.
     */
    HighResTime _lastAccumulated;

    /**
     * The accumulated ticks since the start.
     */
    HighResTicks _elapsed;

    /**
     * The current time scale.
     */
    float _timeScale;

    /**
     * If the clock is paused.
     */
    bool _isPaused;

    /**
     * If the clock has started.
     */
    bool _hasStarted;

    // HELPER FUNCTIONS
    /**
     * Accumulates the given duration.
     *
     * This does not consider the pause state.
     */
    void accumulateTicks( HighResTicks dt );

    /**
     * Converts the given duration to seconds.
     */
    template <typename T, int64 R1, int64 R2>
    Seconds toSeconds(
        std::chrono::duration<T, std::ratio<R1, R2>> duration ) const;

    /**
     * Converts the given duration to high resolution ticks.
     */
    template <typename T, int64 R1, int64 R2>
    HighResTicks toTicks(
        std::chrono::duration<T, std::ratio<R1, R2>> duration ) const;

  public:
    // CONSTRUCTORS
    /**
     * Constructs a new timer.
     */
    Timer();

    /**
     * Constructs a copy of a timer.
     */
    Timer( const Timer& timer );

    /**
     * Destructs the timer.
     */
    ~Timer();

    // OPERATORS
    /**
     * Assigns this as a copy of the timer.
     */
    Timer& operator=( const Timer& timer );

    // ACCESSOR FUNCTIONS
    /**
     * Gets the current time scale.
     */
    float timeScale() const;

    /**
     * Checks if the clock is paused.
     */
    bool isPaused() const;

    // MUTATOR FUNCTIONS
    /**
     * Sets the time scale.
     */
    void setTimeScale( float scale );

    // MEMBER FUNCTIONS
    /**
     * Starts the timer.
     *
     * If called after stop() this resets the timer.
     */
    void start();

    /**
     * Pauses the clock.
     *
     * This returns the number of seconds elapsed since the last lap.
     */
    float pause();

    /**
     * Gets the time elapsed in seconds since the last lap.
     *
     * This returns zero if the timer is paused or hasn't been started.
     */
    float lap();

    /**
     * Gets the time elapsed in seconds since the last lap but doesn't perform
     * a lap.
     */
    float sinceLap() const;

    /**
     * Gets the time elapsed in seconds since the timer was started.
     *
     * This will also account for the elapsed time since the last lap but
     * will not update the internal accumulator.
     */
    float elapsed() const;

    /**
     * Gets the ticks elapsed since the timer was started.
     * The ticks are time scaled.
     *
     * This will also account for the elapsed time since the last lap but
     * will not update the internal accumulator
     */
    uint64 elapsedTicks() const;

    /**
     * Gets the total accumulated time since the timer was started.
     *
     * This will not account for the elapsed time since the last lap.
     */
    float total() const;

    /**
     * Gets the total number of accumulated ticks.
     * The ticks are time scaled.
     *
     * This will not account for the elapsed time since the last lap.
     */
    uint64 totalTicks() const;

    /**
     * Resumes the clock.
     */
    void resume();

    /**
     * Stops the timer.
     *
     * After this is called start() must be called which will
     * reset the timer. However, until it is called the elapsed time will
     * be maintained.
     */
    void stop();
};

// CONSTRUCTORS
inline
Timer::Timer() : _clock(), _lastAccumulated(), _elapsed( 0 ),
                 _timeScale( 1.0f ), _isPaused( false ), _hasStarted( false )
{
}

inline
Timer::Timer( const Timer& timer )
    : _clock( timer._clock ), _lastAccumulated( timer._lastAccumulated ),
      _elapsed( timer._elapsed ), _timeScale( timer._timeScale ),
      _isPaused( timer._isPaused ), _hasStarted( timer._hasStarted )
{
}

inline
Timer::~Timer()
{
}

// OPERATORS
inline
Timer& Timer::operator=( const Timer& timer )
{
    _clock = timer._clock;
    _lastAccumulated = timer._lastAccumulated;
    _elapsed = timer._elapsed;
    _timeScale = timer._timeScale;
    _isPaused = timer._isPaused;
    _hasStarted = timer._hasStarted;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
float Timer::timeScale() const
{
    return _timeScale;
}

inline
bool Timer::isPaused() const
{
    return _isPaused;
}

// MUTATOR FUNCTIONS
inline
void Timer::setTimeScale( float scale )
{
    _timeScale = scale;
}

// MEMBER FUNCTIONS
inline
float Timer::sinceLap() const
{
    if ( !_hasStarted || _isPaused )
    {
        return 0.0f;
    }

    return toSeconds( ( _clock.now() - _lastAccumulated ) * _timeScale )
        .count();
}

inline
float Timer::elapsed() const
{
    if ( !_hasStarted || _isPaused )
    {
        return toSeconds( _elapsed ).count();
    }

    return toSeconds( _elapsed + toTicks(
        toSeconds( _clock.now() - _lastAccumulated ) * _timeScale ) ).count();
}

inline
uint64 Timer::elapsedTicks() const
{
    if ( !_hasStarted || _isPaused )
    {
        return _elapsed.count();
    }

    return ( _elapsed + toTicks(
        toSeconds( _clock.now() - _lastAccumulated ) * _timeScale ) ).count();
}

inline
float Timer::total() const
{
    return toSeconds( _elapsed ).count();
}

inline
uint64 Timer::totalTicks() const
{
    return _elapsed.count();
}

inline
void Timer::stop()
{
    _hasStarted = false;
}

// HELPER FUNCTIONS
inline
void Timer::accumulateTicks( HighResTicks dt )
{
    _elapsed += toTicks( _timeScale * toSeconds( dt ) );
}

template <typename T, int64 R1, int64 R2>
inline
Timer::Seconds Timer::toSeconds(
    std::chrono::duration<T, std::ratio<R1, R2>> duration ) const
{
    return std::chrono::duration_cast<Seconds>( duration );
}

template <typename T, int64 R1, int64 R2>
inline
Timer::HighResTicks Timer::toTicks(
    std::chrono::duration<T, std::ratio<R1, R2>> duration ) const
{
    return std::chrono::duration_cast<HighResTicks>( duration );
}

} // End nspc util

} // End nspc nge

#endif
