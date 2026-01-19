#ifndef CBPP_API_IMAGE_H
#define CBPP_API_IMAGE_H

/*
    CBPP will accept any input image resolution, but if it`s not POT (Power-Of-Two),
    a scaling and padding may be needed. Try to use only POT images to save on
    runtime computation, as image operations are kinda expensive.

    An image is considered POT only if both of it`s sides are POT (e.g 128x512, 1024x256 etc).
*/

#include <stdint.h>
#include <stddef.h>

#include "cbpp/Vector.h"

#define CBPP_MAX_IMAGE_SIZE 8192 // 8192 is supported by the most graphics cards at this point

namespace cbpp {
    typedef uint32_t texint_t;

    struct Color {
        union {
            uint32_t        Packed = 0;
            union {
                char        RGBA[4];
                char R; char G; char B; char A;
            };
        };
        
        Color() = default;

        Color(char RGBA);
        Color(char RGB, char A);
        Color(char R, char G, char B);
        Color(char R, char G, char B, char A);
    };

    enum class EImageFilter : uint32_t {
        Default     = 0,        // Automatic filter choice
        Box         = 1,        // A trapezoid w/1-pixel wide ramps, same result as box for integer scale ratios
        Triangle    = 2,        // On upsampling, produces same results as bilinear texture filtering
        CubicSpline = 3,        // The cubic b-spline (aka Mitchell-Netrevalli with B=1,C=0), gaussian-esque
        Catmull     = 4,        // An interpolating cubic spline
        Mitchell    = 5         // Mitchell-Netrevalli filter with B=1/3, C=1/3
    };

    enum class EImageChannels : uint32_t {
        NONE        = 0,
        L           = 1,        // Greyscale
        LA          = 2,        // Greyscale with alpha
        RGB         = 3,        // Red Green Blue
        RGBA        = 4         // RGB with alpha
    };

    enum class EImageType : uint32_t {
        RAW,
        JPEG,
        PNG,
        TGA,
        BMP,
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

            // Guess the source image type and load it as raw pixel data with the selected channel count
            CImage(const char* sData, texint_t iDataLength, EImageChannels iForceChannels = EImageChannels::NONE);

            CImage(texint_t iW, texint_t iH, EImageChannels iChannels = EImageChannels::RGBA);
            CImage(const char* pData, texint_t iW, texint_t iH, EImageChannels iChannels = EImageChannels::RGBA);

            CImage(const CImage& Other);
            CImage(CImage&& Other);

            CImage& operator=(const CImage& Other);
            CImage& operator=(CImage&& Other);

            bool IsValid() const;
            bool IsPOT() const;

            EImageChannels  Channels() const;
            texint_t        Width() const;
            texint_t        Height() const;

            // The amount of bytes this image takes
            texint_t        Length() const;

            Color GetPixel(texint_t iX, texint_t iY) const;

            /*
                Note that if for example an image has only 2 channels it`s safe to access
                first two bytes in the Color->RGBA etc.

                Modifying other bytes may lead to the next pixels` corruption or to the juicy loud segfault.

                Consider this pointer invalidated after any image manipulations.
            */
            Color* GetPixelP(texint_t iX, texint_t iY);

            const char* Data() const;
            char* Data();

            const char* Row(texint_t iNumber) const;
            char* Row(texint_t iNumber);

            void Fill(Color iColor);

            /* 
                Pad image to POT.
                The actual image spans between (0,0) (top-left corner) and the returned vector (bottom-right one).
            */
            Vec2i PadToPOT();

            /* 
                Scale and pad image to POT while preserving it`s aspect ratio.
                The actual image spans between (0,0) (top-left corner) and the returned vector (bottom-right one).
            */
            Vec2i ModToPOT(EImageFilter iFilter = EImageFilter::Default);

            void Resize(texint_t iNewX, texint_t iNewY, EImageFilter iFilter = EImageFilter::Default);
            void Scale(float fX, float fY, EImageFilter iFilter = EImageFilter::Default);

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
