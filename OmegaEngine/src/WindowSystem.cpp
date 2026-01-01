#include "OmegaEngine/Window/Window.h"
#include "OmegaEngine/Window/impl/WindowGLFW.h"
#include <stdexcept>

namespace Omega {
	/*
	 * Initialization of a windowing system - implementation-independent wrapper
	 */ 
	WindowSystem::WindowSystem() {
		m_windowLib = WindowLib::GLFW;
		WindowGLFW::InitGLFW();
	}

	/*
	 * Shutdown current implementation of a windowing system
	 */
	WindowSystem::~WindowSystem() {
		WindowGLFW::ShutdownGLFW();
	}

	/*
	 * Create a new window with current implementation of a windowing system
	 */
	Window* WindowSystem::CreateWindow(int width, int height, const char* title) {
		WindowGLFW* window = new WindowGLFW(width, height, title);
		if (window == nullptr) {
			throw std::runtime_error("Failed to allocate memory for GLFW window!");
		}
		return window;
	}
}