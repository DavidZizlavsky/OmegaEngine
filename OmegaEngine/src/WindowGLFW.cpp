#include "OmegaEngine/Window/impl/WindowGLFW.h"
#include <stdexcept>

namespace Omega {
	/* 
	 * Variable for safety checks to prevent double termination/initialization of GLFW
	 */
	bool WindowGLFW::isInitGLFW = false;

	/*
	 * Initializes GLFW library for further use
	 */
	void WindowGLFW::InitGLFW() {
		if (isInitGLFW) {
			return;
		}

		if (glfwInit() != GLFW_TRUE) {
			throw std::runtime_error("GLFW initialization failed!");
		}
		isInitGLFW = true;

		// Use OpenGL Core 4.5
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
		// Use debug OpenGL context but only in debug building configuration
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	}

	/*
	 * Terminates GLFW library
	 */
	void WindowGLFW::ShutdownGLFW() {
		if (!isInitGLFW) {
			return;
		}

		glfwTerminate();
		isInitGLFW = false;
	}

	/*
	 * Creates and opens a new GLFW window
	 */
	WindowGLFW::WindowGLFW(int width, int height, const char* title) {
		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (m_window == nullptr) {
			throw std::runtime_error("Failed to create a window!");
		}
		glfwMakeContextCurrent(m_window);
	}

	/*
	 * Destroys GLFW window
	 */
	WindowGLFW::~WindowGLFW() {
		glfwDestroyWindow(m_window);
	}

	/*
	 * Determines if the window should be closed based on window events (such as close button)
	 */
	int WindowGLFW::WindowShouldClose() {
		return glfwWindowShouldClose(m_window);
	}

	/*
	 * Polls all events from GLFW (this may block on some platforms)
	 */
	void WindowGLFW::PollEvents() {
		glfwPollEvents();
	}

	/*
	 * Swaps buffers of a GLFW window
	 */
	void WindowGLFW::SwapBuffers() {
		glfwSwapBuffers(m_window);
	}
}