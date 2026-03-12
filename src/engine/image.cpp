#include "cbpp/Image.h"

#include "cbpp/Memory.h"
#include "cbpp/Math.h"
#include "cbpp/Filesystem.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "stb/stb_image_resize.h"

// Color

namespace cbpp {
    Color::Color(uint8_t _RGBA) {
        Comp.R = _RGBA;
        Comp.G = _RGBA;
        Comp.B = _RGBA;
        Comp.A = _RGBA;
    }

    Color::Color(uint8_t RGB, uint8_t _A) {
        Comp.R = RGB;
        Comp.G = RGB;
        Comp.B = RGB;
        Comp.A = _A;
    }

    Color::Color(uint8_t _R, uint8_t _G, uint8_t _B) {
        Comp.R = _R;
        Comp.G = _G;
        Comp.B = _B;
        Comp.A = 255;
    }

    Color::Color(uint8_t _R, uint8_t _G, uint8_t _B, uint8_t _A) {
        Comp.R = _R;
        Comp.G = _G;
        Comp.B = _B;
        Comp.A = _A;
    }
}

// CImage

namespace cbpp {
    CImage::CImage(const char* pData, texint_t iDataLn, EImageChannels iForceChannels) {
        int iResult = stbi_info_from_memory((unsigned char*)pData, iDataLn, (int*)&m_iWidth, (int*)&m_iHeight, (int*)&m_iChannels);

        if(iResult == 0) {
            WriteLogf(ELogLevel::Error, "Failed to read image data: %s", stbi_failure_reason());

            m_iWidth = 0;
            m_iHeight = 0;
            m_iChannels = EImageChannels::NONE;

            return;
        }

        if(m_iWidth > CBPP_MAX_IMAGE_SIZE || m_iHeight > CBPP_MAX_IMAGE_SIZE) {
            WriteLogf(ELogLevel::Error, "Image (%ux%u) exceeds the resolution limit (%ux%u). Operation aborted.", 
                        m_iWidth, m_iHeight, CBPP_MAX_IMAGE_SIZE, CBPP_MAX_IMAGE_SIZE);

            m_iWidth = 0;
            m_iHeight = 0;
            m_iChannels = EImageChannels::NONE;

            return;   
        }

        m_pData = (char*)stbi_load_from_memory((const unsigned char*)pData, iDataLn, (int*)&m_iWidth, (int*)&m_iHeight, (int*)&m_iChannels, (int)iForceChannels);

        if(iForceChannels != EImageChannels::NONE) {
            m_iChannels = iForceChannels;
        }
    }

    CImage::CImage(texint_t iW, texint_t iH, EImageChannels iChannels) : m_iWidth(iW), m_iHeight(iH), m_iChannels(iChannels) {
        const texint_t iLength = iW * iH * (texint_t)iChannels;

        m_pData = Malloc<char>(iLength);
    }

    CImage::CImage(const char* pData, texint_t iW, texint_t iH, EImageChannels iChannels) : m_iWidth(iW), m_iHeight(iH), m_iChannels(iChannels) {
        const texint_t iLength = iW * iH * (texint_t)iChannels;

        m_pData = Malloc<char>(iLength);
        memcpy(m_pData, pData, iLength);
    }

    CImage::CImage(const CImage& Other) : m_iWidth(Other.Width()), m_iHeight(Other.Height()), m_iChannels(Other.Channels()) {
        const texint_t iLength = m_iWidth * m_iHeight * (texint_t)m_iChannels;

        m_pData = Malloc<char>(iLength);
        memcpy(m_pData, Other.Data(), iLength);
    }

    CImage::CImage(CImage&& Other) {
        memcpy(this, &Other, sizeof(CImage));
        memset(&Other, 0, sizeof(CImage));
    }

    CImage& CImage::operator=(const CImage& Other) {
        if(Other.Channels() != m_iChannels) { return *this; }

        const texint_t iLength = Other.Length();

        m_pData = Realloc<char>(m_pData, iLength);
        memcpy(m_pData, Other.Data(), iLength);

        m_iChannels = Other.Channels();
        m_iWidth = Other.Width();
        m_iHeight = Other.Height();

        return *this;
    }

    CImage& CImage::operator=(CImage&& Other) {
        if(Other.Channels() == m_iChannels) {
            memcpy(this, &Other, sizeof(CImage));
        }
        
        memset(&Other, 0, sizeof(CImage));

        return *this;
    }

    bool CImage::IsValid() const {
        if(m_pData == NULL) { return false; }

        return true;
    }

    bool CImage::IsPOT() const {
        return cbpp::IsPOT(m_iWidth) && cbpp::IsPOT(m_iHeight);
    }

    CImage::~CImage() {
        if(m_pData != NULL) {
            Free(m_pData);
        }
    }

