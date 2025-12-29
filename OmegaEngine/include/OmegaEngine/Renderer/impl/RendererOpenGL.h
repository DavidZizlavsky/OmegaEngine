#pragma once
#include "OmegaEngine/Renderer/Renderer.h"
#include <vector>
#include <GLEW/glew.h>

namespace Omega {
	struct MeshBufferObject {
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	};

	class RendererOpenGL final : public Renderer {
	private:
		static bool isInitGLEW;
		std::vector<RenderObject> m_renderObjects;
		std::vector<bool> m_activeObjects;
		std::vector<GLuint> m_shaderPrograms;
		std::vector<MeshBufferObject> m_meshBufferObjects;

		GLuint CompileShader(GLenum type, const char* source);
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
		void FrameEnd() override {}

		static void InitGLEW();
	};
}