// fixed_array.t.cpp
#include <engine/containers/fixed_array.h>
#include <gtest/gtest.h>

TEST( FixedArray, ConstructionAndAssignment )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    DefaultAllocator<uint32> alloc;

    FixedArray<uint32> array( &alloc );
    FixedArray<uint32> copy( array );
    FixedArray<uint32> move( std::move( array ) );
    FixedArray<uint32> capacity( &alloc, static_cast<uint32>( 100 ) );
    FixedArray<uint32> def;

    def = copy;
    def = std::move( copy );
}

TEST( FixedArray, PushAndPop )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 MAX_SIZE = 2048;
    constexpr uint32 SIZE = 1024;

    uint32 i;
    uint32 tmp;

    DefaultAllocator<uint32> alloc;
    FixedArray<uint32> array( &alloc, MAX_SIZE );

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

TEST( FixedArray, At )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 64;

    uint32 i;

    DefaultAllocator<uint32> alloc;
    FixedArray<uint32> array( &alloc, SIZE );

    for ( i = 0; i < SIZE; ++i )
    {
        array.push( i );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        ASSERT_EQ( i, array.at( i ) );
    }

    EXPECT_THROW( array.at( SIZE + 1 ), std::runtime_error );
}

TEST( FixedArray, InsertAndRemove )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 1024;

    uint32 i;
    uint32 tmp;

    DefaultAllocator<uint32> alloc;
    FixedArray<uint32> array( &alloc, SIZE );

    for ( i = 0; i < SIZE; ++i )
    {
        tmp = ( i * 2 ) % ( array.size() + 1 );
        array.insertAt( tmp, i );
        ASSERT_EQ( i, array.at( tmp ) );
    }

    for ( i = SIZE - 1; i != static_cast<uint32>( -1 ); --i)
    {
        tmp = array[( i * 2 ) % array.size()];
        ASSERT_EQ( tmp, array.removeAt( ( i * 2 ) % array.size() ) );
    }
}

TEST( FixedArray, Iterator )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 64;

    uint32 i;

    DefaultAllocator<uint32> alloc;
    FixedArray<uint32> list( &alloc, SIZE );

    for ( i = 0; i < SIZE; ++i )
    {
        list.push( i );
    }

    FixedArray<uint32>::Iterator iter;
    for ( i = 0, iter = list.begin(); iter != list.end(); ++iter, ++i )
    {
        ASSERT_EQ( i, *iter );
    }

    FixedArray<uint32>::Iterator iter2;
    for ( iter = iter2 = list.begin(); iter != list.end(); ++iter, ++iter2 )
    {
        ASSERT_EQ( iter, iter2 );
    }

    for ( i = 63, iter = --list.end(); iter != list.end(); --i, --iter )
    {
        ASSERT_EQ( i, *iter );
    }

    FixedArray<uint32>::ConstIterator citer;
    for ( i = 0, citer = list.cbegin(); citer != list.cend(); ++i, ++citer )
    {
        ASSERT_EQ( i, *citer );
    }

    EXPECT_THROW( list.at( SIZE + 1 ), std::runtime_error );
}