    texint_t CImage::Length() const {
        return m_iWidth * m_iHeight * (texint_t)m_iChannels;
    }

    EImageChannels  CImage::Channels() const { return m_iChannels; }
    texint_t        CImage::Width() const { return m_iWidth; }
    texint_t        CImage::Height() const { return m_iHeight; }

    void CImage::Fill(Color iColor) {
        const texint_t iLength = this->Length();

        if(m_iChannels == EImageChannels::L) {
            memset(m_pData, iColor.Comp.R, iLength);
            return;
        }

        for(texint_t i = 0; i < iLength; i += (texint_t)m_iChannels) {
            memcpy(&m_pData[i], &iColor, (texint_t)m_iChannels);
        }
    }

    Color CImage::GetPixel(texint_t iX, texint_t iY) const {
        texint_t iLinear = PlanarToLinear(m_iWidth, iX, iY) * (texint_t)m_iChannels;

        Color Out;
        memset(&Out, 0, sizeof(Out));
        memcpy(&Out, &m_pData[iLinear], (texint_t)m_iChannels);

        return Out;
    }

    Color* CImage::GetPixelP(texint_t iX, texint_t iY) {
        texint_t iLinear = PlanarToLinear(m_iWidth, iX, iY) * (texint_t)m_iChannels;
        return (Color*)(&m_pData[iLinear]);
    }

    const char* CImage::Row(texint_t iNumber) const {
        return const_cast<const char*>(m_pData + iNumber * m_iWidth * (texint_t)m_iChannels);
    }

    char* CImage::Row(texint_t iNumber) {
        return m_pData + iNumber * m_iWidth * (texint_t)m_iChannels;
    }
    
    const char* CImage::Data() const { return const_cast<const char*>(m_pData); }
    char* CImage::Data() { return m_pData; }

    void ImgWriteCallback(void *pContext, void *pData, int iSize) {
        IFile* hFile = (IFile*)pContext;
        hFile->Write(iSize, pData);
    }
    
    bool CImage::SaveAs(const char* sPath, EImageType iType, int iArg) {
        IFile* hFile = OpenFile(sPath, "wb");
        if(hFile == NULL) {
            return false;
        }
        
        switch(iType) {
            case EImageType::BMP:
                stbi_write_bmp_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData);
                break;

            case EImageType::JPEG:
                stbi_write_jpg_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData, iArg);
                break;

