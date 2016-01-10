cmake_minimum_required(VERSION 3.3)
project(tron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


#
# SOURCE DEFINITIONS
#

set(
    SOURCE_FILES
    LICENSE
    README.md
    game.m.cpp
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

# INCLUDES
include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
    ${TEST_DIR}
    ${EXT_DIR}/googletest
    ${EXT_DIR}/googletest/include
)

# SUB PROJECTS
add_subdirectory( ${EXT_DIR}/googletest )

# MAIN EXECUTABLE
add_executable( tron ${SOURCE_FILES} )