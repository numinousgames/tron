// window.cpp
#include "engine/rendering/window.h"

#include <assert.h>

namespace nge
{

namespace rndr
{

// MEMBER FUNCTIONS
bool Window::open( const std::string& title, uint32 width, uint32 height )
{
    assert( !_isOpen && _window == nullptr );

    _window = glfwCreateWindow( width, height, title.c_str(),
                                nullptr, nullptr );

    if ( _window == nullptr )
    {
        return false;
    }

    _isOpen = true;
    glfwMakeContextCurrent( _window );

    return true;
}

void Window::refresh()
{
    int32 width;
    int32 height;

    glfwGetFramebufferSize( _window, &width, &height );
    glfwPollEvents();

    _width = static_cast<uint32>( width );
    _height = static_cast<uint32>( height );
}

} // End nspc rndr

} // End nspc nge