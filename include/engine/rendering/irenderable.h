// irenderable.h
#ifndef NGE_RNDR_IRENDERABLE_H
#define NGE_RNDR_IRENDERABLE_H

namespace nge
{

namespace rndr
{

class IRenderable
{
  public:
    // CONSTRUCTORS
    /**
     * Destructs the renderable.
     */
    virtual ~IRenderable() = 0;

    // MEMBER FUNCTIONS
    /**
     * Draws the renderable.
     */
    virtual void draw() = 0;
};

// CONSTRUCTORS
inline
IRenderable::~IRenderable()
{
}

} // End nspc rndr

} // End nspc nge

#endif
