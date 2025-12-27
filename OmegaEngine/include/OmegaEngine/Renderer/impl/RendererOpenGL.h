#pragma once
#include "OmegaEngine/Renderer/Renderer.h"

namespace Omega {
	class RendererOpenGL final : public Renderer {
	private:
		static bool isInitGLEW;
	public:
		RendererOpenGL();
		~RendererOpenGL() override;

#ifdef DEBUG
		void Debug() override;
#endif
		void FrameBegin() override;

		static void InitGLEW();
	};
}