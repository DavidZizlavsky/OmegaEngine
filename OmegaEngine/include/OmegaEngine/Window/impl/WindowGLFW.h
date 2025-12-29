#pragma once
#include "OmegaEngine/Window/Window.h"
#include <GLFW/glfw3.h>

namespace Omega {
	class WindowGLFW final : public Window {
	private:
		GLFWwindow* m_window;

		static bool isInitGLFW;
		ResizeCallback m_resizeCallback;
	public:
		WindowGLFW(int width, int height, const char* title);
		~WindowGLFW();
		int WindowShouldClose() override;
		void PollEvents() override;
		void SwapBuffers() override;

		void SetResizeCallback(ResizeCallback callback) override;
		void OnCallbackFramebufferResize(int width, int height);

		static void CallbackFramebufferResize(GLFWwindow* window, int width, int height);

		static void InitGLFW();
		static void ShutdownGLFW();
	};
}