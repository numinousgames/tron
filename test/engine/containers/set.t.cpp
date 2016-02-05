// set.t.cpp
#include <engine/containers/set.h>
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

TEST( Set, Construction )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;
    using namespace nge::util;

    Set<std::string> def;
    Set<std::string> copy( def );
    Set<std::string> move( std::move( def ) );

    copy = move;
    move = std::move( copy );

    Set<std::string> cap( 100 );
    Set<std::string> hashFunc( &HashUtils::fnv1a );
    Set<std::string> hashFuncAndCap( 100, &HashUtils::fnv1a );

    DefaultAllocator<std::string> valAlloc;
    DefaultAllocator<uint32> binAlloc;
    Set<std::string> withAlloc( &valAlloc, &binAlloc );
    Set<std::string> withAllocAndCap( &valAlloc, &binAlloc, 100 );
    Set<std::string> withAllocAndHashFunc( &valAlloc, &binAlloc,
                                           &HashUtils::fnv1a );
    Set<std::string> withAll( &valAlloc, &binAlloc, 100, &HashUtils::fnv1a );
}

TEST( Set, MemberFunctions )
{
    using namespace nge;
    using namespace nge::cntr;
    using namespace nge::mem;
    using namespace nge::util;

    Set<std::string> set;
    constexpr uint32 COUNT = 64;

    uint32 i;
    uint32 j;
    for ( i = 0; i < COUNT; ++i )
    {
        set.add( getString( i + 1 ) );
        ASSERT_EQ( i + 1, set.size() );
        ASSERT_FALSE( set.isEmpty() );

        for ( j = 0; j < i; ++j )
        {
            ASSERT_TRUE( set.has( getString( j + 1 ) ) );
        }
    }

    for ( i = 0; i < COUNT; ++i )
    {
        set.remove( getString( i + 1 ) );
        ASSERT_EQ( COUNT - i - 1, set.size() );

        std::string str;
        for ( j = 0; j <= i; ++j )
        {
            str = getString( j + 1 );
            ASSERT_FALSE( set.has( str ) );
        }

        for ( j = i + 1; j < COUNT; ++j )
        {
            str = getString( j + 1 );
            ASSERT_TRUE( set.has( str ) );
        }
    }
}