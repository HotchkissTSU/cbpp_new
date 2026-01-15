#ifndef CBPP_API_IMAGE_H
#define CBPP_API_IMAGE_H

#include <stdint.h>
#include <stddef.h>

#define CBPP_MAX_IMAGE_SIZE 4096

namespace cbpp {
    struct Color {
        union {
            uint32_t    Whole = 0;
            char        RGBA[4];
        };
        
        Color() = default;
        
        Color(char RGBA);
        Color(char RGB, char A);
        Color(char R, char G, char B);
        Color(char R, char G, char B, char A);
    };

    enum class EImageChannels : uint32_t {
        L       = 1,    // Greyscale
        LA      = 2,    // Greyscale with alpha
        RGB     = 3,    // Reg Green Blue
        RGBA    = 4     // RGB with alpha
    };

    enum class EImageType : uint32_t {
        JPEG,
        PNG,
        TGA,
        BMP,
        PSD,
        GIF,
        HDR,
        PIC,
        PNM
    };

    /*
        Represents a raw image data.
        All operations are CPU-side
    */
    class CImage {
        char* m_pData = NULL;
        size_t m_iWidth = 0, m_iHeight = 0;
        EImageChannels m_iChannels = EImageChannels::RGB;

        public:
            CImage() = default;
            CImage(const char* sData, size_t iDataLength);

            CImage(size_t iW, size_t iH, EImageChannels iChannels = EImageChannels::RGBA);
            CImage(const char* pData, size_t iW, size_t iH, EImageChannels iChannels = EImageChannels::RGBA);

            CImage(const CImage& Other);
            CImage(CImage&& Other);

            CImage& operator=(const CImage& Other);
            CImage& operator=(CImage&& Other);

            bool IsValid() const;

            EImageChannels Channels() const;
            size_t Width() const;
            size_t Height() const;
            size_t Length() const;

            Color GetPixel(size_t iX, size_t iY) const;
            char* GetPixelP(size_t iX, size_t iY);

            const char* Data() const;
            char* Data();
        
            void Fill(Color iColor);

            // Render another image onto this image at these coordinates
            void Blit(const CImage& Other, size_t iX, size_t iY);

            bool SaveAs(const char* sPath, EImageType iType, int iArg = 0);

            ~CImage();
    };
}

#endif
