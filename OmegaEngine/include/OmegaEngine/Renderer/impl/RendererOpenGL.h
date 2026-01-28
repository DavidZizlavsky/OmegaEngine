#pragma once
#include "OmegaEngine/Renderer/Renderer.h"
#include <vector>
#include <GLEW/glew.h>
#include <cstdint>

namespace Omega {
	struct MeshBufferObject {
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		uint32_t verticesCount;
		uint32_t indicesCount;
	};

	struct alignas(16) FrameData {
		glm::mat4 viewMatrix = 1;
		glm::mat4 projectionMatrix = 1;
		glm::mat4 viewProjectionMatrix = 1;
		glm::vec3 cameraPosition;
	};
	
	struct alignas(16) PointLightData {
		glm::vec4 position;
		glm::vec4 color;
	};

	struct alignas(16) LightData {
		glm::vec4 ambientColor;
		int lightCount = 0;
		PointLightData lights[16];
	};

	class RendererOpenGL final : public Renderer {
	private:
		static bool isInitGLEW;
		std::vector<RenderObject> m_renderObjects;
		std::vector<bool> m_activeObjects;
		std::vector<GLuint> m_shaderPrograms;
		std::vector<MeshBufferObject> m_meshBufferObjects;
		std::vector<MaterialObject> m_materialObjects;

		GLuint m_frameUBO = 0;
		GLuint m_lightUBO = 0;

		GLuint CompileShader(GLenum type, const char* source);
		void Draw(RenderObject renderObject);
	public:
		RendererOpenGL();
		~RendererOpenGL() override;

#ifdef DEBUG
		void Debug() override;
#endif
		void FrameBegin() override;
		MeshHandle CreateMesh(MeshObject meshObject) override;
		ShaderProgramHandle CreateShaderProgram(ShaderProgramObject shaderProgramObject) override;
		MaterialHandle CreateMaterial(MaterialObject material) override;
		RenderObjectHandle CreateRenderObject(RenderObject renderObject) override;
		void UpdateRenderObjectModelMatrix(RenderObjectHandle renderObjectHandle, glm::mat4 modelMatrix) override;
		void UpdateCameraData(CameraData cameraData) override;
		void UpdateLightData(std::vector<PointLight>& lights, glm::vec3 ambientColor) override;
		void FrameEnd() override;

		void ResizeFramebuffer(int width, int height) override;

		static void InitGLEW();
	};
}