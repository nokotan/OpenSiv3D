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
}
