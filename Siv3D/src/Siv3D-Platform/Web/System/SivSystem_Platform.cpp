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

# include <stdlib.h>
# include <unistd.h>
# include <Siv3D/System.hpp>
# include <Siv3D/FileSystem.hpp>

namespace s3d
{
	namespace System
	{
		void Sleep(int32 milliseconds)
		{
			if (milliseconds < 0)
			{
				return;
			}
			
			::usleep(static_cast<uint32>(milliseconds) * 1000);
		}
		
		bool LaunchBrowser(const FilePath& _url)
		{
			const bool isWebPage = _url.starts_with(U"http://") || _url.starts_with(U"https://");
			FilePath url = _url;
			
			if (!isWebPage)
			{
				return false;
			}
			
			EM_ASM({
				window.open(UTF32ToString($0), '_blank')
			}, url.c_str());

			return true;
		}
	}

	namespace Platform::Web::System
	{
		namespace detail
		{
			void RunMainLoop(void* userData)
			{
				const auto* mainLoop = static_cast<const std::function<void()>*>(userData);
				mainLoop->operator()();
			}
		}

		void SetMainLoop(std::function<void()> mainLoop)
		{
			/*
				emscripten_set_main_loop を呼ぶと
				JavaScript の例外を使って大域脱出が行われる。
			
				そのため、この関数の処理が戻ってくることはない上、
				スタックメモリのアンワインド処理も行われないため、
				Main 関数でラムダ式による変数の参照キャプチャを行なったり、
				この関数で mainLoop へのポインタを束縛したりしても
				メモリの領域外アクセスにはならない

				(ただし、アプリを閉じたときに Main 関数の変数のデストラクタが呼ばれることもないので
				注意が必要である)
			*/
			::emscripten_set_main_loop_arg(&detail::RunMainLoop, &mainLoop, 0, 1);
		}
	}
}
