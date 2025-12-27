#include "OmegaEngine/Window/Window.h"
#include "OmegaEngine/Window/impl/WindowGLFW.h"

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
		return new WindowGLFW(width, height, title);
	}
}