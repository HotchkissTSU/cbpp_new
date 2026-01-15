#include "cbpp/Image.h"

#include "cbpp/Memory.h"
#include "cbpp/Math.h"
#include "cbpp/Filesystem.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

// Color

namespace cbpp {
    Color::Color(char _RGBA) {
        RGBA[0] = _RGBA;
        RGBA[1] = _RGBA;
        RGBA[2] = _RGBA;
        RGBA[3] = _RGBA;
    }

    Color::Color(char RGB, char A) {
        RGBA[0] = RGB;
        RGBA[1] = RGB;
        RGBA[2] = RGB;
        RGBA[3] = A;
    }

    Color::Color(char R, char G, char B) {
        RGBA[0] = R;
        RGBA[1] = G;
        RGBA[2] = B;
        RGBA[3] = 255;
    }

    Color::Color(char R, char G, char B, char A) {
        RGBA[0] = R;
        RGBA[1] = G;
        RGBA[2] = B;
        RGBA[3] = A;
    }
}

// CImage

namespace cbpp {
    CImage::CImage(size_t iW, size_t iH, EImageChannels iChannels) : m_iWidth(iW), m_iHeight(iH), m_iChannels(iChannels) {
        const size_t iLength = iW * iH * (size_t)iChannels;

        m_pData = Malloc<char>(iLength);
        this->Fill(Color(0,0,0,255));
    }

    CImage::CImage(const char* pData, size_t iW, size_t iH, EImageChannels iChannels) : m_iWidth(iW), m_iHeight(iH), m_iChannels(iChannels) {
        const size_t iLength = iW * iH * (size_t)iChannels;

        m_pData = Malloc<char>(iLength);
        memcpy(m_pData, pData, iLength);
    }

    CImage::CImage(const CImage& Other) : m_iWidth(Other.Width()), m_iHeight(Other.Height()), m_iChannels(Other.Channels()) {
        const size_t iLength = m_iWidth * m_iHeight * (size_t)m_iChannels;

        m_pData = Malloc<char>(iLength);
        memcpy(m_pData, Other.Data(), iLength);
    }

    CImage::CImage(CImage&& Other) {
        memcpy(this, &Other, sizeof(CImage));
        memset(&Other, 0, sizeof(CImage));
    }

    CImage& CImage::operator=(const CImage& Other) {
        const size_t iLength = Other.Length();

        m_pData = Realloc<char>(m_pData, iLength);
        memcpy(m_pData, Other.Data(), iLength);

        m_iChannels = Other.Channels();
        m_iWidth = Other.Width();
        m_iHeight = Other.Height();

        return *this;
    }

    CImage& CImage::operator=(CImage&& Other) {
        memcpy(this, &Other, sizeof(CImage));
        memset(&Other, 0, sizeof(CImage));

        return *this;
    }

    bool CImage::IsValid() const {
        if(m_pData == NULL) { return false; }

        return true;
    }

    CImage::~CImage() {
        if(m_pData != NULL) {
            Free(m_pData);
        }
    }

    size_t CImage::Length() const {
        return m_iWidth * m_iHeight * (size_t)m_iChannels;
    }

    EImageChannels CImage::Channels() const { return m_iChannels; }
    size_t CImage::Width() const { return m_iWidth; }
    size_t CImage::Height() const { return m_iHeight; }

    void CImage::Fill(Color iColor) {
        const size_t iLength = this->Length();
        for(size_t i = 0; i < iLength; i += (size_t)m_iChannels) {
            memcpy(&m_pData[i], &iColor, (size_t)m_iChannels);
        }
    }

    Color CImage::GetPixel(size_t iX, size_t iY) const {
        size_t iLinear = PlanarToLinear(m_iWidth, iX, iY) * (size_t)m_iChannels;

        Color Out;
        memcpy(&Out, &m_pData[iLinear], (size_t)m_iChannels);

        return Out;
    }

    char* CImage::GetPixelP(size_t iX, size_t iY) {
        size_t iLinear = PlanarToLinear(m_iWidth, iX, iY) * (size_t)m_iChannels;
        return &m_pData[iLinear];
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
                //stbi_write_bmp(sPath, m_iWidth, m_iHeight, (int)m_iChannels, m_pData);
                break;

            case EImageType::JPEG:
                stbi_write_jpg_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData, iArg);
                break;

            case EImageType::PNG:
                stbi_write_png_compression_level = iArg;
                stbi_write_png_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData, 0);
                break;

            case EImageType::TGA:
                stbi_write_tga_with_rle = 1;
                stbi_write_tga_to_func(ImgWriteCallback, hFile, m_iWidth, m_iHeight, (int)m_iChannels, m_pData);
                break;

            default:
                CloseFile(hFile);
                return false;
        }

        CloseFile(hFile);
        return true;
    }
}
