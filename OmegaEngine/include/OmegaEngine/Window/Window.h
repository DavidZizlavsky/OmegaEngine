#pragma once
#include "OmegaEngine/Macros.h"

namespace Omega {
	class OMEGA_API Window {
	public:
		virtual ~Window() = default;

		virtual int WindowShouldClose() = 0;
		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;
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