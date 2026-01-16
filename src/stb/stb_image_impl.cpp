#include "cbpp/Memory.h"

#include "cbpp/Image.h"

//      ++ stb_image ++
#define STBI_MALLOC(sz)             cbpp::Malloc<char>(sz)
#define STBI_REALLOC(p,newsz)       cbpp::Realloc<char>((char*)(p), newsz)
#define STBI_FREE(p)                cbpp::Free(p);
#define STBI_ASSERT(x)              CbAssert(!(x), "stb_image error")

#define STBI_MAX_DIMENSIONS CBPP_MAX_IMAGE_SIZE
#define STBI_NO_STDIO
#define STBI_NO_HDR

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

//      ++ stb_image_write ++
#define STBI_WRITE_NO_STDIO

#define STBIW_MALLOC(sz)            cbpp::Malloc<char>(sz)
#define STBIW_REALLOC(p,newsz)      cbpp::Realloc<char>((char*)(p),newsz)
#define STBIW_FREE(p)               cbpp::Free(p)
#define STBIW_ASSERT(x)             CbAssert(!(x), "stb_image_write error")

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

//      ++ stb_image_resize ++
#define STBIR_MALLOC(size,c)        ((void)(c), cbpp::Malloc<char>(size))
#define STBIR_FREE(ptr,c)           ((void)(c), cbpp::Free(ptr))
#define STBIR_ASSERT(x)             CbAssert(!(x), "stb_image_resize error")

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"
