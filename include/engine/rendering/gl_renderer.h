// gl_renderer.h
#ifndef NGE_RNDR_GL_RENDERER_H
#define NGE_RNDR_GL_RENDERER_H

#include "engine/rendering/irenderer.h"

namespace nge
{

namespace rndr
{

class GlRenderer : public IRenderer<IRenderable>
{
  private:
    // MEMBERS
    /**
     * The renderable items.
     */
    cntr::DynamicArray<IRenderable*> _items;

    /**
     * The window the renderer is attached to.
     */
    Window* _window;

    /**
     * If the renderer is currently drawing.
     */
    bool _isActive;

    // CONSTRUCTORS
    /**
     * Disabled: renderers cannot be copied.
     */
    GlRenderer( const GlRenderer& renderer ) = delete;

    // OPERATORS
    /**
     * Disabled: renderers cannot be copied
     */
    GlRenderer& operator=( const GlRenderer& renderer ) = delete;

  public:
    // CONSTRUCTORS
    /**
     * Constructs a new opengl renderer.
     */
    GlRenderer();

    /**
     * Destructs the renderer.
     */
    ~GlRenderer();

    // MEMBER FUNCTIONS
    /**
     * Adds a renderable to the render queue.
     */
    virtual void addRenderable( IRenderable* renderable );

    /**
     * Adds a renderable from the render queue.
     */
    virtual void removeRenderable( IRenderable* renderable );

    /**
     * Removes all of the renderables from the render queue.
     */
    virtual void removeAllRenderables();

    /**
     * Attaches the renderer to a window.
     */
    virtual void attach( Window* window );

    /**
     * Draws the next frame.
     */
    virtual void draw();

    /**
     * Detaches the renderer from the current window.
     */
    virtual void detach();

    /**
     * Checks if the renderer is currently attached.
     */
    virtual bool isAttached() const;

    /**
     * Checks if the renderer is currently drawing.
     */
    virtual bool isActive() const;
};

// CONSTRUCTORS
inline
GlRenderer::GlRenderer() : _window( nullptr ), _isActive( false )
{
}

inline
GlRenderer::~GlRenderer()
{
}

} // End nspc rndr

} // End nspc nge

#endif
