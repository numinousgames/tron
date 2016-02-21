// window.h
#ifndef NGE_RNDR_WINDOW_H
#define NGE_RNDR_WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>

#include "engine/containers/dynamic_array.h"
#include "engine/intdef.h"

namespace nge
{

namespace rndr
{

class Window
{
  private:
    /**
     * The underlying window.
     */
    GLFWwindow* _window;

    /**
     * The window width.
     */
    uint32 _width;

    /**
     * The window height.
     */
    uint32 _height;

    /**
     * If the window is open.
     */
    bool _isOpen;

    // CONSTRUCTORS
    /**
     * Disabled: windows cannot be copied.
     */
    Window( const Window& window ) = delete;

    // OPERATORS
    /**
     * Disabled: windows cannot be copied.
     */
    Window& operator=( const Window& window ) = delete;

  public:
    // CONSTRUCTORS
    /**
     * Constructs a new window.
     */
    Window();

    /**
     * Destructs the window.
     */
    ~Window();

    // ACCESSOR FUNCTIONS
    /**
     * Gets the width in pixels.
     */
    uint32 width() const;

    /**
     * Gets the height in pixels.
     */
    uint32 height() const;

    /**
     * Gets the aspect ratio.
     */
    float aspectRatio() const;

    // MEMBER FUNCTIONS
    /**
     * Opens the window and returns if it was successfully opened.
     *
     * Behavior is undefined when:
     * The window is already open.
     */
    bool open( const std::string& title, uint32 width, uint32 height );

    /**
     * Refreshes the window.
     *
     * This performs operations such as checking for resizing.
     */
    void refresh();

    /**
     * Determines if the window should be closed.
     */
    bool shouldClose() const;

    /**
     * Manually informs the window it should close.
     */
    void close();

    /**
     * Swaps the window buffer.
     */
    void swapBuffer();
};

// CONSTRUCTORS
inline
Window::Window() : _window( nullptr ), _width( 0 ), _height( 0 ),
                   _isOpen( false )
{
}

inline
Window::~Window()
{
    if ( _window != nullptr )
    {
        glfwDestroyWindow( _window );
    }
}

// ACCESSOR FUNCTIONS
inline
uint32 Window::width() const
{
    return _width;
}

inline
uint32 Window::height() const
{
    return _height;
}

inline
float Window::aspectRatio() const
{
    return static_cast<float>( _width ) / static_cast<float>( _height );
}

// MEMBER FUNCTIONS
inline
bool Window::shouldClose() const
{
    return !_isOpen || glfwWindowShouldClose( _window );
}

inline
void Window::close()
{
    if ( _window != nullptr )
    {
        glfwDestroyWindow( _window );
    }

    _window = nullptr;
}

inline
void Window::swapBuffer()
{
    assert( _window != nullptr );
    glfwSwapBuffers( _window );
}

} // End nspc rndr

} // End nspc nge

#endif
