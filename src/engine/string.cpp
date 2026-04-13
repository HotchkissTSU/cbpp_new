#include "cbpp/String.h"
#include "cbpp/Memory.h"

namespace cbpp {
    void CString::Nullify() {        
        m_sData = Realloc<char>(m_sData, 1);
        m_sData[0] = '\0';
        
        m_iLength = 0;
    }

    CString::CString() {
        Nullify();
    }

    CString::CString(size_t iLength) {
        m_iLength = iLength;
        m_sData = Malloc<char>(iLength+1);
        m_sData[iLength] = '\0';
    }

    CString::CString(const char* sSource) {
        if (sSource == NULL) {
            Nullify();
            return;
        }
        
        m_iLength = strlen(sSource);
        m_sData = Malloc<char>(m_iLength + 1);

        memcpy(m_sData, sSource, m_iLength);
        m_sData[m_iLength] = '\0';
    }

    CString::CString(const CString& Other) {
        if (!Other.IsValid()) {
            Nullify();
            return;
        }
        
        m_iLength = Other.m_iLength;
        m_sData = Malloc<char>(m_iLength + 1);
        memcpy(m_sData, Other.m_sData, m_iLength + 1);
    }

    CString::CString(CString&& Other) 
        : m_sData(Other.m_sData), m_iLength(Other.m_iLength) {
        Other.m_sData = NULL;
        Other.m_iLength = 0;
    }

    CString::~CString() {
        if (m_sData != NULL) {
            Free(m_sData);
            m_sData = NULL;
        }
    }

    CString& CString::operator=(const char* sOther) {
        if (sOther == NULL) {
            Nullify();
            return *this;
        }

        const size_t iLength = strlen(sOther);
        m_sData = Realloc<char>(m_sData, iLength + 1);
        memcpy(m_sData, sOther, iLength);
        m_sData[iLength] = '\0';

        m_iLength = iLength;
        
        return *this;
    }

    CString& CString::operator=(const CString& Other) {
        if (this == &Other) {
            return *this;
        }
        
        if (!Other.IsValid()) {
            Nullify();
            return *this;
        }
        
        m_sData = Realloc<char>(m_sData, Other.m_iLength+1);
        memcpy(m_sData, Other.m_sData, Other.m_iLength+1);

        m_iLength = Other.m_iLength;
        
        return *this;
    }

    CString& CString::operator=(CString&& Other) {
        if (this == &Other) {
            return *this;
        }
        
        if (m_sData != NULL) {
            Free(m_sData);
        }
        
        m_sData = Other.m_sData;
        m_iLength = Other.m_iLength;
        
        Other.m_sData = NULL;
        Other.m_iLength = 0;
        
        return *this;
    }

    const char* CString::String() const {
        return m_sData != NULL ? m_sData : "(null)";
    }

    char* CString::Pointer() {
        return m_sData;
    }

    size_t CString::Length() const {
        return m_iLength;
    }

    size_t CString::LengthUpdate() {
        if (m_sData != NULL) {
            m_iLength = strlen(m_sData);
        } else {
            m_iLength = 0;
        }
        return m_iLength;
    }

    bool CString::IsValid() const {
        return m_sData != NULL && m_sData[0] != '\0';
    }

    void CString::Set(const char* sData) {
        m_iLength = strlen(sData);

        m_sData = Realloc<char>(m_sData, m_iLength+1);
        m_sData[m_iLength] = '\0';

        memcpy(m_sData, sData, m_iLength);
    }

    void CString::Set(const CString& sData) {
        m_iLength = sData.m_iLength;

        m_sData = Realloc<char>(m_sData, m_iLength+1);
        m_sData[m_iLength] = '\0';

        memcpy(m_sData, sData.String(), m_iLength);
    }

    // Index access

    #define STR_INDEX                                                                                   \
        CbAssertf(iIndex >= m_iLength, "String index %zu is out of range (%zu)", iIndex, m_iLength);    \
        return m_sData[iIndex];

    char& CString::operator[](size_t iIndex) {
        STR_INDEX
    }

    const char& CString::operator[](size_t iIndex) const {
        STR_INDEX
    }

    char& CString::At(size_t iIndex) {
        STR_INDEX
    }

    const char& CString::At(size_t iIndex) const {
        STR_INDEX
    }

    #undef STR_INDEX

    // Comparison operators

    bool CString::operator==(const char* sOther) const {
        if (sOther == NULL && m_sData == NULL) return true;
        if (sOther == NULL || m_sData == NULL) return false;
        return strcmp(m_sData, sOther) == 0;
    }

