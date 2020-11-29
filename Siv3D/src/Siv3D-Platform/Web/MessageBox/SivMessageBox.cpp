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

# include <Siv3D/MessageBox.hpp>
# define SIV3D_CONCURRENT
# include <Siv3D/ConcurrentTask.hpp>
# include <Siv3D/Monitor.hpp>
# include <Siv3D/Scene.hpp>
# include <Siv3D/Error.hpp>

extern "C" s3d::MessageBoxSelection s3dShowMessageBox(const char* message, s3d::MessageBoxButtons type);

namespace s3d
{
	MessageBoxSelection ShowMessageBox_Web(const char* title, const char* message, MessageBoxStyle style, MessageBoxButtons buttons)
	{
		return s3dShowMessageBox(message, buttons);
	}

	namespace System
	{
		MessageBoxSelection ShowMessageBox(const String& title, const String& text, MessageBoxStyle style, MessageBoxButtons buttons)
		{
			return ShowMessageBox_Web(title.narrow().c_str(), text.narrow().c_str(), style, buttons);
		}
	}
}
