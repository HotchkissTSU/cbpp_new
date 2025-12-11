#ifndef CBPP_ALLOCW_H
#define CBPP_ALLOCW_H

/*
	Memory management call wrappers. 
    
    Engine allocator is fully replaceable, if the new one keeps
    the exact malloc/realloc/free C interface
*/

#include <string.h>
#include <type_traits>
#include <utility>

#include "cbpp/error.h"

namespace cbpp {
    struct EngineAllocatorData {
        void* (*fpMalloc)(size_t);
        void* (*fpRealloc)(void*, size_t);
        void (*fpFree)(void*);
    };

    constexpr EngineAllocatorData GetAllocatorData() {
        constexpr EngineAllocatorData s_AllocInfo = {
            malloc, 
            realloc, 
            free
        };

        return const_cast<const EngineAllocatorData&> (s_AllocInfo);
    }

    /*
        +1 upon allocating and -1 upon freeing.
        Should be zero at the moment we exit the program
    */
    size_t& GetMallocCounter();

	template <typename T> T* Malloc(size_t iCount) noexcept {
		const size_t iMemorySize = sizeof(T) * iCount;
		T* pMemory = (T*) GetAllocatorData().fpMalloc( iMemorySize );

        CbAssertf(pMemory == NULL, "Allocation of size %zu has failed", iMemorySize);

        GetMallocCounter()++;
		return pMemory;
	}

	template <typename T> T* Calloc(size_t iCount) noexcept {
		T* pMemory = Malloc<T>(iCount);
		memset(pMemory, 0, sizeof(T) * iCount);

        GetMallocCounter()++;
		return pMemory;
	}

	template <typename T> T* Malloc() noexcept {
		return Malloc<T> (1);
    }

	template <typename T> T* Realloc(T* pMemory, size_t iNewSize) noexcept {
		if( pMemory == NULL ) {
			return Malloc<T>(iNewSize);
		}

		const size_t iMemorySize = sizeof(T) * iNewSize;
		T* pTemp = (T*) GetAllocatorData().fpRealloc( pMemory, iMemorySize );

        CbAssertf(pTemp == NULL, "Reallocation of 0x%x to size %zu has failed", pMemory, iMemorySize);

		return pTemp;
    }

	void Free(void* pMemory);

    // Allocate a new object
    template <typename T, typename... args_t> T* New(args_t&&... Args) {
        T* pBuffer = Malloc<T>();
        new(pBuffer) T(std::forward<args_t>(Args)...);
        return pBuffer;
    }

    // Allocate a new array of objects
    template <typename T, typename... args_t> T* NewA(size_t iAmount, args_t&&... Args) {
        T* pBuffer = Malloc<T>(iAmount);
        for(size_t i = 0; i < iAmount; i++) {
            new(&pBuffer[i]) T(std::forward<args_t>(Args)...);
        }
        return pBuffer;
    }
    
    // Properly deallocate an object
    template <typename T> void Delete(T* pObject) {
        if constexpr ( std::is_destructible_v<T> ) {
            pObject->~T();
        }
        Free(pObject);
    }

    // Properly deallocate an array of objects
    template <typename T> void DeleteA(T* pArray, size_t iArraySize) {
        if constexpr ( std::is_destructible_v<T> ) {
            for(size_t i = 0; i < iArraySize; i++) {
                pArray[i].~T();
            }
        }
        Free(pArray);
    }
}

#endif