    bool CString::operator==(const CString& Other) const {
        if (m_sData == NULL && Other.m_sData == NULL) return true;
        if (m_sData == NULL || Other.m_sData == NULL) return false;
        return strcmp(m_sData, Other.m_sData) == 0;
    }

    bool CString::operator<(const char* sOther) const {
        if (sOther == NULL) return false;
        if (m_sData == NULL) return true;
        return strcmp(m_sData, sOther) < 0;
    }

    bool CString::operator<(const CString& Other) const {
        if (Other.m_sData == NULL) return false;
        if (m_sData == NULL) return true;
        return strcmp(m_sData, Other.m_sData) < 0;
    }

    bool CString::operator>(const char* sOther) const {
        if (sOther == NULL) return true;
        if (m_sData == NULL) return false;
        return strcmp(m_sData, sOther) > 0;
    }

    bool CString::operator>(const CString& Other) const {
        if (Other.m_sData == NULL) return true;
        if (m_sData == NULL) return false;
        return strcmp(m_sData, Other.m_sData) > 0;
    }

    // Concatenation

    CString& CString::operator+=(const CString& Other) {
        if (!Other.IsValid()) {
            return *this;
        }
        
        const size_t iNewLength = m_iLength + Other.m_iLength + 1,
                     iOldLength = m_iLength;

        m_sData = Realloc<char>(m_sData, iNewLength);
        memcpy(m_sData + iOldLength, Other.m_sData, Other.m_iLength);

        m_iLength = iNewLength;
        
        return *this;
    }

    CString& CString::operator+=(char iOther) {
        m_sData = Realloc<char>(m_sData, m_iLength + 1);
        m_sData[m_iLength] = iOther;
        m_sData[m_iLength+1] = '\0';

        m_iLength++;
        
        return *this;
    }

    // Formatted printing

    size_t CString::Printv(const char* sFormat, va_list Args) {
        return vsnprintf(m_sData, m_iLength, sFormat, Args);
    }

    size_t CString::Printf(const char* sFormat, ...) {
        va_list Args;
        va_start(Args, sFormat);
        size_t iLn = vsnprintf(m_sData, m_iLength, sFormat, Args);
        va_end(Args);

        return iLn;
    }
    
    // Type casts

    CString::operator const char*() const {
        return const_cast<const char*>(m_sData);
    }

    CString::operator char*() {
        return m_sData;
    }
    
    CString::operator bool() const {
        return IsValid();
    }

    // External operators

    CString operator+(const CString& A, const CString& B) {
        if (!A.IsValid() && !B.IsValid()) {
            return CString();
        }
        
        if (!A.IsValid()) return B;
        if (!B.IsValid()) return A;
        
        CString Out( A.Length() + B.Length() );

        memcpy(Out.m_sData, A.String(), A.Length());
        memcpy(Out.m_sData + A.Length(), B.String(), B.Length());
        
        return Out;
    }

    CString operator+(const CString& A, const char* B) {
        if (B == NULL) {
            return A;
        }
        
        const size_t iBLength = strlen(B);
        if (iBLength == 0) {
            return A;
        }
        
        if (!A.IsValid()) {
            return CString(B);
        }
        
        CString Out( A.Length() + iBLength );

        memcpy(Out.m_sData, A.String(), A.Length());
        memcpy(Out.m_sData + A.Length(), B, iBLength);
        
        return Out;
    }

    EBinaryClass CString::GetBinaryClass() const {
        return EBinaryClass::String;
    }

    size_t CString::AsBinary(uint8_t* pBuffer) const {        
        if(pBuffer != NULL) {
            uint64_t iFixedLength = m_iLength;
            memcpy(pBuffer, &iFixedLength, sizeof(iFixedLength));

            memcpy(pBuffer + sizeof(uint64_t), m_sData, m_iLength);
        }

        return sizeof(uint64_t) + m_iLength;
    }

    bool CString::FromBinary(const uint8_t* pData, size_t iLength) {
        if(iLength < sizeof(uint64_t)) {
            return false;
        }

        size_t iLength2 = *(uint64_t*)(pData);

        if(iLength2 + sizeof(uint64_t) >= iLength) {
            return false;
        }

        if(iLength2 == 0) {
            this->Nullify();
        } else {
            m_iLength = iLength2;
            m_sData = Realloc(m_sData, m_iLength + 1);
            m_sData[m_iLength] = '\0';
            memcpy(m_sData, pData + sizeof(uint64_t), iLength2);
        }

        return true;
    }
}
