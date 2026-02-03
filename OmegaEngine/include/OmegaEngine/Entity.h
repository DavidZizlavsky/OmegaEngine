#pragma once
#include <OmegaEngine/Macros.h>
#include <OmegaEngine/Renderer/Renderer.h>
#include <GLM/gtc/quaternion.hpp>

namespace Omega {
	struct OMEGA_API Transform {
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	};

	class OMEGA_API Entity {
	public:
		Transform transform;
	};
}