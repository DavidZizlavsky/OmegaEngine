#pragma once
#include "OmegaEngine/Renderer.h"

namespace Omega {
	class RendererOpenGL final : public Renderer {
	public:
		RendererOpenGL();
		~RendererOpenGL() override;

#ifdef DEBUG
		void Debug() override;
#endif
		void FrameBegin() override;
	};
}