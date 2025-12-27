#pragma once
#include "OmegaEngine/Macros.h"
#include "OmegaEngine/Window/Window.h"
#include "OmegaEngine/Renderer/Renderer.h"

namespace Omega {
	class OMEGA_API Engine {
	private:
		WindowSystem m_windowSystem;
		Window* m_window = nullptr;
		RendererSystem m_rendererSystem;
		Renderer* m_renderer = nullptr;
	public:
		void Init();
		void Shutdown();
	};
}