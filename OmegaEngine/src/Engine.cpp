#include "OmegaEngine/Engine.h"
#include "OmegaEngine/Renderer/Renderer.h"
#include "OmegaEngine/Window/Window.h"
#include "OmegaEngine/File.h"
#include <iostream>
#include <string>
#include <cstdint>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

namespace Omega {
	void Engine::Init()
	{
		m_window = m_windowSystem.CreateWindow(600, 400, "Test");

		m_renderer = m_rendererSystem.CreateRenderer();
#ifdef DEBUG
		m_renderer->Debug();
#endif
		Window::ResizeCallback resizeCallback = [renderer = m_renderer](int width, int height) {
			renderer->ResizeFramebuffer(width, height);
		};
		m_window->SetResizeCallback(resizeCallback);
		
		std::string vertexSource = File::LoadTextFile("assets/shaders/shaderVertex.vert");
		std::string fragmentSource = File::LoadTextFile("assets/shaders/shaderFragment.frag");

		ShaderProgramObject shaderProgramObject = {};
		shaderProgramObject.vertexShaderSource = vertexSource.c_str();
		shaderProgramObject.fragmentShaderSource = fragmentSource.c_str();

		ShaderProgramHandle shaderProgramHandle = m_renderer->CreateShaderProgram(shaderProgramObject);

		MaterialObject materialObject = {};
		materialObject.shaderProgramHandle = shaderProgramHandle;
		materialObject.color = glm::vec4(1, 0, 0, 1);

		MaterialHandle materialHandle = m_renderer->CreateMaterial(materialObject);

		Vertex vertices[] = { 
			{glm::vec3(-1, 1, 0)},
			{glm::vec3(-1, -1, 0)},
			{glm::vec3(1, -1, 0)},
			{glm::vec3(1, 1, 0)}
		};

		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		MeshObject meshObject = {};
		meshObject.vertices = vertices;
		meshObject.verticesCount = 4;
		meshObject.indices = indices;
		meshObject.indicesCount = 6;

		MeshHandle meshHandle = m_renderer->CreateMesh(meshObject);

		RenderObject renderObject = {};
		renderObject.materialHandle = materialHandle;
		renderObject.meshHandle = meshHandle;
		renderObject.modelMatrix = glm::mat4(1);

		RenderObjectHandle renderObjectHandle = m_renderer->CreateRenderObject(renderObject);

		CameraData cameraData = {};
		cameraData.viewMatrix = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		float rotation = 0;
		constexpr float angleSpeed = glm::radians(45.0f);

		m_timeFrameBefore = std::chrono::steady_clock::now();

		// TODO: move loop to App code
		while (!m_window->WindowShouldClose()) {
			auto now = std::chrono::steady_clock::now();
			float deltaTime = std::chrono::duration<float>(now - m_timeFrameBefore).count();
			m_timeFrameBefore = now;

			m_renderer->FrameBegin();

			rotation += angleSpeed * deltaTime;
			glm::mat4 model = glm::rotate(glm::mat4(1), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			m_renderer->UpdateRenderObjectModelMatrix(renderObjectHandle, model);

			FramebufferSize framebufferSize = m_window->GetFramebufferSize();
			cameraData.projectionMatrix = glm::perspective((float)glm::radians(60.0f), (float)framebufferSize.width / framebufferSize.height, 0.1f, 100.0f);
			m_renderer->UpdateCameraData(cameraData);

			m_renderer->FrameEnd();

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