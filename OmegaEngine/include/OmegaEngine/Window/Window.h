#pragma once
#include "OmegaEngine/Macros.h"
#include <functional>

namespace Omega {
	struct OMEGA_API FramebufferSize {
		int width = 0;
		int height = 0;
	};

	class OMEGA_API Window {
	public:
		using ResizeCallback = std::function<void(int width, int height)>;
		virtual ~Window() = default;

		virtual int WindowShouldClose() = 0;
		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual void SetResizeCallback(ResizeCallback callback) = 0;
		virtual FramebufferSize GetFramebufferSize() = 0;
	};

	enum class OMEGA_API WindowLib {
		None = 0,
		GLFW = 1
	};

	class OMEGA_API WindowSystem {
	private:
		WindowLib m_windowLib = WindowLib::None;
	public:
		WindowSystem();
		~WindowSystem();

		Window* CreateWindow(int width, int height, const char* title);
	};
}