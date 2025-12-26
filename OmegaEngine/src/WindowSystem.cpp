#include "OmegaEngine/Window.h"
#include "OmegaEngine/WindowGLFW.h"

namespace Omega {
	/*
	 * Initialization of a windowing system - implementation-independent wrapper
	 */ 
	WindowSystem::WindowSystem() {
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