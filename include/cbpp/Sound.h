#ifndef CBPP_SOUND_API_H
#define CBPP_SOUND_API_H

#include <stdint.h>
#include <stddef.h>

// Sounds with size bigger than this (5 MB by default) will be streamed
#define CBPP_SOUND_STREAM_THRESHOLD (1024*1024*5)

#define CBPP_SOUND_STREAM_BUFF_SIZE (4096)
#define CBPP_SOUND_SAMPLE_SIZE (4096)

#define CBPP_SOUND_CHANNELS 64

namespace cbpp {
    enum class ESoundLoadingPolicy : uint32_t {
        Automatic,                                  // Let the engine decide how to handle this sound
        ForcePreload,                               // Force preloading the entire file into the memory
        ForceStreaming                              // Force streaming the file from the disc
    };

    enum class ESoundClass : uint32_t {
        Preloaded,
        Streamed
    };

    template <typename format_t, int t_iLength> struct SoundFrame { format_t Data[t_iLength]; };

    typedef SoundFrame<float, 4> SoundFrameF4;

    class ISound {
        uint32_t m_iSampleRate;
        uint32_t m_iChannels;
        uint32_t m_iBitsPerSample;

        size_t m_iPointer = 0;
        size_t m_iFullLength = 0;

        public:
            virtual ESoundClass GetType() const = 0;

            // Amount of bytes in this sound
            virtual size_t Size() const = 0;

            // Length of this sound in seconds
            virtual size_t Length() const = 0;

            // Retreive next N bytes from the sound. Returns the amount of bytes retreived.
            virtual size_t RetreiveBytes(float* pTarget, size_t iTargetLength) = 0;
    };

    ISound* LoadSound(const char* sPath, ESoundLoadingPolicy iLoadPolicy = ESoundLoadingPolicy::Automatic);
    void UnloadSound(ISound* pSound);

    void PlaySound(ISound* pSound, int iChannel = -1);

    class CAudioEngine {

    };
}

#endif
