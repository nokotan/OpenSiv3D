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
# include <Siv3D/EngineLog.hpp>

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
            std::promise<Audio> audioPromise;

            CallbackData(std::promise<Audio>&& p) : 
                audioPromise(std::move(p))
            {}
        };

        EM_IMPORT(s3dDecodeAudioFromFile)
        void s3dDecodeAudioFromFile(const char* filePath, Callback callback, CallbackData* arg);

        void DecodeAudioFromFileCallback(CallbackData* data)
        {
            if (data->dataLength > 0)
            {
                Wave wave { data->dataLength, Arg::samplingRate(data->samplingRate) };

                for (uint32 i = 0; i < data->dataLength; i++)
                {
                    wave[i].set(data->leftChannelData[i], data->rightChannelData[i]);
                }

                data->audioPromise.set_value(Audio(std::move(wave)));

                if (data->leftChannelData != data->rightChannelData) 
                {
                    ::free(data->rightChannelData);
                }

                ::free(data->leftChannelData);
            }
            else
            {
                data->audioPromise.set_value(Audio());
            }
                  
            delete data;
        }
    }

    void DecodeAudioFromFile(const FilePath& path, std::promise<Audio> promise)
    {
        if (Audio processedByEmbeddedCodec(path); !processedByEmbeddedCodec.isEmpty())
        {
            // Immediately resolve
            promise.set_value(processedByEmbeddedCodec);
        }
        else
        {
            auto data = new detail::CallbackData(std::move(promise));
            s3dDecodeAudioFromFile(path.toUTF8().c_str(), detail::DecodeAudioFromFileCallback, data);

            LOG_TRACE(U"DecodeAudioFromFile: falling back to Browser-Supported Decoding");
        }
    }

    std::future<Audio> DecodeAudioFromFile(const FilePath& path)
    {
        std::promise<Audio> promise;
        auto future = promise.get_future();

        DecodeAudioFromFile(path, std::move(promise));
        return future;
    }
}