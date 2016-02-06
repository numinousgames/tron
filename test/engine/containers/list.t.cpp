// list.t.cpp
#include <engine/containers/list.h>
#include <gtest/gtest.h>

TEST( List, ConstructionAndAssignment )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    DefaultAllocator<List<uint32>::Node> alloc;

    List<uint32> capacity( 128 );
    List<uint32> list( &alloc );
    List<uint32> copy( list );
    List<uint32> move( std::move( list ) );
    List<uint32> capacityWithAlloc( &alloc, 128 );
    List<uint32> def;

    def = copy;
    def = std::move( copy );
}

TEST( List, PushAndPop )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 1024;

    uint32 i;
    uint32 tmp;

    DefaultAllocator<List<uint32>::Node> alloc;
    List<uint32> list( &alloc );

    // push
    list.push( 0 );
    EXPECT_EQ( 0,  list[0] );

    list.push( 32 );
    EXPECT_EQ( 32, list[1] );

    list.clear();
    EXPECT_EQ( 0, list.size() );
    EXPECT_TRUE( list.isEmpty() );

    for ( i = 0; i < SIZE; ++i )
    {
        list.push( i + 12 );
        ASSERT_EQ( i + 12, list[list.size() - 1] );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        ASSERT_EQ( i + 12, list[i] );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        list.pushFront( i + 69 );
        ASSERT_EQ( i + 69,  list[0] );
    }

    for ( i = 0; i < SIZE; ++i )
    {
        ASSERT_EQ( i + 69, list[SIZE - i - 1] );
    }

    EXPECT_EQ( SIZE * 2, list.size());

    // pop
    while ( list.size() > 0 )
    {
        i = list.size();

        tmp = list[list.size() - 1];
        ASSERT_EQ( tmp, list.pop() );

        tmp = list[0];
        ASSERT_EQ( tmp, list.popFront() );

        ASSERT_EQ( i - 2, list.size() );
    }

    // mix
    for ( i = 0; i < SIZE; ++i )
    {
        list.pushFront( i + 69 );
        ASSERT_EQ( i + 69, list[0] );
    }

    list.popFront();
    list.push( 10 );
    EXPECT_EQ( 10,  list[list.size() - 1] );
}

TEST( List, At )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    uint32 i;

    DefaultAllocator<List<uint32>::Node> alloc;
    List<uint32> list( &alloc );

    for ( i = 0; i < 64; ++i )
    {
        list.push( i );
    }

    for ( i = 0; i < 64; ++i )
    {
        ASSERT_EQ( i, list.at( i ) );
    }

    EXPECT_THROW( list.at( 65 ), std::runtime_error );
}

TEST( List, InsertAndRemove )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    constexpr uint32 SIZE = 1024;

    uint32 i;
    uint32 tmp;

    DefaultAllocator<List<uint32>::Node> alloc;
    List<uint32> list( &alloc );

    // force wrap
    list.push( 0 );
    list.popFront();
    list.push( 0 );

    for ( i = 0; i < SIZE; ++i )
    {
        tmp = ( i * 2 ) % ( list.size() + 1 );
        list.insertAt( tmp, i );
        ASSERT_EQ( i, list.at( tmp ) );
    }

    for ( i = 0; i < SIZE; ++i)
    {
        tmp = list[( i * 2 ) % list.size()];
        ASSERT_EQ( tmp, list.removeAt( ( i * 2 ) % list.size() ) );
    }
}

TEST( List, Iterator )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;

    uint32 i;

    DefaultAllocator<List<uint32>::Node> alloc;
    List<uint32> list( &alloc );

    for ( i = 0; i < 64; ++i )
    {
        list.push( i );
    }

    List<uint32>::Iterator iter;
    for ( i = 0, iter = list.begin(); iter != list.end(); ++iter, ++i )
    {
        ASSERT_EQ( i, *iter );
    }

    List<uint32>::Iterator iter2;
    for ( iter = iter2 = list.begin(); iter != list.end(); ++iter, ++iter2 )
    {
        ASSERT_EQ( iter, iter2 );
    }

    for ( i = 63, iter = --list.end(); iter != list.end(); --i, --iter )
    {
        ASSERT_EQ( i, *iter );
    }

    List<uint32>::ConstIterator citer;
    for ( i = 0, citer = list.cbegin(); citer != list.cend(); ++i, ++citer )
    {
        ASSERT_EQ( i, *citer );
    }

    EXPECT_THROW( list.at( 65 ), std::runtime_error );
}