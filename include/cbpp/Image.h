#ifndef CBPP_API_IMAGE_H
#define CBPP_API_IMAGE_H

#include <initializer_list>
#include <stdint.h>
#include <stddef.h>

#define CBPP_MAX_IMAGE_SIZE 4096

namespace cbpp {
    typedef uint32_t texint_t;

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
        NONE    = 0,
        L       = 1,    // Greyscale
        LA      = 2,    // Greyscale with alpha
        RGB     = 3,    // Red Green Blue
        RGBA    = 4     // RGB with alpha
    };

    enum class EImageType : uint32_t {
        RAW,
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
        texint_t m_iWidth = 0, m_iHeight = 0;
        EImageChannels m_iChannels = EImageChannels::RGB;

        public:
            CImage() = default;
            CImage(const char* sData, texint_t iDataLength, EImageChannels iForceChannels = EImageChannels::NONE);

            CImage(texint_t iW, texint_t iH, EImageChannels iChannels = EImageChannels::RGBA);
            CImage(const char* pData, texint_t iW, texint_t iH, EImageChannels iChannels = EImageChannels::RGBA);

            CImage(const CImage& Other);
            CImage(CImage&& Other);

            CImage& operator=(const CImage& Other);
            CImage& operator=(CImage&& Other);

            bool IsValid() const;
            bool IsPOT() const;

            EImageChannels Channels() const;
            texint_t Width() const;
            texint_t Height() const;
            texint_t Length() const;

            Color GetPixel(texint_t iX, texint_t iY) const;
            char* GetPixelP(texint_t iX, texint_t iY);

            const char* Data() const;
            char* Data();

            void Fill(Color iColor);

            // Scale image to POT while preserving it`s aspect ratio
            void PadToPOT();

            void Resize(texint_t iNewX, texint_t iNewY);

            // Render another image onto this image at these coordinates
            void Blit(const CImage& Other, texint_t iX, texint_t iY);

            /*
                Supports BMP, PNG, TGA and JPEG
                iArg can be used to specify:
                    - JPEG: image quality, 0-100
                    - PNG:  compression level, 0-9
                    - TGA:  use RLE or not, 1 or 0
            */
            bool SaveAs(const char* sPath, EImageType iType, int iArg = 0);

            ~CImage();
    };
}

#endif
