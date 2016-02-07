cmake_minimum_required(VERSION 2.8.7)
project(tron)

#
# PRE-REQS
#
find_package( Threads REQUIRED )

#
# OPTIONS
#

if( TRAVIS_CI )
    set( BUILD_TESTS TRUE )
elseif( CLION )
    set( BUILD_TESTS TRUE )
elseif( VC )
    set( BUILD_TESTS TRUE )
endif()

#
# SOURCE DEFINITIONS
#

set(
    SOURCE_FILES
    LICENSE
    README.md
    src/engine/intdef.cpp
    include/engine/intdef.h
    src/engine/memory/counting_allocator.cpp
    include/engine/memory/counting_allocator.h
    src/engine/memory/default_allocator.cpp
    include/engine/memory/default_allocator.h
    src/engine/memory/iallocator.cpp
    include/engine/memory/iallocator.h
    src/engine/memory/allocator_guard.cpp
    include/engine/memory/allocator_guard.h
    src/engine/memory/memory_utils.cpp
    include/engine/memory/memory_utils.h
    src/engine/memory/stack_guard.cpp
    include/engine/memory/stack_guard.h
    src/engine/containers/dynamic_array.cpp
    include/engine/containers/dynamic_array.h
)

set(
    TEST_FILES
    test/engine/memory/allocator_guard.t.cpp
    test/engine/memory/counting_allocator.t.cpp
    test/engine/memory/default_allocator.t.cpp
    test/engine/memory/memory_utils.t.cpp
    test/engine/memory/stack_guard.t.cpp
    test/engine/containers/dynamic_array.t.cpp
)

#
# CONSTANT DEFINITIONS
#

# DIRECTORIES
set( INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )
set( EXT_DIR ${PROJECT_SOURCE_DIR}/ext )
set( LIB_DIR ${PROJECT_SOURCE_DIR}/lib )
set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( RES_DIR ${PROJECT_SOURCE_DIR}/res )
set( TEST_DIR ${PROJECT_SOURCE_DIR}/test )

#
# MAIN PROJECT
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

# INCLUDES
include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
    ${TEST_DIR}
    ${EXT_DIR}/googletest/googletest/include
)

# SUB PROJECTS
add_subdirectory(
    ${EXT_DIR}/googletest/googletest
)

# MAIN EXECUTABLE
add_executable(
    tron
    game.m.cpp
    ${SOURCE_FILES}
)

# TEST EXECUTABLE
if ( BUILD_TESTS )
    add_executable(
        all_tests
        test.m.cpp
        ${SOURCE_FILES}
        ${TEST_FILES}
    )

    target_link_libraries(
        all_tests
        gtest
        ${CMAKE_THREAD_LIBS_INIT}
    )
endif()