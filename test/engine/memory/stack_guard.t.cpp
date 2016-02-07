// stack_guard.t.cpp
#include <engine/memory/stack_guard.h>
#include <gtest/gtest.h>

// TODO: use CountingAllocator to validate that memory is released
TEST( StackGuard, Full )
{
    using namespace nge::mem;

    DefaultAllocator<std::string> alloc;
    StackGuard<std::string> null;

    StackGuard<std::string> guard( &alloc, alloc.get( 1 ) );
    StackGuard<std::string> guardCount( &alloc, alloc.get( 25 ), 25 );
    StackGuard<std::string> guardDef( alloc.get( 1 ) );
    StackGuard<std::string> guardDefCount( alloc.get( 25 ), 25 );

    ASSERT_TRUE( !null );
    ASSERT_FALSE( !guard );
    ASSERT_FALSE( !guardCount );
    ASSERT_FALSE( !guardDef );
    ASSERT_FALSE( !guardDefCount );

    ASSERT_FALSE( null == true );
    ASSERT_TRUE( guard == true );
    ASSERT_TRUE( guardCount == true );
    ASSERT_TRUE( guardDef == true );
    ASSERT_TRUE( guardDefCount == true );

    ASSERT_FALSE( null );
    ASSERT_TRUE( guard );
    ASSERT_TRUE( guardCount );
    ASSERT_TRUE( guardDef );
    ASSERT_TRUE( guardDefCount );

    ASSERT_FALSE( null != false );
    ASSERT_TRUE( guard != false );
    ASSERT_TRUE( guardCount != false );
    ASSERT_TRUE( guardDef != false );
    ASSERT_TRUE( guardDefCount != false );

    ASSERT_NO_FATAL_FAILURE( *guard = "this code is on fire" );
    ASSERT_STREQ( "this code is on fire", ( *guard ).c_str() );
    ASSERT_STREQ( "this code is on fire", guard->c_str() );
}