//counting_allocator.t.cpp
#include <gtest/gtest.h>
#include <engine/memory/counting_allocator.h>

TEST( CountingAllocator, Construction )
{
    using namespace nge::mem;

    EXPECT_NO_FATAL_FAILURE(
        CountingAllocator<uint32> alloc;
        CountingAllocator<uint32> copy( alloc );
    );
}

TEST( CountingAllocator, Allocation )
{
    using namespace nge::mem;

    uint32* value = nullptr;

    CountingAllocator<uint32> alloc;
    EXPECT_NO_FATAL_FAILURE( value = alloc.get( 1 ) );
    EXPECT_EQ( alloc.getLocalAllocationCount(), 1 );
    EXPECT_EQ( CountingAllocator<uint32>.getGlobalAllocationCount(), 1 );
    EXPECT_NE( nullptr, value );
    EXPECT_NO_FATAL_FAILURE( alloc.release( value, 1 ) );

    EXPECT_DEATH( alloc.release( value, 0 ), ".*" );
    EXPECT_DEATH( alloc.release( nullptr, 1 ), ".*" );

    value = nullptr;

    EXPECT_NO_FATAL_FAILURE( value = alloc.get( 100 ) );
    EXPECT_EQ( alloc.getLocalAllocationCount(), 100 );
    EXPECT_EQ( CountingAllocator<uint32>.getGlobalAllocationCount(), 100 );
    EXPECT_NE( nullptr, value );

    uint32* otherValue = nullptr;

    CountingAllocator<uint32> otherAlloc;
    EXPECT_NO_FATAL_FAILURE( otherValue = alloc.get( 5 ) );
    EXPECT_EQ( otherAlloc.getLocalAllocationCount(), 5 );
    EXPECT_EQ( CountingAllocator<uint32>.getGlobalAllocationCount(), 100 + 5 );
    EXPECT_NO_FATAL_FAILURE( alloc.release( value, 100 ) );
    EXPECT_EQ( CountingAllocator<uint32>.getGlobalAllocationCount(), 5 );
    EXPECT_NO_FATAL_FAILURE( otherAlloc.release( otherValue, 5 ) );
}