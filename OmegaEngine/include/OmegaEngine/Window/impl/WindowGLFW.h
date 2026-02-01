#pragma once
#include "OmegaEngine/Window/Window.h"
#include <GLFW/glfw3.h>

namespace Omega {
	class WindowGLFW final : public Window {
	private:
		GLFWwindow* m_window;

		static bool isInitGLFW;
		ResizeCallback m_resizeCallback;
		ScrollCallback m_scrollCallback;

		FramebufferSize m_framebufferSize;
		ScrollOffset m_scrollOffset;
	public:
		WindowGLFW(int width, int height, const char* title);
		~WindowGLFW();
		int WindowShouldClose() override;
		void PollEvents() override;
		void SwapBuffers() override;

		FramebufferSize GetFramebufferSize() override;
		ScrollOffset GetScrollOffset() override;
		MousePosition GetCursorPosition() override;

		void SetCursorPosition(MousePosition position) override;
		void SetCursorMode(CursorMode mode) override;
		bool IsKeyPressed(KeyCode key) override;
		bool IsMouseButtonPressed(MouseButtonCode button) override;

		void SetResizeCallback(ResizeCallback callback) override;
		void OnCallbackFramebufferResize(int width, int height);
		void SetScrollCallback(ScrollCallback callback) override;
		void OnCallbackScroll(double xOffset, double yOffset);

		static void CallbackFramebufferResize(GLFWwindow* window, int width, int height);
		static void CallbackScroll(GLFWwindow* window, double xOffset, double yOffset);

		static void InitGLFW();
		static void ShutdownGLFW();
	};
}