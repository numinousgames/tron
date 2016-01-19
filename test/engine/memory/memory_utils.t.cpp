// memory_utils.t.cpp
#include <engine/memory/memory_utils.h>
#include <gtest/gtest.h>

TEST( MemoryUtilsTest, All )
{
    using namespace nge::mem;

    char dst[5];

    MemoryUtils::copy( &dst[0], ( char* )"my", 3 );

    EXPECT_STREQ( "my", dst );

    MemoryUtils::move( &dst[0], ( char* )"name", 5 );

    EXPECT_STREQ( "name", dst );
}