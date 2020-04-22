//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2019 Ryo Suzuki
//	Copyright (c) 2016-2019 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/EngineLog.hpp>
# include "VertexShader_GL.hpp"

namespace s3d
{
	VertexShader_GL::VertexShader_GL(Null)
	{
		m_initialized = true;
	}
	
	VertexShader_GL::~VertexShader_GL()
	{
		if (m_vertexShader)
		{
			::glDeleteShader(m_vertexShader);
		}
	}
	
	VertexShader_GL::VertexShader_GL(const String& source)
	{
		m_vertexShader = ::glCreateShader(GL_VERTEX_SHADER);

		if (!m_vertexShader) {
			LOG_FAIL(U"❌ Vertex shader compilation failed: failed to create shader.");
		}

        // シェーダのコンパイル
		{
			const std::string sourceUTF8 = source.toUTF8();
			const char* pSource = sourceUTF8.c_str();

			::glShaderSource(m_vertexShader, 1, &pSource, NULL);
			::glCompileShader(m_vertexShader);

			GLint status = GL_FALSE;
			::glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
			
			GLint logLen = 0;
			::glGetShaderiv(m_vertexShader, GL_INFO_LOG_LENGTH, &logLen);	

			// ログメッセージ
			if (logLen > 4)
			{
				std::string log(logLen + 1, '\0');
				::glGetShaderInfoLog(m_vertexShader, logLen, &logLen, &log[0]);
				LOG_FAIL(U"❌ Vertex shader compilation failed: {0}"_fmt(Unicode::Widen(log)));
			}	

            if (status == GL_FALSE) {
                ::glDeleteShader(m_vertexShader);
                m_vertexShader = 0;
            }
		}
		
		m_initialized = (m_vertexShader != 0);
	}
	
	bool VertexShader_GL::isInitialized() const noexcept
	{
		return m_initialized;
	}
	
	GLint VertexShader_GL::getShader() const
	{
		return m_vertexShader;
	}
	
	void VertexShader_GL::setVSParameters() {
		GLint m_vsProgram;

		glGetIntegerv(GL_CURRENT_PROGRAM, &m_vsProgram);

		if (!m_vsProgram)
		{
			LOG_FAIL(U"Current program is null!");
			return;
		}

		for (auto[name, index] : m_constantBufferBindings)
		{
			const GLuint blockIndex = ::glGetUniformBlockIndex(m_vsProgram, name.narrow().c_str());
		
			if (blockIndex == GL_INVALID_INDEX)
			{
				LOG_FAIL(U"Uniform block `{}` not found"_fmt(name));
				return;
			}

			::glUniformBlockBinding(m_vsProgram, blockIndex, index);
		}
	}

	void VertexShader_GL::setUniformBlockBinding(const String& name, const GLuint index)
	{
		const GLuint uniformBlockBinding = Shader::Internal::MakeUniformBlockBinding(ShaderStage::Vertex, index);
		
		LOG_DEBUG(U"Uniform block `{}`: binding = VS_{} ({})"_fmt(name, index,uniformBlockBinding));
		
		ConstantBufferBinding cbBinding;

		cbBinding.name = name;
		cbBinding.index = uniformBlockBinding;

		m_constantBufferBindings << cbBinding;
	}
	
	void VertexShader_GL::setUniformBlockBindings(const Array<ConstantBufferBinding>& bindings)
	{
		for (auto[name, index] : bindings)
		{
			setUniformBlockBinding(name, index);
		}
	}
}
