#include "OmegaEngine/Renderer/Renderer.h"
#include "OmegaEngine/Renderer/impl/RendererOpenGL.h"

namespace Omega {
	/*
	 * Initialization of a rendering system - implementation-independent wrapper
	 */
	RendererSystem::RendererSystem() {
		m_rendererLib = RendererLib::OpenGL;
	}

	/*
	 * Shutdown current implementation of a rendering system
	 */
	RendererSystem::~RendererSystem() {

	}

	/*
	 * Create a new renderer instance
	 */
	Renderer* RendererSystem::CreateRenderer() {
		RendererOpenGL::InitGLEW();
		return new RendererOpenGL();
	}
}