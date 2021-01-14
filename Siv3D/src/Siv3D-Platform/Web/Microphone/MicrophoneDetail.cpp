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

# include "MicrophoneDetail.hpp"
# include <Siv3D/EngineLog.hpp>
# include <emscripten/html5.h>
# include <algorithm>

namespace s3d
{
	void Microphone::MicrophoneDetail::InputCallback_S16_2ch(void* data)
	{
		Microphone::MicrophoneDetail* that = static_cast<Microphone::MicrophoneDetail*>(data);
		
		if (that->onRead_S16_2ch())
		{
			::emscripten_set_timeout(&Microphone::MicrophoneDetail::InputCallback_S16_2ch, 20.0, data);
		}
	}

	Microphone::MicrophoneDetail::MicrophoneDetail()
	{
		
	}
	
	Microphone::MicrophoneDetail::~MicrophoneDetail()
	{
		release();
	}
	
	bool Microphone::MicrophoneDetail::init(const Optional<size_t> deviceID, const RecordingFormat format, const size_t bufferLength, const bool loop)
	{
		m_samplingRate = detail::ToSamplingRate(format);
		m_loop = loop;
		
		if (bufferLength < 256)
		{
			LOG_FAIL(U"❌ Microphone: bufferLength is too short");
			return false;
		}
		
		m_device = ::alcCaptureOpenDevice(NULL, m_samplingRate, AL_FORMAT_MONO16, bufferLength);
		
		m_buffer.assign(bufferLength, WaveSampleS16::Zero());
		m_writePos = 0;
		
		m_initialized = true;
		
		return true;
	}
	
	bool Microphone::MicrophoneDetail::isAvailable() const
	{
		return m_initialized;
	}
	
	void Microphone::MicrophoneDetail::release()
	{
		LOG_TRACE(U"Microphone::MicrophoneDetail::release()");
		
		if (!m_initialized)
		{
			return;
		}
		
		if (m_device)
		{
			LOG_TRACE(U"::alcCloseDevice");

			::alcCloseDevice(m_device);
		}
		
		m_initialized = false;
	}
	
	bool Microphone::MicrophoneDetail::start()
	{
		if (!m_initialized || m_isRecording)
		{
			return false;
		}
		
		::alcCaptureStart(m_device);
		::emscripten_set_timeout(&Microphone::MicrophoneDetail::InputCallback_S16_2ch, 20.0, this);
		
		m_isRecording = true;
		
		return true;
	}
	
	void Microphone::MicrophoneDetail::stop()
	{
		if (!m_initialized || !m_isRecording)
		{
			return;
		}
		
		::alcCaptureStop(m_device);
		
		m_writePos = 0;
		
		m_buffer.fill(WaveSampleS16::Zero());
		
		m_isRecording = false;
	}
	
	bool Microphone::MicrophoneDetail::isRecording() const
	{
		return m_isRecording;
	}
	
	uint32 Microphone::MicrophoneDetail::samplingRate() const
	{
		return m_samplingRate;
	}
	
	const Array<WaveSampleS16>& Microphone::MicrophoneDetail::getBuffer() const
	{
		return m_buffer;
	}
	
	size_t Microphone::MicrophoneDetail::posSample() const
	{
		return m_writePos;
	}

	bool Microphone::MicrophoneDetail::onRead_S16_2ch()
	{
		if (!m_isRecording)
		{
			return false;
		}

		ALCint samples;

		::alcGetIntegerv(m_device, ALC_CAPTURE_SAMPLES, sizeof(samples), &samples);

		if (m_loop)
		{
			while (samples)
			{
				const size_t write_samples = std::min(static_cast<size_t>(samples), m_buffer.size() - m_writePos);
				
				::alcCaptureSamples(m_device, (ALCvoid*)(m_buffer.data() + m_writePos), write_samples);
				m_writePos += write_samples;
				
				if (m_writePos >= m_buffer.size())
				{
					m_writePos = 0;
				}
				
				samples -= write_samples;
			}
			
			return true;
		}
		else
		{
			if (m_writePos + samples > m_buffer.size())
			{
				samples = m_buffer.size() - m_writePos;
			}
			
			const size_t write_bytes = samples * (2 * sizeof(int16));
			
			::alcCaptureSamples(m_device, (ALCvoid*)(m_buffer.data() + m_writePos), write_bytes);

			m_writePos += samples;
			
			if (m_writePos >= m_buffer.size())
			{
				m_isRecording = false;
				
				return false;
			}
			
			return true;
		}
	}
}
