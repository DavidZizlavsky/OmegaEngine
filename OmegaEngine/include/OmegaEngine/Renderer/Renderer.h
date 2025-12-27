#pragma once
#include "OmegaEngine/Macros.h"

namespace Omega {
	class OMEGA_API Renderer {
	public:
		virtual ~Renderer() = default;

#ifdef DEBUG
		virtual void Debug() {};
#endif
		virtual void FrameBegin() {}
		virtual void FrameEnd() {}
	};

	enum class OMEGA_API RendererLib {
		None = 0,
		OpenGL = 1
	};

	class OMEGA_API RendererSystem {
	private:
		RendererLib m_rendererLib = RendererLib::None;
	public:
		RendererSystem();
		~RendererSystem();

		Renderer* CreateRenderer();
	};
}