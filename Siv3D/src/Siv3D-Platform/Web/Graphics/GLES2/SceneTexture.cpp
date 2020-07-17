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

# include <Siv3DEngine.hpp>
# include "SceneTexture.hpp"

namespace s3d
{
	namespace detail
	{
		static GLuint LoadShaders(const FilePath& vsFilePath, const FilePath& psFilePath)
		{
			const std::string vsSource = TextReader(vsFilePath).readAll().toUTF8();
			const std::string psSource = TextReader(psFilePath).readAll().toUTF8();
			
			if (vsSource.empty() || psSource.empty())
			{
				return 0;
			}
			
			GLuint vertexShader = ::glCreateShader(GL_VERTEX_SHADER);
			{
				const char* vsSourcePtr = vsSource.c_str();
				
				::glShaderSource(vertexShader, 1, & vsSourcePtr, nullptr);
				::glCompileShader(vertexShader);
				
				GLint status = GL_FALSE, infoLogLength = 0;
				::glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
				::glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
				
				if (infoLogLength > 4)
				{
					std::string logText(infoLogLength + 1, '\0');
					::glGetShaderInfoLog(vertexShader, infoLogLength, nullptr, logText.data());
					LOG_FAIL(U"VS: {0}"_fmt(Unicode::Widen(logText)));
				}
			}
			
			GLuint pixelShader = ::glCreateShader(GL_FRAGMENT_SHADER);
			{
				const char* psSourcePtr = psSource.c_str();
				
				::glShaderSource(pixelShader, 1, & psSourcePtr, nullptr);
				::glCompileShader(pixelShader);
				
				GLint status = GL_FALSE, infoLogLength = 0;
				::glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &status);
				::glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &infoLogLength);
				
				if (infoLogLength > 4)
				{
					std::string logText(infoLogLength + 1, '\0');
					::glGetShaderInfoLog(pixelShader, infoLogLength, nullptr, logText.data());
					LOG_FAIL(U"PS: {0}"_fmt(Unicode::Widen(logText)));
				}
			}
			
			GLuint program = ::glCreateProgram();
			{
				::glAttachShader(program, vertexShader);
				::glAttachShader(program, pixelShader);

				::glBindAttribLocation(program, 0, "VertexPosition");
				::glBindAttribLocation(program, 1, "UVPosition");

				::glLinkProgram(program);
				
				GLint status = GL_FALSE, infoLogLength = 0;
				::glGetProgramiv(program, GL_LINK_STATUS, &status);
				::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
				
				if (infoLogLength > 4)
				{
					std::string logText(infoLogLength + 1, '\0');
					::glGetProgramInfoLog(program, infoLogLength, nullptr, logText.data());
					LOG_FAIL(U"LINK: {0}"_fmt(Unicode::Widen(logText)));
				}
			}
			
			::glDetachShader(program, vertexShader);
			::glDetachShader(program, pixelShader);
			
			::glDeleteShader(vertexShader);
			::glDeleteShader(pixelShader);
			
