// default_allocator.t.cpp
#include <gtest/gtest.h>
#include <engine/memory/default_allocator.h>

TEST( DefaultAllocator, Construction )
{
    using namespace nge::mem;

    EXPECT_NO_FATAL_FAILURE(
        DefaultAllocator<std::string> alloc;
        DefaultAllocator<std::string> copy( alloc );
    );
}

TEST( DefaultAllocator, Allocation )
{
    using namespace nge::mem;

    std::string* value = nullptr;

    DefaultAllocator<std::string> alloc;

    EXPECT_NO_FATAL_FAILURE( value = alloc.get( 1 ) );
    EXPECT_NE( nullptr, value );
    EXPECT_NO_FATAL_FAILURE( alloc.release( value, 1 ) );

    EXPECT_DEATH( alloc.release( nullptr, 1 ), ".*" );
    EXPECT_DEATH( alloc.release( value, 0 ), ".*" );

    value = nullptr;

    EXPECT_NO_FATAL_FAILURE( value = alloc.get( 100 ) );
    EXPECT_NE( nullptr, value );
    EXPECT_NO_FATAL_FAILURE( alloc.release( value, 100 ) );
}