#include "cbpp/Sound.h"

#include "cbpp/Filesystem.h"

namespace cbpp {
    class CLoadedSound final : public ISound {
        friend ISound* LoadSoundPreloaded(IFile*, size_t);

        uint32_t* m_pData = NULL;
        size_t m_iSize = 0;

        public:
            ESoundClass GetType() const;
    };

    class CStreamedSound final : public ISound {
        friend ISound* LoadSoundStreamed(IFile*);

        IFile* m_pStream = NULL;

        public:
            ESoundClass GetType() const;
    };
}
