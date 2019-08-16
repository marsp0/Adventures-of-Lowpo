#include <GLFW/glfw3.h>
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// Display includes
#include <X11/Xlib.h>
#include <X11/keysym.h>

GLFWwindow* window;
Display* display;
Window winfocus;
int revert;

float EPSILON;

class GLFWListener: public Catch::TestEventListenerBase
{
	using TestEventListenerBase::TestEventListenerBase;

	virtual void testRunStarting( Catch::TestRunInfo const& testRunInfo )
	{
		std::cout << "Starting test run" << std::endl;
		EPSILON = 0.0005;
		// glfw init
	    glfwInit();
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	    window = glfwCreateWindow(800, 600, "Adventures of Lowpo", nullptr, nullptr);
	    glfwMakeContextCurrent(window);

		// display init
		display = XOpenDisplay(NULL);
        XGetInputFocus(display, &winfocus, &revert);
        
	}

	virtual void testRunEnded( Catch::TestRunStats const& testRunStats )
	{
		std::cout << "Ending Test Run" << std::endl;
		glfwTerminate();
	}
};

CATCH_REGISTER_LISTENER( GLFWListener )