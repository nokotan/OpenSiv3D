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
# include <Siv3D/EngineLog.hpp>
# include <Window/IWindow.hpp>
# include "CClipboard.hpp"

# include <future>

using s3dGetClipboardTextCallBack = void(*)(char* text, std::promise<s3d::String>* promise);

extern "C" {

	void s3dGetClipboardText(s3dGetClipboardTextCallBack, std::promise<s3d::String>*);
	void s3dSetClipboardText(const char* text);
}

namespace s3d
{
	CClipboard::CClipboard()
	{
	}

	CClipboard::~CClipboard()
	{
		LOG_TRACE(U"CClipboard::~CClipboard()");
	}

	void CClipboard::init()
	{
		LOG_TRACE(U"CClipboard::init()");

		m_window = static_cast<GLFWwindow*>(Siv3DEngine::Get<ISiv3DWindow>()->getHandle());

		LOG_INFO(U"ℹ️ CClipboard initialized");
	}

	bool CClipboard::hasChanged()
	{
		return false;
	}

	bool CClipboard::getText(String& text)
	{
		text = Unicode::Widen(glfwGetClipboardString(m_window));
		return !text.isEmpty();
	}

	bool CClipboard::getImage(Image& image)
	{
		image.clear();
		return !image.isEmpty();
	}

	bool CClipboard::getFilePaths(Array<FilePath>& paths)
	{
		paths.clear();
		return !paths.isEmpty();
	}

	void CClipboard::setText(const String& text)
	{
		s3dSetClipboardText(Unicode::Narrow(text).c_str());
	}

	void CClipboard::setImage(const Image& image)
	{
	}

	void CClipboard::clear()
	{
	}

	namespace Platform::Web::Clipboard 
	{
		namespace detail
		{
			void OnGetClipboardText(char* text, std::promise<s3d::String>* promise)
			{
				if (text)
				{
					promise->set_value(Unicode::Widen(text));
				}
				else
				{
					promise->set_value(U"");
				}
				
				delete promise;
			}
		}

		std::future<String> GetText()
		{
			auto p = new std::promise<s3d::String>();
			auto result_future = p->get_future();

			::s3dGetClipboardText(&detail::OnGetClipboardText, p);

			return result_future;
		}
	}
}
