#include "cbpp/Sound.h"

#include "engine/audio/sound_classes.h"

#include "drflac/dr_flac.h"
#include "engine/audio/alloc_wrap.h"

#include "cbpp/Memory.h"

namespace cbpp {
    ISound* LoadSoundStreamed(IFile* pFile) {

    }

    ISound* LoadSoundPreloaded(IFile* pFile, size_t iFileLength) {
        CLoadedSound* pSound = New<CLoadedSound>();

        CloseFile(pFile);
        return (ISound*)pSound;
    }

    ISound* LoadSound(const char* sPath, ESoundLoadingPolicy iLoadPolicy) {
        IFile* pFile = OpenFile(sPath, "rb");

        if(pFile == NULL) {
            return NULL;
        }

        size_t iFileLength = pFile->Length();

        if(iLoadPolicy != ESoundLoadingPolicy::Automatic) {
            if(iLoadPolicy == ESoundLoadingPolicy::ForcePreload) {
                return LoadSoundPreloaded(pFile, iFileLength);
            }
            return LoadSoundStreamed(pFile);

        } else {
            if(iFileLength > CBPP_SOUND_STREAM_THRESHOLD) {
                return LoadSoundStreamed(pFile);
            }
            return LoadSoundPreloaded(pFile, iFileLength);
        }
    }
}
