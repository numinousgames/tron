// map.t.cpp
#include <engine/containers/map.h>
#include <gtest/gtest.h>

std::string getString( nge::uint32 size )
{
    std::ostringstream oss;
    while ( size-- > 0 )
    {
        oss << "a";
    }
    return oss.str();
}

TEST( Map, Construction )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;
    using namespace nge::util;

    Map<std::string, uint32> def;
    Map<std::string, uint32> copy( def );
    Map<std::string, uint32> move( std::move( def ) );

    copy = move;
    move = std::move( copy );

    Map<std::string, uint32> cap( 100 );
    Map<std::string, uint32> hashFunc( &HashUtils::fnv1a );
    Map<std::string, uint32> hashFuncAndCap( 100, &HashUtils::fnv1a );

    DefaultAllocator<Map::KVPair<std::string, uint32>> valAlloc;
    DefaultAllocator<uint32> binAlloc;
    Map<std::string, uint32> withAlloc( &valAlloc, &binAlloc );
    Map<std::string, uint32> withAllocAndCap( &valAlloc, &binAlloc, 100 );
    Map<std::string, uint32> withAllocAndHashFunc( &valAlloc, &binAlloc,
                                           &HashUtils::fnv1a );
    Map<std::string, uint32> withAll( &valAlloc, &binAlloc, 100, &HashUtils::fnv1a );
}

TEST( Map, MemberFunctions )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;
    using namespace nge::util;

    Map<std::string, uint32> map;
    constexpr uint32 COUNT = 64;

    uint32 i;
    uint32 j;
    for ( i = 0; i < COUNT; ++i )
    {
        map.assign( getString( i + 1 ), i + 1 );
        ASSERT_EQ( i + 1, map.size() );
        ASSERT_FALSE( map.isEmpty() );

        for ( j = 0; j < i; ++j )
        {
            ASSERT_TRUE( map.hasKey( getString( j + 1 ) ) );
            ASSERT_TRUE( map.hasValue( j + 1 ) );
        }
    }

    for ( i = 0; i < COUNT; ++i )
    {
        map.remove( getString( i + 1 ) );
        ASSERT_EQ( COUNT - i - 1, map.size() );

        std::string str;
        for ( j = 0; j <= i; ++j )
        {
            str = getString( j + 1 );
            ASSERT_FALSE( map.hasKey( str ) );
            ASSERT_FALSE( map.hasValue( j + 1 ) );
        }

        for ( j = i + 1; j < COUNT; ++j )
        {
            str = getString( j + 1 );
            ASSERT_TRUE( map.hasKey( str ) );
            ASSERT_TRUE( map.hasValue( j + 1 ) );
        }
    }
}

TEST( Map, Iterator )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;
    using namespace nge::util;

    Map<uint32, uint32> map;
    uint32 i;

    for ( i = 0; i < 32; ++i )
    {
        map.assign( i, i );
    }

    Map<uint32, uint32>::ConstIterator iter;
    for ( i = 0, iter = map.cbegin(); iter != map.cend(); ++i, ++iter )
    {
        ASSERT_EQ( i, *iter );
    }

    Map<uint32, uint32>::ConstIterator iter2;
    for ( iter = iter2 = map.cbegin(); iter != map.cend(); ++iter, ++iter2 )
    {
        ASSERT_EQ( iter, iter2 );
    }
}