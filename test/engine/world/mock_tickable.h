// mock_tickable.h
#ifndef NGE_MOCK_TICKABLE_H
#define NGE_MOCK_TICKABLE_H

#include <engine/world/itickable.h>
#include <engine/intdef.h>

namespace nge
{

namespace test
{

class MockTickable : public wrld::ITickable
{
  private:
    /**
     * The number of times pretick() was called.
     */
    uint32 _preticks;

    /**
     * The number of times tick( float ) was called.
     */
    uint32 _ticks;

    /**
     * The number of times postick() was called.
     */
    uint32 _posticks;

    /**
     * The total amount of elapsed time.
     */
    float _elapsed;

  public:
    // CONSTRUCTORS
    /**
     * Constructs a new mock tickable.
     */
    MockTickable();

    /**
     * Constructs a copy of the mock tickable.
     */
    MockTickable( const MockTickable& mock );

    /**
     * Destructs the mock tickable.
     */
    ~MockTickable();

    // OPERATORS
    /**
     * Assigns this as a copy of the mock tickable.
     */
    MockTickable& operator=( const MockTickable& mock );

    // ACCESSOR FUNCTIONS
    /**
     * Gets the number of times pretick() was called.
     */
    uint32 preticks() const;

    /**
     * Gets the number of times tick( float ) was called.
     */
    uint32 ticks() const;

    /**
     * Gets the number of times postick() was called.
     */
    uint32 posticks() const;

    /**
     * Gets the total elapsed time.
     */
    float elapsed() const;

    // MEMBER FUNCTIONS
    /**
     * Performs a pretick.
     */
    virtual void pretick();

    /**
     * Performs a tick.
     */
    virtual void tick( float dtS );

    /**
     * Performs a postick.
     */
    virtual void postick();

    /**
     * Resets the tickable.
     */
    void reset();
};

// CONSTRUCTORS
inline
MockTickable::MockTickable() : _preticks( 0 ), _ticks( 0 ), _posticks( 0 ),
                               _elapsed( 0.0f )
{
}

inline
MockTickable::MockTickable( const MockTickable& mock )
    : _preticks( mock._preticks ), _ticks( mock._ticks ),
      _posticks( mock._posticks ), _elapsed( mock._elapsed )
{
}

inline
MockTickable::~MockTickable()
{
}

// OPERATORS
inline
MockTickable& MockTickable::operator=( const MockTickable& mock )
{
    _preticks = mock._preticks;
    _ticks = mock._ticks;
    _posticks = mock._posticks;
    _elapsed = mock._elapsed;

    return *this;
}

// ACCESSOR FUNCTIONS
inline
uint32 MockTickable::preticks() const
{
    return _preticks;
}

inline
uint32 MockTickable::ticks() const
{
    return _ticks;
}

inline
uint32 MockTickable::posticks() const
{
    return _posticks;
}

inline
float MockTickable::elapsed() const
{
    return _elapsed;
}

// MEMBER FUNCTIONS
inline
void MockTickable::pretick()
{
    ++_preticks;
}

inline
void MockTickable::tick( float dtS )
{
    ++_ticks;
    _elapsed += dtS;
}

inline
void MockTickable::postick()
{
    ++_posticks;
}

inline
void MockTickable::reset()
{
    _preticks = 0;
    _ticks = 0;
    _posticks = 0;
    _elapsed = 0.0f;
}


} // End nspc test

} // End nspc nge

#endif
