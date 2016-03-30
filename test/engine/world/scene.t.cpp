// scene.t.cpp
#include <engine/world/scene.h>
#include <gtest/gtest.h>

#include "engine/world/mock_tickable.h"

TEST( Scene, Construction )
{
    using namespace nge::wrld;

    Scene scene;
    Scene copy( scene );
    Scene moved( std::move( scene ) );

    copy = moved;
    copy = std::move( moved );
}

TEST( Scene, TickableManagementAndUpdate )
{
    using namespace nge::wrld;
    using namespace nge::test;

    Scene scene;
    MockTickable mock;

    scene.addTickable( &mock );

    scene.update( 10.0f );

    ASSERT_EQ( 1, mock.preticks() );
    ASSERT_EQ( 1, mock.ticks() );
    ASSERT_EQ( 1, mock.posticks() );
    ASSERT_EQ( 10.0f, mock.elapsed() );

    scene.update( 0.0f );

    ASSERT_EQ( 2, mock.preticks() );
    ASSERT_EQ( 2, mock.ticks() );
    ASSERT_EQ( 2, mock.posticks() );
    ASSERT_EQ( 10.0f, mock.elapsed() );

    scene.removeTickable( &mock );

    mock.reset();

    scene.update( 10.0f );

    ASSERT_EQ( 0, mock.preticks() );
    ASSERT_EQ( 0, mock.ticks() );
    ASSERT_EQ( 0, mock.posticks() );
    ASSERT_EQ( 0.0f, mock.elapsed() );

    scene.addTickable( &mock );

    scene.update( 10.0f );

    ASSERT_EQ( 1, mock.preticks() );
    ASSERT_EQ( 1, mock.ticks() );
    ASSERT_EQ( 1, mock.posticks() );
    ASSERT_EQ( 10.0f, mock.elapsed() );

    scene.removeAll();

    scene.update( 10.0f );

    ASSERT_EQ( 1, mock.preticks() );
    ASSERT_EQ( 1, mock.ticks() );
    ASSERT_EQ( 1, mock.posticks() );
    ASSERT_EQ( 10.0f, mock.elapsed() );
}