cmake_minimum_required(VERSION 2.8.7)
project(tron)

#
# PRE-REQS
#
find_package( Threads REQUIRED )
find_package( OpenGL REQUIRED )

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
    # BASE
    src/engine/intdef.cpp
    include/engine/intdef.h
    src/engine/port.cpp
    include/engine/port.h
    src/engine/strdef.cpp
    include/engine/strdef.h
    # CONTAINERS
    src/engine/containers/dynamic_array.cpp
    include/engine/containers/dynamic_array.h
    src/engine/containers/fixed_array.cpp
    include/engine/containers/fixed_array.h
    src/engine/containers/list.cpp
    include/engine/containers/list.h
    src/engine/containers/set.cpp
    include/engine/containers/set.h
    # MATH
    src/engine/math/mat.cpp
    include/engine/math/mat.h
    src/engine/math/mat2x2.cpp
    include/engine/math/mat2x2.h
    src/engine/math/mat3x3.cpp
    include/engine/math/mat3x3.h
    src/engine/math/mat4x4.cpp
    include/engine/math/mat4x4.h
    src/engine/math/mat_math.cpp
    include/engine/math/mat_math.h
    src/engine/math/math.cpp
    include/engine/math/math.h
    src/engine/math/vec.cpp
    include/engine/math/vec.h
    src/engine/math/vec2.cpp
    include/engine/math/vec2.h
    src/engine/math/vec3.cpp
    include/engine/math/vec3.h
    src/engine/math/vec4.cpp
    include/engine/math/vec4.h
    src/engine/math/vec_math.cpp
    include/engine/math/vec_math.h
    # MEMORY
    src/engine/memory/allocator_guard.cpp
    include/engine/memory/allocator_guard.h
    src/engine/memory/counting_allocator.cpp
    include/engine/memory/counting_allocator.h
    src/engine/memory/default_allocator.cpp
    include/engine/memory/default_allocator.h
    src/engine/memory/iallocator.cpp
    include/engine/memory/iallocator.h
    src/engine/memory/memory_utils.cpp
    include/engine/memory/memory_utils.h
    src/engine/memory/stack_guard.cpp
    include/engine/memory/stack_guard.h
    # RENDERING
    src/engine/rendering/gl_renderer.cpp
    include/engine/rendering/gl_renderer.h
    src/engine/rendering/irenderable.cpp
    include/engine/rendering/irenderable.h
    src/engine/rendering/irenderer.cpp
    include/engine/rendering/irenderer.h
#    src/engine/rendering/shape.cpp
#    include/engine/rendering/shape.h
    src/engine/rendering/window.cpp
    include/engine/rendering/window.h
    # UTILITY
    src/engine/utility/hasher.cpp
    include/engine/utility/hasher.h
    src/engine/utility/hash_utils.cpp
    include/engine/utility/hash_utils.h
    src/engine/utility/timer.cpp
    include/engine/utility/timer.h
    # WORLD
    src/engine/world/itickable.cpp
    include/engine/world/itickable.h
    src/engine/world/ngudef.cpp
    include/engine/world/ngudef.h
    src/engine/world/scene.cpp
    include/engine/world/scene.h
#    src/engine/world/view_port.cpp
#    include/engine/world/view_port.h
)

set(
    TEST_FILES
    # CONTAINERS
    test/engine/containers/dynamic_array.t.cpp
    test/engine/containers/fixed_array.t.cpp
    test/engine/containers/list.t.cpp
    test/engine/containers/set.t.cpp
    # MATH
    test/engine/math/mat2x2.t.cpp
    test/engine/math/mat3x3.t.cpp
    test/engine/math/mat4x4.t.cpp
    test/engine/math/vec2.t.cpp
    test/engine/math/vec3.t.cpp
    test/engine/math/vec4.t.cpp
    # MEMORY
    test/engine/memory/allocator_guard.t.cpp
    test/engine/memory/counting_allocator.t.cpp
    test/engine/memory/default_allocator.t.cpp
    test/engine/memory/memory_utils.t.cpp
    test/engine/memory/stack_guard.t.cpp
    # UTILITY
    test/engine/utility/hasher.t.cpp
    test/engine/utility/hash_utils.t.cpp
    test/engine/utility/timer.t.cpp
    # WORLD
    test/engine/world/mock_tickable.cpp
    test/engine/world/mock_tickable.h
    test/engine/world/scene.t.cpp
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

# SUB PROJECTS
add_subdirectory(${EXT_DIR}/googletest/googletest)
add_subdirectory(${EXT_DIR}/glfw)

# INCLUDES
include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
    ${TEST_DIR}
    ${EXT_DIR}/googletest/googletest/include
    ${EXT_DIR}/glfw/include
    ${OPENGL_INCLUDE_DIRS}
)

# MAIN EXECUTABLE
add_executable(
    tron
    game.m.cpp
    ${SOURCE_FILES}
)

target_link_libraries(
    tron
    glfw
    ${GLFW_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENGL_LIBRARIES}
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
        glfw
        ${GLFW_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENGL_LIBRARIES}
    )
endif()