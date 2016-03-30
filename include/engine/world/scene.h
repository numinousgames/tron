// scene.h
#ifndef NGE_WRLD_SCENE_H
#define NGE_WRLD_SCENE_H

#include "engine/containers/dynamic_array.h"
#include "engine/utility/timer.h"
#include "engine/world/itickable.h"
#include "engine/world/ngudef.h"

namespace nge
{

namespace wrld
{

class Scene
{
  private:
    /**
     * The default initial capacity.
     */
    static constexpr uint32 DEFAULT_CAPACITY = static_cast<uint32>( -1 );

    /**
     * The items that are updated during the update cycle..
     */
    cntr::DynamicArray<ITickable*> _tickables;

    /**
     * The warning capacity threshold.
     */
    // TODO: broadcast when the capacity is reached
    uint32 _capacity;

  public:
    // CONSTRUCTORS
    /**
     * Constructs a new scene.
     */
    Scene();

    /**
     * Constructs a scene with the given warning capacity threshold.
     */
    Scene( uint32 capacity );

    /**
     * Constructs a new scene using the given allocator.
     *
     * If the total number of items goes above the capacity warnings will
     * be issued but the engine will behave normally otherwise.
     */
    Scene( mem::IAllocator<ITickable*>* alloc );

    /**
     * Constructs a new scene using the given allocator and tickable
     * capacity.
     *
     * If the total number of items goes above the capacity warnings will
     * be issued but the engine will behave normally otherwise.
     */
    Scene( mem::IAllocator<ITickable*>* alloc, uint32 capacity );

    /**
     * Constructs a copy of the scene.
     */
    Scene( const Scene& scene );

    /**
     * Makes a new scene by moving it a new instance.
     */
    Scene( Scene&& scene );

    /**
     * Destructs the scene.
     */
    ~Scene();

    // OPERATORS
    /**
     * Assigns this as a copy of the given scene.
     */
    Scene& operator=( const Scene& scene );

    /**
     * Moves the given scene to this instance.
     */
    Scene& operator=( Scene&& scene );

    // MEMBER FUNCTIONS
    /**
     * Adds the given tickable to the scene.
     */
    void addTickable( ITickable* tickable );

    /**
     * Removes the given tickable from the scene.
     */
    void removeTickable( ITickable* tickable );

    /**
     * Removes all tickables from the scene.
     */
    void removeAll();

    /**
     * Updates the scene given the elapsed time.
     */
    void update( float dtS );
};

// CONSTRUCTORS
inline
Scene::Scene() : _tickables(), _capacity( DEFAULT_CAPACITY )
{
}

inline
Scene::Scene( uint32 capacity ) : _tickables( capacity ),
                                  _capacity( capacity )
{
}

inline
Scene::Scene( mem::IAllocator<ITickable*>* alloc )
    : _tickables( alloc ), _capacity( DEFAULT_CAPACITY )
{
}

inline
Scene::Scene( mem::IAllocator<ITickable*>* alloc, uint32 capacity )
    : _tickables( alloc, capacity ), _capacity( capacity )
{
}

inline
Scene::Scene( const Scene& scene )
    : _tickables( scene._tickables ),
      _capacity( scene._capacity )
{
}

inline
Scene::Scene( Scene&& scene )
    : _tickables( std::move( scene._tickables ) ),
      _capacity( scene._capacity )
{
    scene._capacity = 0;
}

inline
Scene::~Scene()
{
}

// OPERATORS
inline
Scene& Scene::operator=( const Scene& scene )
{
    _tickables = scene._tickables;
    _capacity = scene._capacity;

    return *this;
}

inline
Scene& Scene::operator=( Scene&& scene )
{
    _tickables = std::move( scene._tickables );
    _capacity = scene._capacity;

    scene._capacity = 0;

    return *this;
}

// MEMBER FUNCTIONS
inline
void Scene::addTickable( ITickable* tickable )
{
    _tickables.push( tickable );
}

inline
void Scene::removeTickable( ITickable* tickable )
{
    uint32 index = _tickables.indexOf( tickable );

    if ( index != static_cast<uint32>( -1 ) )
    {
        _tickables.removeAt( index );
    }
}

inline
void Scene::removeAll()
{
    _tickables.clear();
}

} // End nspc wrld

} // End nspc nge

#endif
