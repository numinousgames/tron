// scene.cpp
#include "engine/world/scene.h"

namespace nge
{

namespace wrld
{

// MEMBER FUNCTIONS
void Scene::update( float dtS )
{
    cntr::DynamicArray<ITickable*>::Iterator iter;

    for ( iter = _tickables.begin(); iter != _tickables.end(); ++iter )
    {
        ( *iter )->pretick();
    }

    for ( iter = _tickables.begin(); iter != _tickables.end(); ++iter )
    {
        ( *iter )->tick( dtS );
    }

    for ( iter = _tickables.begin(); iter != _tickables.end(); ++iter )
    {
        ( *iter )->postick();
    }
}

} // End nspc wrld

} // End nspc nge