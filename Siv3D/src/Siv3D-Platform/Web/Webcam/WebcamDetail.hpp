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

# pragma once
# include <atomic>
# include <thread>
# include <GLFW/glfw3.h>
# include <Siv3D/Webcam.hpp>
# include <Siv3D/Image.hpp>
# include <Siv3D/Texture.hpp>

namespace s3d
{
	enum class WebcamState
	{
		None,

		Ready,

		Active,

		Stop,

		HasError,
	};

	class Webcam::WebcamDetail
	{
	private:

		bool m_initialized = false;

		int32 m_index = 0;

		GLuint m_videoBufferTexture;
		GLuint m_videoPixelBuffer[2];
		GLuint m_videoBufferFrameBuffer[2];
		GLuint m_videoElementID;

		Size m_currentResolution = Size(640, 480);

		std::atomic<WebcamState> m_state = WebcamState::None;

		std::atomic<int32> m_newFrameCount = 0;
		int32 m_totalFrameCount = 0;
		double m_lastFrameGetTime = 0.0;
		GLsync m_pixelReadSync[2];

		void open();
		void resize(Size);

		static void OnOpened(void* data, int succeeded);
		static void OnStart(void* data, int succeeded);
		static int OnUpdate(double, void* data);

	public:

		WebcamDetail();

		WebcamDetail(size_t index);

		~WebcamDetail();

		bool isAvailable();

		bool start();

		void stop();

		bool isActive() const;

		size_t index() const;

		Size getResolution() const;

		bool setResolution(const Size& resolution);

		bool hasNewFrame() const;

		bool getFrame(Image& image);

		bool getFrame(DynamicTexture& texture);
	};
}
