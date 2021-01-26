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
# include "Fwd.hpp"
# include "Array.hpp"
# include "Optional.hpp"
# include "Texture.hpp"
# include "FileFilter.hpp"

# if SIV3D_PLATFORM(WEB)
#	include <future>
# endif

namespace s3d
{
	namespace Dialog
	{
		Optional<FilePath> OpenFile(const Array<FileFilter>& filters = {}, const FilePath& defaultPath = U"", const String& title = U"");

		Array<FilePath> OpenFiles(const Array<FileFilter>& filters = {}, const FilePath& defaultPath = U"", const String& title = U"");

		Optional<FilePath> SaveFile(const Array<FileFilter>& filters = {}, const FilePath& defaultPath = U"", const String& title = U"");

		Optional<FilePath> SelectFolder(const FilePath& defaultPath = U"", const String& title = U"");


		Image OpenImage(const FilePath& defaultPath = U"", const String& title = U"");

		Texture OpenTexture(const FilePath& defaultPath = U"", const String& title = U"");

		Texture OpenTexture(TextureDesc desc, const FilePath& defaultPath = U"", const String& title = U"");

		Wave OpenWave(const FilePath& defaultPath = U"", const String& title = U"");

		Audio OpenAudio(const FilePath& defaultPath = U"", const String& title = U"");

		Audio OpenAudio(Arg::loop_<bool> loop, const FilePath& defaultPath = U"", const String& title = U"");


		Optional<FilePath> SaveImage(const FilePath& defaultPath = U"", const String& title = U"");

		Optional<FilePath> SaveWave(const FilePath& defaultPath = U"", const String& title = U"");
	}

# if SIV3D_PLATFORM(WEB)

 	namespace Platform::Web::Dialog 
	{
        std::future<Optional<FilePath>> OpenFile(const Array<FileFilter>& filters = {}, const FilePath& defaultPath = U"", const String& title = U"");

		std::future<Array<FilePath>> OpenFiles(const Array<FileFilter>& filters = {}, const FilePath& defaultPath = U"", const String& title = U"");


		std::future<Image> OpenImage(const FilePath& defaultPath = U"", const String& title = U"");

		std::future<Texture> OpenTexture(const FilePath& defaultPath = U"", const String& title = U"");

		std::future<Texture> OpenTexture(TextureDesc desc, const FilePath& defaultPath = U"", const String& title = U"");

		std::future<Wave> OpenWave(const FilePath& defaultPath = U"", const String& title = U"");

		std::future<Audio> OpenAudio(const FilePath& defaultPath = U"", const String& title = U"");

		std::future<Audio> OpenAudio(Arg::loop_<bool> loop, const FilePath& defaultPath = U"", const String& title = U"");
    }

# endif
}
