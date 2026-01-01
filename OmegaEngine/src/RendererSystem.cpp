#include "OmegaEngine/Renderer/Renderer.h"
#include "OmegaEngine/Renderer/impl/RendererOpenGL.h"
#include <stdexcept>

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
		RendererOpenGL* renderer = new RendererOpenGL();
		if (renderer == nullptr) {
			throw std::runtime_error("Failed to allocate memory for OpenGL renderer!");
		}
		return renderer;
	}
}