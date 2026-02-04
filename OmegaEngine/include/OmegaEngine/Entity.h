#pragma once
#include <OmegaEngine/Macros.h>
#include <OmegaEngine/Renderer/Renderer.h>
#include <GLM/gtc/quaternion.hpp>

namespace Omega {
	struct OMEGA_API Transform {
		glm::vec3 position { 0.0f };
		glm::quat rotation { 1, 0, 0, 0 };
		glm::vec3 scale { 1.0f };

		glm::mat4 ToMatrix() const {
			glm::mat4 mat(1.0f);
			mat = glm::translate(mat, position);
			mat *= glm::mat4_cast(rotation);
			mat = glm::scale(mat, scale);
			return mat;
		}
	};

	class OMEGA_API Entity {
	public:
		MeshHandle mesh;
		MaterialHandle material;
		Transform transform;
	};
}