#pragma once
#include "OmegaEngine/Window/Window.h"
#include <GLFW/glfw3.h>

namespace Omega {
	class WindowGLFW final : public Window {
	private:
		GLFWwindow* m_window;

		static bool isInitGLFW;
	public:
		WindowGLFW(int width, int height, const char* title);
		~WindowGLFW();
		int WindowShouldClose() override;
		void PollEvents() override;
		void SwapBuffers() override;

		static void InitGLFW();
		static void ShutdownGLFW();
	};
}