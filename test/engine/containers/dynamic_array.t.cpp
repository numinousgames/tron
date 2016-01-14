// dynamic_array.t.cpp
#include <engine/containers/dynamic_array.h>
#include <gtest/gtest.h>

TEST( DynamicArray, ConstructionAndAssignment )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    DefaultAllocator<int32> alloc;

    DynamicArray<int32> array( &alloc );
    DynamicArray<int32> copy( array );
    DynamicArray<int32> capacity( &alloc, static_cast<uint32>( 100 ) );
    DynamicArray<int32> def;

    def = copy;
}

TEST( DynamicArray, PushAndPop )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 1024;

    int32 i;
    int32 tmp;

    DefaultAllocator<int32> alloc;
    DynamicArray<int32> array( &alloc );

    // push
    array.push( 0 );
    EXPECT_EQ( 0,  array[0] );

    array.push( 32 );
    EXPECT_EQ( 32, array[1] );

    array.clear();
    EXPECT_EQ( 0, array.size() );
    EXPECT_TRUE( array.isEmpty() );

    for ( i = 0; i < SIZE; ++i )
    {
        array.push( i + 12 );
        ASSERT_EQ( i + 12, array[array.size() - 1] );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        ASSERT_EQ( i + 12, array[i] );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        array.pushFront( i + 69 );
        ASSERT_EQ( i + 69,  array[0] );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        ASSERT_EQ( i + 69, array[SIZE - i - 1] );
    }

    EXPECT_EQ( SIZE * 2, array.size());

    // pop
    while ( array.size() > 0 )
    {
        i = array.size();

        tmp = array[array.size() - 1];
        ASSERT_EQ( tmp, array.pop() );

        tmp = array[0];
        ASSERT_EQ( tmp, array.popFront() );

        ASSERT_EQ( i - 2, array.size() );
    }

    // mix
    for ( i = 0; i < SIZE; ++i )
    {
        array.pushFront( i + 69 );
        ASSERT_EQ( i + 69, array[0] );
    }

    array.popFront();
    array.push( 10 );
    EXPECT_EQ( 10,  array[array.size() - 1] );
}

TEST( DynamicArray, At )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    int32 i;

    DefaultAllocator<int32> alloc;
    DynamicArray<int32> array( &alloc );

    for ( i = 0; i < 64; ++i )
    {
        array.push( i );
    }

    for ( i = 0; i < 64; ++i )
    {
        ASSERT_EQ( i, array.at( i ) );
    }

    EXPECT_THROW( array.at( 65 ), std::runtime_error );
}

TEST( DynamicArray, InsertAndRemove )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 1024;

    uint32 i;
    uint32 tmp;

    DefaultAllocator<uint32> alloc;
    DynamicArray<uint32> array( &alloc );

    // force wrap
    array.push( 0 );
    array.popFront();
    array.push( 0 );

    for ( i = 0; i < SIZE; ++i )
    {
        tmp = ( i * 2 ) % ( array.size() + 1 );
        array.insertAt( tmp, i );
        ASSERT_EQ( i, array.at( tmp ) );
    }

    for ( i = 0; i < SIZE; ++i)
    {
        tmp = array[( i * 2 ) % array.size()];
        ASSERT_EQ( tmp, array.removeAt( ( i * 2 ) % array.size() ) );
    }
}