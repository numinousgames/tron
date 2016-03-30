// game.m.cpp

#include <engine/utility/timer.h>
#include <iostream>
#include <engine/rendering/window.h>
#include <engine/rendering/gl_renderer.h>

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int main( int argc, char* argv[] )
{
    using namespace nge;


    if ( !glfwInit() )
    {
        std::cout << "Failed to initialize GLFW\n";
        exit( EXIT_FAILURE );
    }

    glfwSetErrorCallback( &error_callback );

    rndr::Window window;
    window.open( "Numinous Game Engine", 800, 600 );

    rndr::GlRenderer renderer;
    renderer.attach( &window );

    while ( !window.shouldClose() )
    {
        window.refresh();
        renderer.draw();
    }

    renderer.detach();
    window.close();

    glfwTerminate();

    return 0;
}