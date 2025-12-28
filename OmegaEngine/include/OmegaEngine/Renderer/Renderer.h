#pragma once
#include "OmegaEngine/Macros.h"
#include <cstdint>
#include <GLM/glm.hpp>

namespace Omega {
	using MeshHandle = uint32_t;
	using MaterialHandle = uint32_t;
	using RenderObjectHandle = uint32_t;
	using ShaderProgramHandle = uint32_t;

	struct OMEGA_API RenderObject {
		MeshHandle meshHandle;
		MaterialHandle materialHandle;
		glm::mat4 modelMatrix;
	};

	struct OMEGA_API MaterialObject {
		ShaderProgramHandle shaderProgramHandle;
		glm::vec4 color;
	};

	struct OMEGA_API ShaderProgramObject {
		const char* vertexShaderSource = nullptr;
		const char* fragmentShaderSource = nullptr;
	};

	struct OMEGA_API Vertex {
		glm::vec3 position;
	};

	struct OMEGA_API MeshObject {
		Vertex* vertices = nullptr;
		uint32_t verticesCount = 0;
		uint32_t* indices = nullptr;
		uint32_t indicesCount = 0;
	};

	class OMEGA_API Renderer {
	public:
		virtual ~Renderer() = default;

#ifdef DEBUG
		virtual void Debug() {};
#endif
		virtual void FrameBegin() = 0;
		virtual MeshHandle CreateMesh(MeshObject meshObject) = 0;
		virtual ShaderProgramHandle CreateShaderProgram(ShaderProgramObject shaderProgramObject) = 0;
		virtual MaterialHandle CreateMaterial(MaterialObject material) = 0;
		virtual RenderObjectHandle CreateRenderObject(RenderObject renderObject) = 0;
		virtual void FrameEnd() = 0;
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