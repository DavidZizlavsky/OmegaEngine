#pragma once
#include "OmegaEngine/Macros.h"
#include "OmegaEngine/Window.h"
#include "OmegaEngine/Renderer.h"

namespace Omega {
	class OMEGA_API Engine {
	private:
		WindowSystem m_windowSystem;
		Window* m_window = nullptr;
		Renderer* m_renderer = nullptr;
	public:
		void Init();
		void Shutdown();
	};
}