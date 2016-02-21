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

    const float ratio = _window->aspectRatio();

    glViewport( 0, 0, _window->width(), _window->height() );

    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();

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