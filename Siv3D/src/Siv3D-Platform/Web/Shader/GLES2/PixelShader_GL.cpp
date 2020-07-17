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

# include <Siv3D/SamplerState.hpp>
# include <Siv3D/EngineLog.hpp>
# include "PixelShader_GL.hpp"

namespace s3d
{
	PixelShader_GL::PixelShader_GL(Null)
	{
		m_initialized = true;
	}
	
	PixelShader_GL::~PixelShader_GL()
	{
		if (m_pixelShader)
		{
			::glDeleteShader(m_pixelShader);
		}
	}
	
	PixelShader_GL::PixelShader_GL(const String& source)
	{
		m_pixelShader = ::glCreateShader(GL_FRAGMENT_SHADER);

		if (!m_pixelShader) {
			LOG_FAIL(U"❌ Pixel shader compilation failed: failed to create shader.");
		}

        // シェーダのコンパイル
		{
			const std::string sourceUTF8 = source.toUTF8();
			const char* pSource = sourceUTF8.c_str();

			::glShaderSource(m_pixelShader, 1, &pSource, NULL);
			::glCompileShader(m_pixelShader);

			GLint status = GL_FALSE;
			::glGetShaderiv(m_pixelShader, GL_COMPILE_STATUS, &status);
			
			GLint logLen = 0;
			::glGetShaderiv(m_pixelShader, GL_INFO_LOG_LENGTH, &logLen);	

			// ログメッセージ
			if (logLen > 4)
			{
				std::string log(logLen + 1, '\0');
				::glGetShaderInfoLog(m_pixelShader, logLen, &logLen, &log[0]);
				LOG_FAIL(U"❌ Pixel shader compilation failed: {0}"_fmt(Unicode::Widen(log)));
			}	

            if (status == GL_FALSE) {
                ::glDeleteShader(m_pixelShader);
                m_pixelShader = 0;
            }
		}
	
		m_initialized = (m_pixelShader != 0);
	}
	
	bool PixelShader_GL::isInitialized() const noexcept
	{
		return m_initialized;
	}
	
	GLint PixelShader_GL::getShader() const
	{
		return m_pixelShader;
	}
	
	void PixelShader_GL::setPSSamplerUniform()
	{
		GLint m_psProgram;

		glGetIntegerv(GL_CURRENT_PROGRAM, &m_psProgram);

		if (!m_psProgram)
		{
			LOG_FAIL(U"Current program is null!");
			return;
		}

		if (!m_textureIndices)
		{
			for (uint32 slot = 0; slot < SamplerState::MaxSamplerCount; ++slot)
			{
				const String name = Format(U"Texture", slot);
				const std::string s = name.narrow();
				const GLint location = ::glGetUniformLocation(m_psProgram, s.c_str());
				
				if (location != -1)
				{
					LOG_DEBUG(U"{} location: {}"_fmt(name, location));
					m_textureIndices.emplace_back(slot, location);
				}
			}
		}

		for (auto[slot, location] : m_textureIndices)
		{
			::glUniform1i(location, slot);
		}
	}

	void PixelShader_GL::loadUniformVariables(const void* uniformBuffer)
	{
		GLint m_psProgram;

		glGetIntegerv(GL_CURRENT_PROGRAM, &m_psProgram);

		if (!m_psProgram)
		{
			LOG_FAIL(U"Current program is null!");
			return;
		}

		GLint uniformCount;

		::glGetProgramiv(m_psProgram, GL_ACTIVE_UNIFORMS, &uniformCount);

		for (auto[name, index] : m_constantBufferBindings)
		{
			const GLuint uniformIndex = ::glGetUniformLocation(m_psProgram, name.narrow().c_str());
		
			if (uniformIndex == GL_INVALID_INDEX)
			{
				LOG_TRACE(U"Uniform `{}` not found"_fmt(name));
				return;
			}

			GLint uniformSize;
			GLuint uniformType;
			GLchar uniformName[4];
			GLint uniformNameWritten;
			
			// ::glGetActiveUniform(m_psProgram, uniformIndex, 0, &uniformNameWritten, &uniformSize, &uniformType, uniformName);
			uniformSize = 1;
			uniformType = GL_FLOAT_VEC4;

			switch (uniformType)
			{
			case GL_FLOAT:
				::glUniform1fv(uniformIndex, uniformSize, static_cast<const GLfloat*>(uniformBuffer) + index);
				break;
			case GL_FLOAT_VEC2:
				::glUniform2fv(uniformIndex, uniformSize, static_cast<const GLfloat*>(uniformBuffer) + index);
				break;
			case GL_FLOAT_VEC3:
				::glUniform3fv(uniformIndex, uniformSize, static_cast<const GLfloat*>(uniformBuffer) + index);
				break;
			case GL_FLOAT_VEC4:
				::glUniform4fv(uniformIndex, uniformSize, static_cast<const GLfloat*>(uniformBuffer) + index);
				break;
			default:
				LOG_FAIL(U"Uniform `{}` type `{}` is not recognized"_fmt(name, uniformType));
				break;
			}
		}
	}
	
	void PixelShader_GL::setUniformBinding(const String& name, const GLuint index)
	{
		const GLuint uniformBlockBinding = Shader::Internal::MakeUniformBlockBinding(ShaderStage::Pixel, index);

		LOG_DEBUG(U"Uniform block `{}`: binding = PS_{} ({})"_fmt(name, index,uniformBlockBinding));

		ConstantBufferBinding cbBinding;

		cbBinding.name = name;
		cbBinding.index = uniformBlockBinding;

		m_constantBufferBindings << cbBinding;
	}
	
	void PixelShader_GL::setUniformBindings(const Array<ConstantBufferBinding>& bindings)
	{
		for (auto[name, index] : bindings)
		{
			setUniformBinding(name, index);
		}
	}
}
