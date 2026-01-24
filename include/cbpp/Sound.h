#ifndef CBPP_SOUND_API_H
#define CBPP_SOUND_API_H

#include <stdint.h>
#include <stddef.h>

// Sounds with size bigger than this (5 MB by default) will be streamed
#define CBPP_SOUND_STREAM_THRESHOLD (1024*1024*5)

#define CBPP_SOUND_STREAM_BUFF_SIZE (4096)

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

    class ISound {
        uint32_t m_iSampleRate;
        uint32_t m_iChannels;
        uint32_t m_iBitsPerSample;

        public:
            virtual ESoundClass GetType() const = 0;
    };

    ISound* LoadSound(const char* sPath, ESoundLoadingPolicy iLoadPolicy = ESoundLoadingPolicy::Automatic);
    void UnloadSound(ISound* pSound);
}

#endif
