// allocator_guard.t.cpp
#include <engine/memory/allocator_guard.h>
#include <gtest/gtest.h>

TEST( AllocatorGuardTest, Construction )
{
    using namespace nge::mem;

    AllocatorGuard<std::string> guard( nullptr );

    EXPECT_NE( nullptr, guard.allocator() );

    DefaultAllocator<std::string> test;

    AllocatorGuard<std::string> guard2( &test );

    EXPECT_EQ( &test, guard2.allocator() );
}

TEST( AllocatorGuardTest, Usage )
{
    using namespace nge::mem;

    DefaultAllocator<std::string> test;

    AllocatorGuard<std::string> guard( &test );

    std::string* val = guard.get( 1 );
    *val = "3dog";

    EXPECT_STREQ( "3dog", val->c_str() );

    guard.release( val, 1 );
}