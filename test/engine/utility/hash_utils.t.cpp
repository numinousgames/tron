// hash_utils.t.cpp
#include <engine/utility/hash_utils.h>
#include <gtest/gtest.h>

TEST( HashUtils, Fnv1aHash )
{
    using namespace nge::util;
    // TODO: compute the following hash by hand
    //ASSERT_EQ( ???, HashUtils::fnv1a( "WOMBO COMBO!" ) );
}

TEST( HashUtils, CompileTimeHash )
{
    using namespace nge::util;
    ASSERT_EQ( HashUtils::fnv1a( "hisNameIsRobertPaulson" ),
               chash( "hisNameIsRobertPaulson" ) );
}