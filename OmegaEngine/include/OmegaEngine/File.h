#pragma once
#include "OmegaEngine/Macros.h"
#include "OmegaEngine/Renderer/Renderer.h"
#include <string>

namespace Omega {
	class OMEGA_API File {
	public:
		static std::string LoadTextFile(const std::string& path);
		static MeshObject LoadMeshFromObj(const std::string& path);
	};
}