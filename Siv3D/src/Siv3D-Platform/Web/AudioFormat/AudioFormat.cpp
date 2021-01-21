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

# include <Siv3D/Wave.hpp>
# include <Siv3D/Audio.hpp>
# include <Siv3D/Platform.hpp>
# include <Siv3D/PlatformDetail.hpp>

# include <future>

namespace s3d::Platform::Web::AudioProcessing
{
    namespace detail
    {
        using Callback = void(*)(struct CallbackData*);

        struct CallbackData
        {
            float* leftChannelData;
            float* rightChannelData;
            uint32 samplingRate;
            std::size_t dataLength;
            std::promise<Audio> wavePromise;
        };

        EM_IMPORT(s3dDecodeAudioFromFile)
        void s3dDecodeAudioFromFile(const char* filePath, Callback callback, CallbackData* arg);

        void DecodeAudioFromFileCallback(CallbackData* data)
        {
            Wave wave { data->dataLength, Arg::samplingRate(data->samplingRate) };

            for (uint32 i = 0; i < data->dataLength; i++)
            {
                wave[i].set(data->leftChannelData[i], data->rightChannelData[i]);
            }

            data->wavePromise.set_value(Audio(std::move(wave)));

            if (data->leftChannelData != data->rightChannelData) 
            {
                ::free(data->rightChannelData);
            }

            ::free(data->leftChannelData);
            
            delete data;
        }
    }

    std::future<Audio> DecodeAudioFromFile(const FilePath& path)
    {
        auto data = new detail::CallbackData();
        auto future = data->wavePromise.get_future();

        s3dDecodeAudioFromFile(path.toUTF8().c_str(), detail::DecodeAudioFromFileCallback, data);

        return future;
    }
}