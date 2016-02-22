// gl_renderer.cpp
#include "engine/rendering/gl_renderer.h"

namespace nge
{

namespace rndr
{

void GlRenderer::addRenderable( IRenderable* renderable )
{
    _items.push( renderable );
}

void GlRenderer::removeRenderable( IRenderable* renderable )
{
    _items.removeAt( _items.indexOf( renderable ) );
}

void GlRenderer::removeAllRenderables()
{
    _items.clear();
}

void GlRenderer::attach( Window* window )
{
    _window = window;
}

void GlRenderer::draw()
{
    assert( _window != nullptr );

    glViewport( 0, 0, _window->width(), _window->height() );

    glClear( GL_COLOR_BUFFER_BIT );

    for ( auto iter = _items.begin();
          iter != _items.end();
          ++iter )
    {
        ( *iter )->draw();
    }

    _window->swapBuffer();
}

void GlRenderer::detach()
{
    _window = nullptr;
}

bool GlRenderer::isAttached() const
{
    return _window != nullptr;
}

bool GlRenderer::isActive() const
{
    return false;
}

} // End nspc rndr

} // End nspc nge