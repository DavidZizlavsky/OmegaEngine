#pragma once
#include "OmegaEngine/Macros.h"

namespace Omega {
	class OMEGA_API Window {
	public:
		~Window() = default;

		virtual int WindowShouldClose() = 0;
		virtual void PollEvents() = 0;
		virtual void SwapBuffers() = 0;
	};

	class OMEGA_API WindowSystem {
	public:
		WindowSystem();
		~WindowSystem();

		Window* CreateWindow(int width, int height, const char* title);
	};
}