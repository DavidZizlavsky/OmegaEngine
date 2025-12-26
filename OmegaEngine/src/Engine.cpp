#include "OmegaEngine/Engine.h"
#include "OmegaEngine/Renderer.h"
#include "OmegaEngine/RendererOpenGL.h"
#include "OmegaEngine/Window.h"
#include <iostream>

namespace Omega {
	void Engine::Init()
	{
		m_window = m_windowSystem.CreateWindow(600, 400, "Test");

		// TODO: abstract Renderer creation to not use implemented code in here
		m_renderer = new RendererOpenGL();
#ifdef DEBUG
		m_renderer->Debug();
#endif
		
		// TODO: move loop to App code
		while (!m_window->WindowShouldClose()) {
			m_renderer->FrameBegin();

			m_window->SwapBuffers();
			m_window->PollEvents();
		}
	}

	void Engine::Shutdown()
	{
		if (m_renderer != nullptr) {
			delete m_renderer;
		}

		if (m_window != nullptr) {
			delete m_window;
		}
	}
}