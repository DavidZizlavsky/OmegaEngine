#pragma once
#include "OmegaEngine/Renderer/Renderer.h"
#include "OmegaEngine/Renderer/impl/RendererOpenGL.h"
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>
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

		// Frame UBO initialization
		glCreateBuffers(1, &m_frameUBO);
		glNamedBufferStorage(
			m_frameUBO,
			sizeof(FrameData),
			nullptr,
			GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT
		);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_frameUBO);
	}
	
	/*
	 * Shutdown renderer
	 */
	RendererOpenGL::~RendererOpenGL() 
	{
		glDeleteBuffers(1, &m_frameUBO);
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
		// TEST: what does sending nullptrs (0 size indices or vertices arrays)

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
		meshBufferObject.verticesCount = meshObject.verticesCount;
		meshBufferObject.indicesCount = meshObject.indicesCount;

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
		// TODO: Size of a vector can get bigger than handle's data type
		MaterialHandle materialHandle = m_materialObjects.size();
		m_materialObjects.push_back(material);

		return materialHandle;
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

	// Draws a single not-instanced and not-batched render object
	void RendererOpenGL::Draw(RenderObject renderObject)
	{
		MaterialObject materialObject = m_materialObjects[renderObject.materialHandle];
		MeshBufferObject meshBufferObject = m_meshBufferObjects[renderObject.meshHandle];
		GLuint shaderProgram = m_shaderPrograms[materialObject.shaderProgramHandle];

		glUseProgram(shaderProgram);

		// TODO: change this to UBO or move uniform location to member variable
		GLint uColorLocation = glGetUniformLocation(shaderProgram, "u_Color");
		if (uColorLocation != -1) {
			glUniform4fv(
				uColorLocation,
				1,
				glm::value_ptr(materialObject.color)
			);
		}
		else {
			std::cerr << "Color uniform location not found" << std::endl;
		}

		GLint uModelMatrixLocation = glGetUniformLocation(shaderProgram, "u_ModelMatrix");
		if (uModelMatrixLocation != -1) {
			glUniformMatrix4fv(
				uModelMatrixLocation,
				1,
				GL_FALSE,
				glm::value_ptr(renderObject.modelMatrix)
			);
		}
		else {
			std::cerr << "Model matrix uniform location not found" << std::endl;
		}

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_frameUBO);

		glBindVertexArray(meshBufferObject.vao);
		glDrawElements(
			GL_TRIANGLES,
			meshBufferObject.indicesCount,
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	// Update 
	void RendererOpenGL::UpdateRenderObjectModelMatrix(RenderObjectHandle renderObjectHandle, glm::mat4 modelMatrix)
	{
		RenderObject& renderObject = m_renderObjects.at(renderObjectHandle);
		renderObject.modelMatrix = modelMatrix;
	}

	// Update camera data
	void RendererOpenGL::UpdateCameraData(CameraData cameraData)
	{
		FrameData frameData = {};
		frameData.projectionMatrix = cameraData.projectionMatrix;
		frameData.viewMatrix = cameraData.viewMatrix;

		void* mappedPtr = glMapNamedBufferRange(
			m_frameUBO,
			0,
			sizeof(FrameData),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT
		);

		std::memcpy(mappedPtr, &frameData, sizeof(FrameData));

		glUnmapNamedBuffer(m_frameUBO);
	}

	// Renders all render objects
	void RendererOpenGL::FrameEnd()
	{
		for (auto &renderObject : m_renderObjects) {
			Draw(renderObject);
		}
	}

	// Adjust viewport according to viewport
	void RendererOpenGL::ResizeFramebuffer(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}