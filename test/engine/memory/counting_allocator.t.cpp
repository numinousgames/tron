//counting_allocator.t.cpp
#include <gtest/gtest.h>
#include <engine/memory/counting_allocator.h>

TEST( CountingAllocator, Construction )
{
    using namespace nge::mem;
    using namespace nge;

    EXPECT_NO_FATAL_FAILURE(
        CountingAllocator<uint32> alloc;
        CountingAllocator<uint32> copy( alloc );
    );
}

TEST( CountingAllocator, Allocation )
{
    using namespace nge::mem;
    using namespace nge;

    uint32* value = nullptr;

    CountingAllocator<uint32> alloc;
    EXPECT_NO_FATAL_FAILURE( value = alloc.get( 1 ) );
    EXPECT_EQ( alloc.getAllocationCount(), 1 );
    EXPECT_EQ( CountingAllocator<uint32>::getGlobalAllocationCount(), 1 );
    EXPECT_NE( nullptr, value );
    EXPECT_NO_FATAL_FAILURE( alloc.release( value, 1 ) );

    EXPECT_DEATH( alloc.release( value, 1 ), ".*" );
    EXPECT_DEATH( alloc.release( value, 0 ), ".*" );
    EXPECT_DEATH( alloc.release( nullptr, 1 ), ".*" );

    value = nullptr;

    EXPECT_NO_FATAL_FAILURE( value = alloc.get( 100 ) );
    EXPECT_EQ( alloc.getAllocationCount(), 100 );
    EXPECT_EQ( CountingAllocator<uint32>::getGlobalAllocationCount(), 100 );
    EXPECT_NE( nullptr, value );

    uint32* otherValue = nullptr;

    CountingAllocator<uint32> otherAlloc;
    EXPECT_NO_FATAL_FAILURE( otherValue = otherAlloc.get( 5 ) );
    EXPECT_EQ( otherAlloc.getAllocationCount(), 5 );
    EXPECT_EQ( CountingAllocator<uint32>::getGlobalAllocationCount(), 105 );
    EXPECT_NO_FATAL_FAILURE( alloc.release( value, 100 ) );
    EXPECT_EQ( CountingAllocator<uint32>::getGlobalAllocationCount(), 5 );
    EXPECT_NO_FATAL_FAILURE( otherAlloc.release( otherValue, 5 ) );
}