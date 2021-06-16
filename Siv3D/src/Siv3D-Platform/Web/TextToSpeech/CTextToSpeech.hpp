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
# include "TextToSpeech/ITextToSpeech.hpp"
# include <Siv3D/Array.hpp>

namespace s3d
{
	class CTextToSpeech : public ISiv3DTextToSpeech
	{
		Array<int32> m_AvailableLanguages;

		int32 m_DefaultLanguage;

		double m_Speed = 1.0;

		double m_Volume = 1.0;
	public:

		CTextToSpeech();

		~CTextToSpeech() override;

		void init() override;

		bool hasLanguage(int32 languageCode) override;

		void setDefaultLanguage(int32 languageCode) override;

		int32 getDefaultLanguage() const override;

		bool speak(StringView text, int32 languageCode) override;

		bool isSpeaking() const override;

		void pause() override;

		void resume() override;

		void setVolume(double volume) override;

		double getVolume() const override;

		void setSpeed(double speed) override;

		double getSpeed() const override;
	};
}
