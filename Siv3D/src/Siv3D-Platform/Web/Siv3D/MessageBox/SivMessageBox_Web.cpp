﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/MessageBox.hpp>
# include <Siv3D/AsyncTask.hpp>
# include <Siv3D/Monitor.hpp>
# include <Siv3D/Scene.hpp>
# include <Siv3D/Error.hpp>

namespace s3d
{
	namespace detail
	{
		enum class MessageBoxButtons
		{
			OK,

			OKCancel,

			YesNo,
		};

		extern "C" s3d::MessageBoxResult s3dShowMessageBox(const char* message, MessageBoxButtons type);

		MessageBoxResult ShowMessageBox_Web(const StringView title, const StringView text, MessageBoxStyle style, MessageBoxButtons buttons)
		{
			return s3dShowMessageBox(text.narrow().c_str(), buttons);
		}
	}

	namespace System
	{
		MessageBoxResult MessageBoxOK(const StringView title, const StringView text, const MessageBoxStyle style)
		{
			return detail::ShowMessageBox_Web(title, text, style, detail::MessageBoxButtons::OK);
		}

		MessageBoxResult MessageBoxOKCancel(const StringView title, const StringView text, const MessageBoxStyle style)
		{
			return detail::ShowMessageBox_Web(title, text, style, detail::MessageBoxButtons::OKCancel);
		}

		MessageBoxResult MessageBoxYesNo(const StringView title, const StringView text, const MessageBoxStyle style)
		{
			return detail::ShowMessageBox_Web(title, text, style, detail::MessageBoxButtons::YesNo);
		}
	}
}