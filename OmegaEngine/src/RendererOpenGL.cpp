#pragma once
#include "OmegaEngine/Renderer/Renderer.h"
#include "OmegaEngine/Renderer/impl/RendererOpenGL.h"
#include <GLEW/glew.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <cstddef>
#include <cstdint>

#ifdef DEBUG
static void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
	std::cerr << "[OpenGL Debug] " << message << std::endl;
}
#endif

namespace Omega {
	/*
	 * Variable for safety checks to prevent double termination/initialization of GLFW
	 */
	bool RendererOpenGL::isInitGLEW = false;

	/*
	 * Initializes GLEW library for further use
	 */
	void RendererOpenGL::InitGLEW()
	{
		if (isInitGLEW) {
			return;
		}
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			throw std::runtime_error("GLEW initialization failed!");
		}
		isInitGLEW = true;
	}

	/*
	 * Initialize renderer
	 */ 
	RendererOpenGL::RendererOpenGL()
	{
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

	// Register mesh and return a handle
	MeshHandle RendererOpenGL::CreateMesh(MeshObject meshObject)
	{
		GLuint vao = 0;
		GLuint vbo = 0;
		GLuint ebo = 0;

		glCreateVertexArrays(1, &vao);
		glCreateBuffers(1, &vbo);
		glCreateBuffers(1, &ebo);

		glNamedBufferStorage(
			vbo,
			meshObject.verticesCount * sizeof(Vertex),
			meshObject.vertices,
			0
		);

		glNamedBufferStorage(
			ebo,
			meshObject.indicesCount * sizeof(uint32_t),
			meshObject.indices,
			0
		);

		glVertexArrayVertexBuffer(
			vao,
			0,
			vbo,
			0,
			sizeof(Vertex)
		);

		glVertexArrayElementBuffer(
			vao,
			ebo
		);

		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribFormat(
			vao,
			0, // Attrib index
			3, // Size
			GL_FLOAT, // Data type
			GL_FALSE, // Normalized
			offsetof(Vertex, position) // Relative offset
		);
		glVertexArrayAttribBinding(vao, 0, 0);

		MeshBufferObject meshBufferObject = {};
		meshBufferObject.vao = vao;
		meshBufferObject.vbo = vbo;
		meshBufferObject.ebo = ebo;

		// TODO: Size of a vector can get bigger than handle's data type
		MeshHandle meshHandle = m_meshBufferObjects.size();
		m_meshBufferObjects.push_back(meshBufferObject);

		return meshHandle;
	}


	// Compiles GLSL shader and returns OpenGL handle
	GLuint RendererOpenGL::CompileShader(GLenum type, const char* source)
	{
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		GLint success = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint logLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
			std::string log(logLen, '\0');
			glGetShaderInfoLog(shader, logLen, nullptr, log.data());

			const char* typeStr =
				type == GL_VERTEX_SHADER ? "VERTEX" :
				type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "UNKNOWN";

			std::cerr << typeStr << " SHADER COMPILE ERROR:\n" << log << std::endl;
			glDeleteShader(shader);

			throw std::runtime_error("Failed to compile GLSL shader!");
		}

		return shader;
	}

	// Register shader program and return a handle
	ShaderProgramHandle RendererOpenGL::CreateShaderProgram(ShaderProgramObject shaderProgramObject)
	{
		if (shaderProgramObject.vertexShaderSource == nullptr || shaderProgramObject.fragmentShaderSource == nullptr) {
			throw std::runtime_error("Failed to create shader program! Vertex and fragment shaders are required!");
		}

		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, shaderProgramObject.vertexShaderSource);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, shaderProgramObject.fragmentShaderSource);

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		GLint success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success) {
			// TEST: Maybe this needs it's own error handling independent of default debug messenger
			throw std::runtime_error("Failed to link shaders into shader program!");
		}

		// TODO: Size of a vector can get bigger than handle's data type
		ShaderProgramHandle programHandle = m_shaderPrograms.size();
		m_shaderPrograms.push_back(program);

		return programHandle;
	}

	// Register material and return a handle
	MaterialHandle RendererOpenGL::CreateMaterial(MaterialObject material)
	{
		// TODO: implement
		return NULL;
	}

	// Register render object and return a handle
	RenderObjectHandle RendererOpenGL::CreateRenderObject(RenderObject renderObject)
	{
		// TODO: Size of a vector can get bigger than handle's data type
		RenderObjectHandle handle = m_renderObjects.size();
		m_renderObjects.push_back(renderObject);
		m_activeObjects.push_back(handle);
		return handle;
	}
}