			return program;
		}
	}

	static uint32 nextHigherPowerOfTwo(uint32 val)
	{
		uint32 nextHigher = val - 1;

		for (auto i : { 1, 2, 4, 8, 16 })
		{
			nextHigher = nextHigher | (nextHigher >> i);
		}

		return nextHigher + 1;
	}
	
	SceneTexture::SceneTexture()
	{
		
	}
	
	SceneTexture::~SceneTexture()
	{
	# if defined(SIV3D_USE_MULTISAMPLE)
		
		if (m_msTexture)
		{
			::glDeleteTextures(1, &m_msTexture);
			m_msTexture = 0;
		}
		
		if (m_msFrameBuffer)
		{
			::glDeleteFramebuffers(1, &m_msFrameBuffer);
			m_msFrameBuffer = 0;
		}
		
		if (m_resolvedTexture)
		{
			::glDeleteTextures(1, &m_resolvedTexture);
			m_resolvedTexture = 0;
		}
		
		if (m_resolvedFrameBuffer)
		{
			::glDeleteFramebuffers(1, &m_resolvedFrameBuffer);
			m_resolvedFrameBuffer = 0;
		}
		
	# else
		
		if (m_texture)
		{
			::glDeleteTextures(1, &m_texture);
			m_texture = 0;
		}
		
		if (m_frameBuffer)
		{
			::glDeleteFramebuffers(1, &m_frameBuffer);
			m_frameBuffer = 0;
		}
		
	# endif
		
		if (m_vertex)
		{
			::glDeleteBuffers(1, &m_vertex);
		}
		
		if (m_copyProgram)
		{
			::glDeleteProgram(m_copyProgram);
			m_copyProgram = 0;
		}
	}
	
	bool SceneTexture::init()
	{
	# if defined(SIV3D_USE_MULTISAMPLE)
		
		::glGenFramebuffers(1, &m_msFrameBuffer);
		::glGenFramebuffers(1, &m_resolvedFrameBuffer);
		
		::glGenTextures(1, &m_msTexture);
		::glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msTexture);
		::glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, m_size.x, m_size.y, GL_FALSE);
		
		::glGenTextures(1, &m_resolvedTexture);
		::glBindTexture(GL_TEXTURE_2D, m_resolvedTexture);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		::glBindFramebuffer(GL_FRAMEBUFFER, m_msFrameBuffer);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_msTexture, 0);
		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}
		
		::glBindFramebuffer(GL_FRAMEBUFFER, m_resolvedFrameBuffer);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_resolvedTexture, 0);
		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}
		
	# else
		
		::glGenFramebuffers(1, &m_frameBuffer);
		::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		
		Size textureSize { nextHigherPowerOfTwo(m_size.x), nextHigherPowerOfTwo(m_size.y) };
		Vector2D textureSizeRatio { static_cast<float>(m_size.x) / textureSize.x, static_cast<float>(m_size.y) / textureSize.y };

		::glGenTextures(1, &m_texture);
		::glBindTexture(GL_TEXTURE_2D, m_texture);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// ::glBindFramebuffer(GL_FRAMEBUFFER, m_texture);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);

	# endif
		
		::glGenBuffers(1, &m_vertex);
		::glBindBuffer(GL_ARRAY_BUFFER, m_vertex);
		{
			Array<Float4> vertex = {
				{ -1.0f, -1.0f, 0.0f, textureSizeRatio.y }, { -1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, -1.0f, textureSizeRatio.x, textureSizeRatio.y },
				{ -1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, textureSizeRatio.x, 0.0f }, { 1.0f, -1.0f, textureSizeRatio.x, textureSizeRatio.y },
			};
			::glBufferData(GL_ARRAY_BUFFER, sizeof(Float4) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
		}
		::glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_copyProgram = detail::LoadShaders(Resource(U"engine/shader/GLSL1.0/fullscreen_triangle.vert"), Resource(U"engine/shader/GLSL1.0/fullscreen_triangle.frag"));
		if (!m_copyProgram)
		{
			return false;
		}
		m_locationTexture = ::glGetUniformLocation(m_copyProgram, "Texture0");

		return true;
	}
	
	void SceneTexture::clear(const ColorF& color)
	{
	# if defined(SIV3D_USE_MULTISAMPLE)
		
		::glBindFramebuffer(GL_FRAMEBUFFER, m_msFrameBuffer);
		
	# else
		
		::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		
	# endif
		
		::glClearColor(
					   static_cast<float>(color.r),
					   static_cast<float>(color.g),
					   static_cast<float>(color.b),
					   1.0f);
		::glClear(GL_COLOR_BUFFER_BIT);
	}
	
	const Size& SceneTexture::getSize() const noexcept
	{
		return m_size;
	}
	
	void SceneTexture::resize(const Size& size, const ColorF& clearColor)
	{
		if (size == m_size)
		{
			return;
		}
		
	# if defined(SIV3D_USE_MULTISAMPLE)
		
		::glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_msTexture);
		::glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, size.x, size.y, GL_FALSE);
		
		::glBindTexture(GL_TEXTURE_2D, m_resolvedTexture);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		
	# else
		
		Size textureSize { nextHigherPowerOfTwo(m_size.x), nextHigherPowerOfTwo(m_size.y) };
		Vector2D textureSizeRatio { static_cast<float>(m_size.x) / textureSize.x, static_cast<float>(m_size.x) / textureSize.y };
		
		::glBindTexture(GL_TEXTURE_2D, m_texture);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		
		::glBindBuffer(GL_ARRAY_BUFFER, m_vertex);
		{
			Array<Float4> vertex = {
				{ -1.0f, -1.0f, 0.0f, textureSizeRatio.y }, { -1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, -1.0f, textureSizeRatio.x, textureSizeRatio.y },
				{ -1.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, textureSizeRatio.x, 0.0f }, { 1.0f, -1.0f, textureSizeRatio.x, textureSizeRatio.y },
			};
			::glBufferData(GL_ARRAY_BUFFER, sizeof(Float4) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
		}
		::glBindBuffer(GL_ARRAY_BUFFER, 0);

	# endif
		
		m_size = size;
		
		clear(clearColor);
	}  
	
	void SceneTexture::bindSceneFrameBuffer()
	{
	# if defined(SIV3D_USE_MULTISAMPLE)
		
		::glBindFramebuffer(GL_FRAMEBUFFER, m_msFrameBuffer);
		
	# else
		
		::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		
	# endif
	}
	
	void SceneTexture::resolve(const bool linearFilter)
	{
	# if defined(SIV3D_USE_MULTISAMPLE)
		
		::glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFrameBuffer);
		::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolvedFrameBuffer);
		::glBlitFramebuffer(0, 0, m_size.x, m_size.y, 0, 0, m_size.x, m_size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		auto[s, viewRect] = Siv3DEngine::Get<ISiv3DRenderer2D>()->getLetterboxingTransform();
		::glViewport(viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);
		
		::glUseProgram(m_copyProgram);
		{
			::glActiveTexture(GL_TEXTURE0);
			::glBindTexture(GL_TEXTURE_2D, m_resolvedTexture);
			::glUniform1i(m_locationTexture, 0);

			{
				::glBindBuffer(GL_ARRAY_BUFFER, 0);
				::glDrawArrays(GL_TRIANGLES, 0, 3);
			}
		}
		::glUseProgram(0);
			
		Siv3DEngine::Get<ISiv3DProfiler>()->reportDrawcalls(1, 1);
		
	# else
		
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
		auto[s, viewRect] = Siv3DEngine::Get<ISiv3DRenderer2D>()->getLetterboxingTransform();
		::glViewport(viewRect.left, viewRect.top, viewRect.right, viewRect.bottom);
		
		::glUseProgram(m_copyProgram);
		{
			::glActiveTexture(GL_TEXTURE0);
			::glBindTexture(GL_TEXTURE_2D, m_texture);
			::glUniform1i(m_locationTexture, 0);
			
			{
				::glBindBuffer(GL_ARRAY_BUFFER, m_vertex);

				::glEnableVertexAttribArray(0);
				::glEnableVertexAttribArray(1);

				::glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (GLubyte*)0);
				::glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (GLubyte*)8);

				::glDrawArrays(GL_TRIANGLES, 0, 6);
				::glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}
		::glUseProgram(0);
		
		Siv3DEngine::Get<ISiv3DProfiler>()->reportDrawcalls(1, 1);
		
	# endif
	}
	
	void SceneTexture::requestCapture()
	{
		m_screenCaptureRequested = true;
	}
	
	bool SceneTexture::hasCaptureRequest() const noexcept
	{
		return m_screenCaptureRequested;
	}
	
	void SceneTexture::capture()
	{
		m_screenCaptureRequested = false;
		
		if (m_size != m_image.size())
		{
			m_image.resize(m_size);
		}
		
# if defined(SIV3D_USE_MULTISAMPLE)
		::glBindFramebuffer(GL_FRAMEBUFFER, m_resolvedFrameBuffer);
# else
		::glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
# endif
		{
			::glReadPixels(0, 0, m_size.x, m_size.y, GL_RGBA, GL_UNSIGNED_BYTE, m_image.data());
		}
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	const Image& SceneTexture::getImage() const
	{
		return m_image;
	}
	
	void SceneTexture::setFrameBuffer(const GLuint frameBuffer)
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	}
}
