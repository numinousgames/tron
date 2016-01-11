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
    include/engine/memory/iallocator.h
    src/engine/memory/iallocator.cpp
    include/engine/memory/default_allocator.h
    src/engine/memory/default_allocator.cpp
)

set(
    TEST_FILES

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
    ${EXT_DIR}/googletest/googlemock/include
    ${EXT_DIR}/googletest/googletest/include
)

# SUB PROJECTS
add_subdirectory(
    ${EXT_DIR}/googletest
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