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
# include <emscripten/html5.h>
# include "WebcamDetail.hpp"


namespace s3d
{
	using s3dVideoCallback = void(*)(void*, int);

	EM_IMPORT(s3dOpenVideo)
	extern void s3dOpenVideo(int width, int height, s3dVideoCallback callback, void* callbackArg);

	EM_IMPORT(s3dQueryCurrentTime)
	extern double s3dQueryCurrentTime(GLuint idx);

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
		open();
	}

	Webcam::WebcamDetail::~WebcamDetail()
	{
		if (!m_initialized)
		{
			return;
		}

		stop();

		::glDeleteFramebuffers(2, m_videoBufferFrameBuffer);
		::glDeleteBuffers(2, m_videoPixelBuffer);
		::glDeleteTextures(1, &m_videoBufferTexture);
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
		::emscripten_request_animation_frame(OnUpdate, data);
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
		resize(m_currentResolution);

		return true;
	}

	bool Webcam::WebcamDetail::hasNewFrame() const
	{
		if (!m_initialized)
		{
			return false;
		}

		return (m_state == WebcamState::Active && (m_newFrameCount > 0));
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

		auto selectedFrame = m_totalFrameCount % 2;
		auto& pixelReadSync = m_pixelReadSync[selectedFrame];

		if (pixelReadSync)
		{
			auto waitResult = ::glClientWaitSync(pixelReadSync, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
			
			if (waitResult == GL_TIMEOUT_EXPIRED || waitResult == GL_WAIT_FAILED)
			{
				return false;
			}

			::glDeleteSync(pixelReadSync);
			pixelReadSync = nullptr;
		}

		::glBindBuffer(GL_PIXEL_PACK_BUFFER, m_videoPixelBuffer[selectedFrame]);
		{	
			::glGetBufferSubData(GL_PIXEL_PACK_BUFFER, 0, m_currentResolution.x * m_currentResolution.y * 4, image.data());	
		}
		::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

		m_newFrameCount = 0;

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

		::glGenTextures(1, &m_videoBufferTexture);
		::glBindTexture(GL_TEXTURE_2D, m_videoBufferTexture);
		{
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		::glBindTexture(GL_TEXTURE_2D, 0);

		::glGenBuffers(2, m_videoPixelBuffer);
		::glGenFramebuffers(2, m_videoBufferFrameBuffer);

		for (auto& syncObject : m_pixelReadSync)
		{
			syncObject = nullptr;			
		}

		resize(m_currentResolution);
		s3dOpenVideo(m_currentResolution.x, m_currentResolution.y, WebcamDetail::OnOpened, this);
	}

	void Webcam::WebcamDetail::resize(Size newSize)
	{
		::glBindTexture(GL_TEXTURE_2D, m_videoBufferTexture);
		{
			::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, newSize.x, newSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		::glBindTexture(GL_TEXTURE_2D, 0);

		for (const auto& pixelBuffer : m_videoPixelBuffer)
		{
			::glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBuffer);
			::glBufferData(GL_PIXEL_PACK_BUFFER, newSize.x * newSize.y * 4, nullptr, GL_DYNAMIC_READ);			
		}

		::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

		::glBindFramebuffer(GL_FRAMEBUFFER, m_videoBufferFrameBuffer[0]);
		{
			::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_videoBufferTexture, 0);

			if (::glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
			{
				LOG_ERROR(U"Incomplete Framebuffer");
				m_state = WebcamState::HasError;
				return;
			}
		}
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Webcam::WebcamDetail::OnOpened(void* data, int idx) 
	{
		WebcamDetail* that = static_cast<WebcamDetail*>(data);

		if (!idx)
		{
			LOG_ERROR(U"cv::VideoCapture::open({}) failed"_fmt(that->m_index));
			that->m_state = WebcamState::HasError;
			return;		
		}

		that->m_videoElementID = idx;
		that->m_initialized = true;
		that->m_state = WebcamState::Ready;

		LOG_INFO(U"cv::VideoCapture::open({}) succeeded"_fmt(that->m_index));
	}

	int Webcam::WebcamDetail::OnUpdate(double, void* data) 
	{
		WebcamDetail* that = static_cast<WebcamDetail*>(data);

		if (that->m_state == WebcamState::Stop)
		{
			return 0;
		}

		double currentTime = s3dQueryCurrentTime(that->m_videoElementID);

		if (that->m_lastFrameGetTime != currentTime)
		{
			for (GLuint i = 0; i < 8; i++) {
				::glBindSampler(i, 0);
			}

			{
				::glBindTexture(GL_TEXTURE_2D, that->m_videoBufferTexture);
				s3dBindVideo(GL_TEXTURE_2D, 0, GL_RGBA8, that->m_currentResolution.x, that->m_currentResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, that->m_videoElementID);
				::glBindTexture(GL_TEXTURE_2D, 0);
			}

			{
				auto selectedFrame = that->m_totalFrameCount % 2;
				auto& selectedSyncObject = that->m_pixelReadSync[selectedFrame];

				if (selectedSyncObject)
				{
					::glDeleteSync(selectedSyncObject);
				}

				::glBindFramebuffer(GL_FRAMEBUFFER, that->m_videoBufferFrameBuffer[0]);
				::glBindBuffer(GL_PIXEL_PACK_BUFFER, that->m_videoPixelBuffer[selectedFrame]);
				{
					::glPixelStorei(0x9240 /*GL_UNPACK_FLIP_Y_WEBGL*/, GL_TRUE);
					::glReadPixels(0, 0, that->m_currentResolution.x, that->m_currentResolution.y, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					::glPixelStorei(0x9240 /*GL_UNPACK_FLIP_Y_WEBGL*/, GL_FALSE);				
				}
				::glBindFramebuffer(GL_FRAMEBUFFER, 0);
				::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		
				selectedSyncObject = ::glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);	
			}

			++(that->m_totalFrameCount);
			++(that->m_newFrameCount);
			that->m_lastFrameGetTime = currentTime;
		}

		::emscripten_request_animation_frame(OnUpdate, data);
		return 0;
	}
}
