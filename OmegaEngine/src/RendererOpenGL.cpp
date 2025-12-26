#pragma once
#include "OmegaEngine/RendererOpenGL.h"
#include <GLEW/glew.h>
#include <stdexcept>
#include <iostream>

#ifdef DEBUG
static void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	std::cerr << "[OpenGL Debug] " << message << std::endl;
}
#endif

namespace Omega {
	/*
	 * Initialize renderer
	 */ 
	RendererOpenGL::RendererOpenGL()
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("GLEW initialization failed!");
		}

		// Set default clear color
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	}
	
	/*
	 * Shutdown renderer
	 */
	RendererOpenGL::~RendererOpenGL() 
	{
		
	}

#ifdef DEBUG
	/*
	 * Initialize OpenGL debug for debug builds
	 */
	void RendererOpenGL::Debug()
	{
		if (GLEW_ARB_debug_output)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLDebugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		else {
			std::cout << "OpenGL debug extension is not available!" << std::endl;
		}
	}
#endif

	// Start each frame by clearing the buffer
	void RendererOpenGL::FrameBegin()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}