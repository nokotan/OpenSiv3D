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

# include <Siv3D/Dialog.hpp>
# include <Siv3D/FileSystem.hpp>
# include <Siv3D/Wave.hpp>
# include <Siv3D/Audio.hpp>

namespace s3d
{
	namespace Dialog
	{
		Optional<FilePath> OpenFile(const Array<FileFilter>& filters, const FilePath& defaultPath, const String&)
		{
			// [Siv3D ToDo]
			return none;
		}
		
		Optional<FilePath> SaveFile(const Array<FileFilter>& filters, const FilePath& defaultPath, const String&)
		{
			// [Siv3D ToDo]
			return none;
		}
		
		Optional<FilePath> SelectFolder(const FilePath& defaultPath, const String&)
		{
			// [Siv3D ToDo]
			return none;
		}
	}

	namespace Platform::Web::Dialog
	{
		namespace detail
		{
			template <class T>
			using s3dOpenDialogCallback = void (*)(std::promise<T>*, char*);

			template <class T>
			EM_IMPORT(s3dOpenDialog)
			void s3dOpenDialogImpl(s3dOpenDialogCallback<T>, std::promise<T>*);

			template <class T>
			void OnOpenFileDialogClosed(std::promise<T>* result, char* fileName)
			{
				if (fileName == 0)
				{
					result->set_value(T{});
				}
				else
				{
					auto path = Unicode::Widen(fileName);
					result->set_value(T{path});
				}

				delete result;
			}

			template <class T>
			std::future<T> s3dOpenDialog()
			{
				auto result = new std::promise<T>();
				auto result_future = result->get_future();
				
				s3dOpenDialogImpl<T>(&OnOpenFileDialogClosed<T>, result);
				
				return result_future;
			}
		}

		std::future<Optional<FilePath>> OpenFile(const Array<FileFilter>& filters, const FilePath& defaultPath, const String&)
		{
			return detail::s3dOpenDialog<Optional<FilePath>>();
		}



		std::future<Image> OpenImage(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Image>();
		}

		std::future<Texture> OpenTexture(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Texture>();
		}

		std::future<Texture> OpenTexture(const TextureDesc desc, const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Texture>();
		}

		std::future<Wave> OpenWave(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Wave>();
		}

		std::future<Audio> OpenAudio(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Audio>();
		}

		std::future<Audio> OpenAudio(const Arg::loop_<bool> loop, const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Audio>();
		}
	}
}
