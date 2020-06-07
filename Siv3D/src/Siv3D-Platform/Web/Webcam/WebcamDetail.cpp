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

# include <Siv3D/DynamicTexture.hpp>
# include <Siv3D/EngineLog.hpp>
# include <Siv3D/System.hpp>
# include <Siv3DEngine.hpp>
# include <Texture/GL/CTexture_GL.hpp>
# include <emscripten.h>
# include "WebcamDetail.hpp"


namespace s3d
{
	using s3dVideoCallback = void(*)(void*, int);

	EM_IMPORT(s3dOpenVideo)
	extern void s3dOpenVideo(s3dVideoCallback callback, void* callbackArg);

	EM_IMPORT(s3dBindVideo)
	extern void s3dBindVideo(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, GLuint idx);

	EM_IMPORT(s3dPlayVideo)
	extern void s3dPlayVideo(GLuint idx, s3dVideoCallback callback, void* callbackArg);

	EM_IMPORT(s3dStopVideo)
	extern void s3dStopVideo(GLuint idx);

	Webcam::WebcamDetail::WebcamDetail()
	{

	}

	Webcam::WebcamDetail::WebcamDetail(const size_t index)
		: m_index(static_cast<int32>(index))
	{
		m_currentResolution = { 640, 480 };
		open();
	}

	Webcam::WebcamDetail::~WebcamDetail()
	{
		if (!m_initialized)
		{
			return;
		}

		stop();
	}

	bool Webcam::WebcamDetail::isAvailable()
	{
		return m_initialized;
	}

	bool Webcam::WebcamDetail::start()
	{
		if (!m_initialized)
		{
			return false;
		}

		if (m_state == WebcamState::Active)
		{
			return true;
		}

		s3dPlayVideo(m_videoElementID, WebcamDetail::OnStart, this);
		m_state = WebcamState::Active;
		
		return true;
	}

	void Webcam::WebcamDetail::OnStart(void* data, int) 
	{
		WebcamDetail* that = static_cast<WebcamDetail*>(data);

		::glGenTextures(1, &that->m_videoBufferTexture);
		::glBindTexture(GL_TEXTURE_2D, that->m_videoBufferTexture);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, that->m_currentResolution.x, that->m_currentResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		::glGenFramebuffers(2, that->m_videoBufferFrameBuffer);
		::glBindFramebuffer(GL_FRAMEBUFFER, that->m_videoBufferFrameBuffer[0]);
		::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, that->m_videoBufferTexture, 0);

		if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		{
			LOG_ERROR(U"Incomplete Framebuffer");
		}

		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Webcam::WebcamDetail::stop()
	{
		if (!m_initialized)
		{
			return;
		}

		s3dStopVideo(m_videoElementID);

		m_state = WebcamState::None;
	}

	bool Webcam::WebcamDetail::isActive() const
	{
		if (!m_initialized)
		{
			return false;
		}

		return (m_state == WebcamState::Active);
	}

	size_t Webcam::WebcamDetail::index() const
	{
		if (!m_initialized)
		{
			return 0;
		}

		return m_index;
	}

	Size Webcam::WebcamDetail::getResolution() const
	{
		return m_currentResolution;
	}

	bool Webcam::WebcamDetail::setResolution(const Size& resolution)
	{
		if (!m_initialized)
		{
			return false;
		}

		if (isActive())
		{
			return false;
		}

		m_currentResolution = resolution;

		return true;
	}

	bool Webcam::WebcamDetail::hasNewFrame() const
	{
		if (!m_initialized)
		{
			return false;
		}

		return (m_state == WebcamState::Active); // && (m_newFrameCount > 0);
	}

	bool Webcam::WebcamDetail::getFrame(Image& image)
	{
		if (!m_initialized)
		{
			return false;
		}

		if (m_state != WebcamState::Active)
		{
			return false;
		}

		image.resize(m_currentResolution);

		for (GLuint i = 0; i < 8; i++) {
			::glBindSampler(i, 0);
		}

		{
			::glBindTexture(GL_TEXTURE_2D, m_videoBufferTexture);
			s3dBindVideo(GL_TEXTURE_2D, 0, GL_RGBA8, m_currentResolution.x, m_currentResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_videoElementID);
			::glBindTexture(GL_TEXTURE_2D, 0);
		}

		{
			::glBindFramebuffer(GL_FRAMEBUFFER, m_videoBufferFrameBuffer[0]);
			{
				::glPixelStorei(0x9240 /*GL_UNPACK_FLIP_Y_WEBGL*/, GL_TRUE);
				::glReadPixels(0, 0, m_currentResolution.x, m_currentResolution.y, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
				::glPixelStorei(0x9240 /*GL_UNPACK_FLIP_Y_WEBGL*/, GL_FALSE);
			}
			::glBindFramebuffer(GL_FRAMEBUFFER, 0);

			m_newFrameCount = 0;
		}

		return true;
	}

	bool Webcam::WebcamDetail::getFrame(DynamicTexture& texture)
	{
		if (!m_initialized)
		{
			return false;
		}

		if (m_state != WebcamState::Active)
		{
			return false;
		}

		for (GLuint i = 0; i < 8; i++) {
			::glBindSampler(i, 0);
		}

		{
			::glBindTexture(GL_TEXTURE_2D, m_videoBufferTexture);
			s3dBindVideo(GL_TEXTURE_2D, 0, GL_RGBA8, m_currentResolution.x, m_currentResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_videoElementID);
			::glBindTexture(GL_TEXTURE_2D, 0);
		}

		auto textureManager = dynamic_cast<CTexture_GL*>(Siv3DEngine::Get<ISiv3DTexture>());

		{
			::glBindFramebuffer(GL_READ_FRAMEBUFFER, m_videoBufferFrameBuffer[0]);
			::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_videoBufferFrameBuffer[1]);
			::glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureManager->getTexture(texture.id()), 0);
			
			::glBlitFramebuffer(
				0, 0, m_currentResolution.x, m_currentResolution.y,
				0, 0, texture.width(), texture.height(),
				GL_COLOR_BUFFER_BIT, GL_NEAREST
			);

			::glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

			::glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
			::glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		}

		m_newFrameCount = 0;

		return true;
	}

	void Webcam::WebcamDetail::open()
	{
		LOG_TRACE(U"Webcam::WebcamDetail::open()");
		s3dOpenVideo(WebcamDetail::OnOpened, this);
	}

	void Webcam::WebcamDetail::OnOpened(void* data, int idx) 
	{
		WebcamDetail* that = static_cast<WebcamDetail*>(data);

		if (idx)
		{
			LOG_INFO(U"cv::VideoCapture::open({}) succeeded"_fmt(that->m_index));
			that->m_videoElementID = idx;
			that->m_initialized = true;
			that->m_state = WebcamState::Ready;
		}
		else
		{
			LOG_ERROR(U"cv::VideoCapture::open({}) failed"_fmt(that->m_index));
			that->m_state = WebcamState::HasError;
		}
	}
}