            case EImageType::PNG:
                stbi_write_png_compression_level = iArg;
                stbi_write_png_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData, 0);
                break;

            case EImageType::TGA:
                stbi_write_tga_with_rle = iArg;
                stbi_write_tga_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData);
                break;

            default:
                CloseFile(hFile);
                return false;
        }

        CloseFile(hFile);
        return true;
    }

    void CImage::Resize(texint_t iW, texint_t iH, EImageFilter iFilter) {
        const size_t iOutLength = iW * iH * (int)m_iChannels;

        if(iW > CBPP_MAX_IMAGE_SIZE || iH > CBPP_MAX_IMAGE_SIZE) {
            WriteLogf(ELogLevel::Error, "Upscaled image (%ux%u) is going to exceed the resolution limit (%ux%u). Operation aborted.", 
                        iW, iH, CBPP_MAX_IMAGE_SIZE, CBPP_MAX_IMAGE_SIZE);

            return;
        }

        char* pOutput = Malloc<char>(iOutLength);

        int iAlpha = STBIR_ALPHA_CHANNEL_NONE;
        if(m_iChannels == EImageChannels::RGBA) {
            iAlpha = 3;
        } else if(m_iChannels == EImageChannels::LA) {
            iAlpha = 1;
        }

        int iResult = stbir_resize_uint8_generic((unsigned char*)m_pData, (int)m_iWidth, (int)m_iHeight,
                                                 0, (unsigned char*)pOutput, iW, iH, 0, (int)m_iChannels,
                                                 iAlpha, 0, STBIR_EDGE_CLAMP, (stbir_filter)iFilter, STBIR_COLORSPACE_LINEAR, NULL);

        m_pData = Realloc<char>(m_pData, iOutLength);
        memcpy(m_pData, pOutput, iOutLength);
        Free(pOutput);

        m_iHeight = iH;
        m_iWidth = iW;
    }

    void CImage::Scale(float fX, float fY, EImageFilter iFilter) {
        texint_t iNewWidth = (texint_t)(fX * (float)m_iWidth);
        texint_t iNewHeight = (texint_t)(fY * (float)m_iHeight);

        this->Resize(iNewWidth, iNewHeight, iFilter);
    }

    Vec2i CImage::PadToPOT() {
        if(cbpp::IsPOT(m_iWidth) && cbpp::IsPOT(m_iHeight)) {
            return Vec2i(m_iWidth, m_iHeight); // already is POT
        }
        
        texint_t iNewWidth = CeilToPowerOf2(m_iWidth), iNewHeight = CeilToPowerOf2(m_iHeight);

        if(iNewHeight > CBPP_MAX_IMAGE_SIZE || iNewWidth > CBPP_MAX_IMAGE_SIZE) {
            WriteLogf(ELogLevel::Error, "Upscaled image (%ux%u) is going to exceed the resolution limit (%ux%u). Operation aborted.", 
                        iNewWidth, iNewHeight, CBPP_MAX_IMAGE_SIZE, CBPP_MAX_IMAGE_SIZE);

            return Vec2i(m_iWidth, m_iHeight);
        }

        char* pNewImage = Malloc<char>(iNewWidth * iNewHeight * (texint_t)m_iChannels);
        
        for(size_t i = 0; i < m_iHeight; i++) { // copy old image to the new buffer
            char* pDestRow = pNewImage + i*iNewWidth*(texint_t)m_iChannels;
            char* pSourceRow = m_pData + i*m_iWidth*(texint_t)m_iChannels;

            memcpy(pDestRow, pSourceRow, m_iWidth * (texint_t)m_iChannels);
        }

        Vec2i Out(m_iWidth, m_iHeight);

        Free(m_pData);
        m_pData = pNewImage;
        m_iHeight = iNewHeight;
        m_iWidth = iNewWidth;

        return Out;
    }

    Vec2i CImage::ModToPOT(EImageFilter iFilter) {
        if(cbpp::IsPOT(m_iWidth) && cbpp::IsPOT(m_iHeight)) {
            return Vec2i(m_iWidth, m_iHeight); // already is POT
        }

        texint_t iNewWidthPOT = CeilToPowerOf2(m_iWidth), iNewHeightPOT = CeilToPowerOf2(m_iHeight);

        if(iNewHeightPOT > CBPP_MAX_IMAGE_SIZE || iNewWidthPOT > CBPP_MAX_IMAGE_SIZE) {
            WriteLogf(ELogLevel::Error, "Upscaled image (%ux%u) is going to exceed the resolution limit (%ux%u). Operation aborted.", 
                        iNewWidthPOT, iNewHeightPOT, CBPP_MAX_IMAGE_SIZE, CBPP_MAX_IMAGE_SIZE);

            return Vec2i(m_iWidth, m_iHeight);
        }
        
        int iAlpha = STBIR_ALPHA_CHANNEL_NONE;
        if(m_iChannels == EImageChannels::RGBA) {
            iAlpha = 3;
        } else if(m_iChannels == EImageChannels::LA) {
            iAlpha = 1;
        }

        texint_t iNewWidth, iNewHeight;

        if(m_iWidth > m_iHeight) {  // horizontal image
            iNewWidth = iNewWidthPOT;
            iNewHeight = (texint_t)((float)m_iHeight * (float)iNewWidthPOT / (float)m_iWidth);
            
        } else {                    // vertical image
            iNewHeight = iNewHeightPOT;
            iNewWidth = (texint_t)((float)m_iWidth * (float)iNewHeightPOT / (float)m_iHeight);
        }
        
        const size_t iNewImgLength = iNewWidthPOT * iNewHeightPOT * (texint_t)m_iChannels;
        const size_t iScaledImage = iNewWidth * iNewHeight * (texint_t)m_iChannels;

        char* pNewImage = Malloc<char>(iNewImgLength + iScaledImage);
        char* pScaledImage = pNewImage + iNewImgLength;
        
        // this dumb function can`t write smaller image to the bigger buffer, so this retarded double-copying is required !! FIXME !!
        int iResult = stbir_resize_uint8_generic((unsigned char*)m_pData, m_iWidth, m_iHeight,
                                                 0, (unsigned char*)pScaledImage, iNewWidth, iNewHeight, 0, (int)m_iChannels,
                                                 iAlpha, 0, STBIR_EDGE_CLAMP, (stbir_filter)iFilter, STBIR_COLORSPACE_LINEAR, NULL);

        for(size_t i = 0; i < iNewHeight; i++) { // copy scaled image to the new buffer
            char* pDestRow = pNewImage + i*iNewWidthPOT*(texint_t)m_iChannels;
            char* pSourceRow = pScaledImage + i*iNewWidth*(texint_t)m_iChannels;

            memcpy(pDestRow, pSourceRow, iNewWidth * (texint_t)m_iChannels);
        }
        
        Free(m_pData);
        
        pNewImage = Realloc<char>(pNewImage, iNewImgLength);

        m_pData = pNewImage;
        m_iWidth = iNewWidthPOT;
        m_iHeight = iNewHeightPOT;

        return Vec2i(iNewWidth, iNewHeight);
    }
}
