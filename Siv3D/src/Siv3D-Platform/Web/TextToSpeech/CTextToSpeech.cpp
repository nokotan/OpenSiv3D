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
# include "CTextToSpeech.hpp"

namespace s3d
{
	namespace detail
	{
		struct AvailableLanguageList
		{
			struct AvailableLanguage
			{
				int32 languageCode;

				bool isDefault;
			};

			size_t length;

			AvailableLanguage* data;
		};

		__attribute__((import_name("s3dEnumerateAvailableTextToSpeechLanguages")))
		extern AvailableLanguageList s3dEnumerateAvailableTextToSpeechLanguages();

		__attribute__((import_name("s3dStartTextToSpeechLanguages")))
		extern void s3dStartTextToSpeechLanguages(const char* text, double rate, double volume, int32 code);

		__attribute__((import_name("s3dIsSpeakingTextToSpeechLanguages")))
		extern bool s3dIsSpeakingTextToSpeechLanguages();

		__attribute__((import_name("s3dPauseTextToSpeechLanguages")))
		extern void s3dPauseTextToSpeechLanguages();

		__attribute__((import_name("s3dResumeTextToSpeechLanguages")))
		extern void s3dResumeTextToSpeechLanguages();
	}

	CTextToSpeech::CTextToSpeech() {}

	CTextToSpeech::~CTextToSpeech()
	{
		LOG_TRACE(U"CTextToSpeech::~CTextToSpeech()");
	}

	void CTextToSpeech::init()
	{
		LOG_TRACE(U"CTextToSpeech::init()");

		auto list = detail::s3dEnumerateAvailableTextToSpeechLanguages();

		if (list.length > 0) 
		{
			LOG_INFO(U"ℹ️ TextToSpeech available");
		}
		else
		{
			LOG_WARNING(U"⚠️ TextToSpeech unavailable");
		}

		for (uint32 i = 0; i < list.length; ++i)
		{
			auto& item = list.data[i];

			m_AvailableLanguages << item.languageCode;

			if (item.isDefault)
			{
				m_DefaultLanguage = item.languageCode;
				
				LOG_INFO(U"LanguageCode: {} - Default"_fmt(item.languageCode));
			}
			else
			{
				LOG_INFO(U"LanguageCode: {}"_fmt(item.languageCode));
			}
		}

		::free(list.data);

		LOG_INFO(U"ℹ️ TextToSpeech initialized");
	}

	bool CTextToSpeech::hasLanguage(const int32 languageCode)
	{
		return m_AvailableLanguages.includes(languageCode);
	}

	void CTextToSpeech::setDefaultLanguage(const int32 languageCode)
	{
		m_DefaultLanguage = languageCode;
	}

	int32 CTextToSpeech::getDefaultLanguage() const
	{
		return m_DefaultLanguage;
	}

	bool CTextToSpeech::speak(const StringView text, const int32 languageCode)
	{
		const int32 lang = (languageCode == 0) ? m_DefaultLanguage : languageCode;

		detail::s3dStartTextToSpeechLanguages(Unicode::ToUTF8(text).c_str(), m_Speed, m_Volume, lang);
		return (true);
	}

	bool CTextToSpeech::isSpeaking() const
	{
		return detail::s3dIsSpeakingTextToSpeechLanguages();
	}

	void CTextToSpeech::pause()
	{
		detail::s3dPauseTextToSpeechLanguages();
	}

	void CTextToSpeech::resume()
	{
		detail::s3dResumeTextToSpeechLanguages();
	}

	void CTextToSpeech::setVolume(const double volume)
	{
		m_Volume = volume;
	}

	double CTextToSpeech::getVolume() const
	{
		return m_Volume;
	}

	void CTextToSpeech::setSpeed(const double speed)
	{
		m_Speed = speed;
	}

	double CTextToSpeech::getSpeed() const
	{
		// [Siv3D ToDo]
		return m_Speed;
	}
}
