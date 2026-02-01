#include "OmegaEngine/Window/impl/WindowGLFW.h"
#include "OmegaEngine/Window/Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Omega {
	/* 
	 * Variable for safety checks to prevent double termination/initialization of GLFW
	 */
	bool WindowGLFW::isInitGLFW = false;

	/*
	 * Initializes GLFW library for further use
	 */
	void WindowGLFW::InitGLFW()
	{
		if (isInitGLFW) {
			return;
		}

		if (glfwInit() != GLFW_TRUE) {
			throw std::runtime_error("GLFW initialization failed!");
		}
		isInitGLFW = true;

		// Use OpenGL Core 4.5
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// TODO: Abstract constant value into config struct
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);

#ifdef DEBUG
		// Use debug OpenGL context but only in debug building configuration
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	}

	/*
	 * Terminates GLFW library
	 */
	void WindowGLFW::ShutdownGLFW()
	{
		if (!isInitGLFW) {
			return;
		}

		glfwTerminate();
		isInitGLFW = false;
	}

	/*
	 * GLFW callback for framebuffer resizing 
	 */
	void WindowGLFW::CallbackFramebufferResize(GLFWwindow* window, int width, int height)
	{
		WindowGLFW* self = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

		if (!self) {
			return;
		}

		self->OnCallbackFramebufferResize(width, height);
	}

	void WindowGLFW::OnCallbackFramebufferResize(int width, int height)
	{
		m_framebufferSize.width = width;
		m_framebufferSize.height = height;

		if (m_resizeCallback) {
			m_resizeCallback(width, height);
		}
	}

	void WindowGLFW::CallbackScroll(GLFWwindow* window, double xOffset, double yOffset)
	{
		WindowGLFW* self = static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));

		if (!self) {
			return;
		}

		self->OnCallbackScroll(xOffset, yOffset);
	}

	void WindowGLFW::OnCallbackScroll(double xOffset, double yOffset)
	{
		m_scrollOffset.x = xOffset;
		m_scrollOffset.y = yOffset;

		if (m_scrollCallback) {
			m_scrollCallback(xOffset, yOffset);
		}
	}

	/*
	 * Creates and opens a new GLFW window
	 */
	WindowGLFW::WindowGLFW(int width, int height, const char* title)
	{
		m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (m_window == nullptr) {
			throw std::runtime_error("Failed to create a window!");
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, CallbackFramebufferResize);
		glfwGetFramebufferSize(m_window, &m_framebufferSize.width, &m_framebufferSize.height);
		glfwSetScrollCallback(m_window, CallbackScroll);
	}

	/*
	 * Destroys GLFW window
	 */
	WindowGLFW::~WindowGLFW()
	{
		glfwDestroyWindow(m_window);
	}

	/*
	 * Determines if the window should be closed based on window events (such as close button)
	 */
	int WindowGLFW::WindowShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	/*
	 * Polls all events from GLFW (this may block on some platforms)
	 */
	void WindowGLFW::PollEvents()
	{
		m_scrollOffset.x = 0.0;
		m_scrollOffset.y = 0.0;
		glfwPollEvents();
	}

	/*
	 * Swaps buffers of a GLFW window
	 */
	void WindowGLFW::SwapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	/*
	 * Sets member variable to have a callback
	 */
	void WindowGLFW::SetResizeCallback(ResizeCallback callback)
	{
		m_resizeCallback = callback;
	}

	/*
	 * Returns framebuffer size struct
	 */
	FramebufferSize WindowGLFW::GetFramebufferSize()
	{
		return m_framebufferSize;
	}

	/*
	 * Returns mouse position struct
	 */
	MousePosition WindowGLFW::GetCursorPosition()
	{
		MousePosition mousePosition = {};
		glfwGetCursorPos(m_window, &mousePosition.x, &mousePosition.y);
		return mousePosition;
	}

	/*
	 * Sets cursor position
	 */
	void WindowGLFW::SetCursorPosition(MousePosition position)
	{
		glfwSetCursorPos(m_window, position.x, position.y);
	}

	/*
	 * Sets cursor mode
	 */
	void WindowGLFW::SetCursorMode(CursorMode mode)
	{
		switch (mode) {
			case CursorMode::NORMAL:
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			case CursorMode::HIDDEN:
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
		}
	}

	bool WindowGLFW::IsKeyPressed(KeyCode key)
	{
		int state = glfwGetKey(m_window, (int)key);
		if (state == GLFW_PRESS) {
			return true;
		}
		else {
			return false;
		}
	}

	bool WindowGLFW::IsMouseButtonPressed(MouseButtonCode button)
	{
		int state = glfwGetMouseButton(m_window, (int)button);
		if (state == GLFW_PRESS) {
			return true;
		}
		else {
			return false;
		}
	}

	void WindowGLFW::SetScrollCallback(ScrollCallback callback)
	{
		m_scrollCallback = callback;
	}

	ScrollOffset WindowGLFW::GetScrollOffset()
	{
		return m_scrollOffset;
	}
}