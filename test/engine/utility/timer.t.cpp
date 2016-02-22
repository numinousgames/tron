// timer.t.cpp
#include <engine/utility/timer.h>
#include <gtest/gtest.h>

TEST( Timer, Construction )
{
    using namespace nge::util;

    Timer timer;
    Timer copy( timer );

    timer = copy;
}

TEST( Timer, StartAndStop )
{
    using namespace nge::util;

    Timer timer;

    ASSERT_EQ( 0.0f, timer.elapsed() );
    ASSERT_EQ( 0, timer.elapsedTicks() );

    timer.start();

    usleep( 1000 );

    EXPECT_TRUE( timer.elapsed() > 0.001f );
    EXPECT_TRUE( timer.elapsedTicks() > 1000000 );

    timer.stop();

    usleep( 1000 );

    EXPECT_TRUE( timer.elapsed() > 0.001f );
    EXPECT_TRUE( timer.elapsedTicks() > 1000000 );
    EXPECT_FALSE( timer.elapsed() > 0.002f );
    EXPECT_FALSE( timer.elapsedTicks() > 2000000 );

    timer.start();
    timer.stop();

    EXPECT_TRUE( timer.elapsed() < 0.001f );
    EXPECT_TRUE( timer.elapsedTicks() < 1000000 );
}

TEST( Timer, PauseAndResumeAndLapAndTotal )
{
    using namespace nge::util;

    Timer timer;

    ASSERT_EQ( 0.0f, timer.elapsed() );
    ASSERT_EQ( 0.0f, timer.elapsedTicks() );
    ASSERT_EQ( 0.0f, timer.lap() );

    timer.start();

    usleep( 1000 );

    EXPECT_TRUE( timer.elapsed() > 0.001f );
    EXPECT_TRUE( timer.elapsedTicks() > 1 );

    timer.pause();

    usleep( 1000 );

    EXPECT_TRUE( timer.elapsed() > 0.001f );
    EXPECT_TRUE( timer.elapsedTicks() > 1000000 );
    EXPECT_FALSE( timer.elapsed() > 0.002f );
    EXPECT_FALSE( timer.elapsedTicks() > 2000000 );

    timer.resume();

    usleep( 1000 );

    EXPECT_TRUE( timer.elapsed() > 0.002f );
    EXPECT_TRUE( timer.elapsedTicks() > 2000000 );

    usleep( 1000 );

    EXPECT_FALSE( timer.total() > 0.002f );
    EXPECT_FALSE( timer.totalTicks() > 2000000 );
    EXPECT_TRUE( timer.elapsed() > 0.002f );
    EXPECT_TRUE( timer.elapsedTicks() > 2000000 );

    timer.lap();

    EXPECT_TRUE( timer.total() > 0.002f );
    EXPECT_TRUE( timer.totalTicks() > 2000000 );
    EXPECT_TRUE( timer.elapsed() > 0.002f );
    EXPECT_TRUE( timer.elapsedTicks() > 2000000 );

    timer.stop();
}

TEST( Timer, TimeScale )
{
    using namespace nge::util;

    Timer timer;

    ASSERT_EQ( 1.0f, timer.timeScale() );

    timer.setTimeScale( 2.0f );
    timer.start();

    usleep( 1000 );

    EXPECT_TRUE( timer.elapsed() > 0.002f );
    EXPECT_TRUE( timer.elapsedTicks() > 2000000 );

    timer.stop();
    timer.setTimeScale( 0.5f );
    timer.start();

    EXPECT_FALSE( timer.elapsed() > 0.001f );
    EXPECT_FALSE( timer.elapsedTicks() > 1000000 );

    timer.stop();
}