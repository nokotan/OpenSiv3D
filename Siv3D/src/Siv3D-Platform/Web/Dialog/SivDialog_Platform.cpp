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

extern "C"
{
	void s3dSaveDialog(const char* fileName);
}

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
			const auto fileName = FileSystem::FileName(defaultPath);
			::s3dSaveDialog(fileName.narrow().c_str());
			
			return FilePath(U"/dev/save");
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
			String TransformFileFilters(const Array<FileFilter>& filters) 
			{
				return filters
					.map([](const FileFilter& f) 
					{
						return f.patterns
							.map([](const String& p) 
							{
								return p.count(U'/') == 1 ? p : U"." + p;
							})
							.join(U",", U"", U"");
					})
					.join(U",", U"", U"");
			}

			template <class T>
			using s3dOpenDialogCallback = void (*)(char*, std::promise<T>*);

			template <class T>
			EM_IMPORT(s3dOpenDialog)
			void s3dOpenDialogImpl(const char*, s3dOpenDialogCallback<T>, std::promise<T>*);

			template <class T>
			void OnOpenFileDialogClosed(char* fileName, std::promise<T>* result)
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

				delete fileName;
				delete result;
			}

			void OnOpenAudioDialogClosed(char* fileName, std::promise<Audio>* result)
			{
				if (fileName == 0)
				{
					result->set_value(Audio{});
				}
				else
				{
					auto path = Unicode::Widen(fileName);
					AudioProcessing::DecodeAudioFromFile(path, std::move(*result));
				}

				delete fileName;
				delete result;
			}

			template <class T>
			std::future<T> s3dOpenDialog(const Array<FileFilter>& filters)
			{
				const auto filter = TransformFileFilters(filters);

				auto result = new std::promise<T>();
				auto result_future = result->get_future();
				
				s3dOpenDialogImpl<T>(filter.narrow().c_str(), &OnOpenFileDialogClosed<T>, result);
				
				return result_future;
			}

			std::future<Audio> s3dOpenAudio(const Array<FileFilter>& filters)
			{
				const auto filter = TransformFileFilters(filters);

				auto result = new std::promise<Audio>();
				auto result_future = result->get_future();
				
				s3dOpenDialogImpl<Audio>(filter.narrow().c_str(), &OnOpenAudioDialogClosed, result);
				
				return result_future;
			}
		}

		std::future<Optional<FilePath>> OpenFile(const Array<FileFilter>& filters, const FilePath& defaultPath, const String&)
		{
			return detail::s3dOpenDialog<Optional<FilePath>>(filters);
		}



		std::future<Image> OpenImage(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Image>({ FileFilter::AllImageFiles() });
		}

		std::future<Texture> OpenTexture(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Texture>({ FileFilter::AllImageFiles() });
		}

		std::future<Texture> OpenTexture(const TextureDesc desc, const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Texture>({ FileFilter::AllImageFiles() });
		}

		std::future<Wave> OpenWave(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenDialog<Wave>({ FileFilter::AllAudioFiles() });
		}

		std::future<Audio> OpenAudio(const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenAudio({ FileFilter::AllAudioFiles() });
		}

		std::future<Audio> OpenAudio(const Arg::loop_<bool> loop, const FilePath& defaultPath, const String& title)
		{
			return detail::s3dOpenAudio({ FileFilter::AllAudioFiles() });
		}
	}
}
