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
    CImage::CImage(const char* pData, texint_t iDataLn, EImageChannels iForceChannels) {
        m_pData = (char*)stbi_load_from_memory((const unsigned char*)pData, iDataLn, 
                                                (int*)&m_iWidth, (int*)&m_iHeight, 
                                                (int*)&m_iChannels, (int)iForceChannels);

        if(iForceChannels != EImageChannels::NONE) {
            m_iChannels = iForceChannels;
        }
    }

    CImage::CImage(texint_t iW, texint_t iH, EImageChannels iChannels) : m_iWidth(iW), m_iHeight(iH), m_iChannels(iChannels) {
        const texint_t iLength = iW * iH * (texint_t)iChannels;

        m_pData = Malloc<char>(iLength);
        this->Fill(Color(0,0,0,255));
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
        const texint_t iLength = Other.Length();

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

    EImageChannels CImage::Channels() const { return m_iChannels; }
    texint_t CImage::Width() const { return m_iWidth; }
    texint_t CImage::Height() const { return m_iHeight; }

    void CImage::Fill(Color iColor) {
        const texint_t iLength = this->Length();
        for(texint_t i = 0; i < iLength; i += (texint_t)m_iChannels) {
            memcpy(&m_pData[i], &iColor, (texint_t)m_iChannels);
        }
    }

    Color CImage::GetPixel(texint_t iX, texint_t iY) const {
        texint_t iLinear = PlanarToLinear(m_iWidth, iX, iY) * (texint_t)m_iChannels;

        Color Out;
        memcpy(&Out, &m_pData[iLinear], (texint_t)m_iChannels);

        return Out;
    }

    char* CImage::GetPixelP(texint_t iX, texint_t iY) {
        texint_t iLinear = PlanarToLinear(m_iWidth, iX, iY) * (texint_t)m_iChannels;
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

    // BROKEN
    void CImage::Blit(const CImage& Other, texint_t iX, texint_t iY) {
        const char* pSource = Other.Data();

        if(iX > m_iWidth || iY > m_iHeight) {
            return;
        }

        texint_t iCopyWidth = Other.Width();      // in pixels
        if(iX + Other.Width() > m_iWidth) {
            iCopyWidth = iX + Other.Width() - m_iWidth;
        }

        texint_t iCopyHeight = Other.Height();    // in pixels
        if(iY + Other.Height() > m_iHeight) {
            iCopyHeight = iY + Other.Height() - m_iHeight;
        }

        for(texint_t iRowNumber = iY; iRowNumber < iY + iCopyHeight; iRowNumber++) {
            char* pDestRow = m_pData + iRowNumber*iCopyWidth*(texint_t)m_iChannels;
            const char* pSourceRow = Other.Data() + iRowNumber*Other.Width();

            if(m_iChannels == Other.Channels()) {
                memcpy(pDestRow + iX, pSourceRow, iCopyWidth*(texint_t)m_iChannels);
            } else {
                for(texint_t i = iX; i < iX + iCopyWidth; i) {
                    memcpy(pDestRow + i*(texint_t)m_iChannels, pSourceRow + i*(texint_t)Other.Channels(), (texint_t)m_iChannels);
                }
            }
        }
    }
}
