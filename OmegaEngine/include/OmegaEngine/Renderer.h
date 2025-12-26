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
}