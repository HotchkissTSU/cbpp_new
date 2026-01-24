#include "engine/audio/alloc_wrap.h"

#include "cbpp/Memory.h"

namespace cbpp {
    void* AE_Malloc(size_t iSize, void* pContext) {
        return (void*)Malloc<char>(iSize);
    }

    void* AE_Realloc(void* p, size_t iSize, void* pContext) {
        return (void*)Realloc<char>((char*)p, iSize);
    }

    void AE_Free(void* p, void* pContext) {
        Free(p);
    }

    drflac_allocation_callbacks GetAudioAllocWrapper() {
        constexpr drflac_allocation_callbacks Wrap = {
            NULL,
            AE_Malloc,
            AE_Realloc,
            AE_Free
        };

        return Wrap;
    }
}
