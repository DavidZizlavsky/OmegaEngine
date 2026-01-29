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
		materialObject.color = glm::vec4(1);

		MaterialHandle materialHandle = m_renderer->CreateMaterial(materialObject);

		auto timeStart = std::chrono::high_resolution_clock::now();
		MeshObject test = File::LoadMeshFromObj("assets/meshes/AMV.obj");
		auto timeEnd = std::chrono::high_resolution_clock::now();
		std::cout << "Time to load 3D model: " << std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart) << "\n";

		MeshHandle meshHandle = m_renderer->CreateMesh(test);

		RenderObject renderObject = {};
		renderObject.materialHandle = materialHandle;
		renderObject.meshHandle = meshHandle;
		renderObject.modelMatrix = glm::mat4(1);

		RenderObjectHandle renderObjectHandle = m_renderer->CreateRenderObject(renderObject);

		CameraData cameraData = {};
		cameraData.cameraPosition = glm::vec3(0, 2, 3);
		cameraData.fov = 60.0f;
		cameraData.nearPlane = 0.1f;
		cameraData.farPlane = 100.0f;

		float totalTime = 0.0f;

		std::vector<PointLight> lights = std::vector<PointLight>();
		PointLight pointLight = {};
		pointLight.position = glm::vec4(0, 3, 2, 1);
		pointLight.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		lights.push_back(pointLight);

		float rotation = 0;
		constexpr float angleSpeed = glm::radians(45.0f);

		m_timeFrameBefore = std::chrono::steady_clock::now();

		FramebufferSize framebufferSize = m_window->GetFramebufferSize();
		double yaw = -90.0f, pitch = -45.0f;

		MousePosition mousePosition = {};
		mousePosition.x = framebufferSize.width / 2.0;
		mousePosition.y = framebufferSize.height / 2.0;
		m_window->SetCursorPosition(mousePosition);

		m_window->SetCursorMode(CursorMode::Hidden);
		// TODO: move loop to App code
		while (!m_window->WindowShouldClose()) {
			auto now = std::chrono::steady_clock::now();
			float deltaTime = std::chrono::duration<float>(now - m_timeFrameBefore).count();
			m_timeFrameBefore = now;

			m_renderer->FrameBegin();

			m_renderer->UpdateLightData(lights, glm::vec3(0.06f, 0.06f, 0.07f));

			rotation += angleSpeed * deltaTime;
			glm::mat4 model = glm::rotate(glm::mat4(1), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
			m_renderer->UpdateRenderObjectModelMatrix(renderObjectHandle, model);

			framebufferSize = m_window->GetFramebufferSize();
			if (framebufferSize.height != 0) {
				cameraData.aspectRatio = framebufferSize.width / (float)framebufferSize.height;
			}

			mousePosition = m_window->GetCursorPosition();
			double xoffset = mousePosition.x - (framebufferSize.width / 2);
			double yoffset = (framebufferSize.height / 2) - mousePosition.y;

			constexpr float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			pitch = glm::clamp(pitch, -89.0, 89.0);

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			front = glm::normalize(front);

			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
			glm::vec3 up = glm::normalize(glm::cross(right, front));

			cameraData.cameraFront = front;
			cameraData.cameraUp = up;

			m_renderer->UpdateCameraData(cameraData);

			mousePosition.x = framebufferSize.width / 2.0;
			mousePosition.y = framebufferSize.height / 2.0;
			m_window->SetCursorPosition(mousePosition);

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