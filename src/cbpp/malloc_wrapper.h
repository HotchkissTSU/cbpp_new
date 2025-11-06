#ifndef CBPP_ALLOCW_H
#define CBPP_ALLOCW_H

/*
	Memory management call wrappers. 
    This is possible to replace the allocator if keeping the "malloc"-ish interface
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

        CbAssert(pMemory == NULL, "Allocation failure");

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

        CbAssert(pTemp == NULL, "Reallocation failure");

		return pTemp;
    }

	void Free(void* pMemory);

    template <typename T, typename... args_t> void ConstructBufferPrimitive(T* pBuffer, T& Value, args_t&&... Args) {
        memcpy(pBuffer, &Value, sizeof(T));
    }

    /*
        Attempt to call a specified constructor for this buffer.
        Primitive types are copied from the first variadic argument.
    */
    template <typename T, typename... args_t> void ConstructBuffer(T* pBuffer, args_t&&... Args) {
        if constexpr (std::is_fundamental_v<T>) {
            ConstructBufferPrimitive(pBuffer, std::forward<args_t>(Args)...);
        } else {
            static_assert(std::is_constructible_v<T, args_t...>, "No suitable constructor");
            new(pBuffer) T(std::forward<args_t>(Args)...);
        }
    }

    /*
        Attempt to call a copy constructor for this buffer.
        Primitive types are simply copied.
    */
    template <typename T> void ConstructBuffer(T* pBuffer, T& Value) {
        if constexpr (std::is_fundamental_v<T>) {
            memcpy(pBuffer, &Value, sizeof(T));
        } else {
            static_assert(std::is_copy_constructible_v<T>, "No copy constructor");
            new(pBuffer) T(Value);
        }
    }

    /*
        Attempt to call a default constructor for this buffer.
        Primitive types are zeroed.
    */
    template <typename T> void ConstructBuffer(T* pBuffer) {
        if constexpr (std::is_fundamental_v<T>) {
            memset(pBuffer, 0, sizeof(T));
        } else {
            static_assert(std::is_default_constructible_v<T>, "No default constructor");
            new(pBuffer) T();
        }
    }

    // Allocate a new object
    template <typename T, typename... args_t> T* New(args_t&&... Args) {
        T* pBuffer = Malloc<T>();
        new(pBuffer) T(std::forward<args_t>(Args)...);
        return pBuffer;
    }

    // Allocate a new array of objects
    template <typename T, typename... args_t> T* NewA(size_t iAmount, args_t... Args) {
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
