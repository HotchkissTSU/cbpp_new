#include "cbpp/mutable_string.h"

#include "cbpp/malloc_wrapper.h"

#define STR_INDEX return m_sData[iIndex];

namespace cbpp {
    CString::CString() : m_iLength(0) {
        m_sData = NULL;
    }

    CString::CString(const char* sSource) {
        m_sData = StringDup(sSource, &m_iLength);
    }

    CString::CString(const CString& Other) {
        m_iLength = Other.Length();
        m_sData = Malloc<char>(m_iLength+1);
        memcpy(m_sData, Other, m_iLength+1);
    }

    char& CString::operator[](size_t iIndex) { STR_INDEX }
    char& CString::At(size_t iIndex) { STR_INDEX }
    char CString::operator[](size_t iIndex) const { STR_INDEX }
    char CString::At(size_t iIndex) const { STR_INDEX }

    size_t CString::Length() const { return m_iLength; }

    size_t CString::LengthUpdate() {
        m_iLength = strlen(m_sData);
        return m_iLength;
    }

    CString& CString::operator=(const CString& Other) {
        m_iLength = Other.Length();

        m_sData = Realloc<char>(m_sData, m_iLength+1);
        memcpy(m_sData, Other.String(), m_iLength+1);

        return *this;
    }

    CString& CString::operator+=(const CString& Other) {
        const size_t iLength = this->Length();
        const size_t iOtherLength = Other.Length();

        m_iLength = iLength + iOtherLength + 1;

        m_sData = Realloc<char>(m_sData, m_iLength);
        memcpy(&m_sData[iLength], Other, iOtherLength+1);

        return *this;
    }

    CString& CString::operator+=(char iOther) {
        m_iLength++;
        m_sData = Realloc<char>(m_sData, m_iLength);
        m_sData[m_iLength-1] = iOther;
        m_sData[m_iLength] = '\0';

        return *this;
    }

    CString operator+(const CString& A, const CString& B) {
        const size_t iLength = A.Length() + B.Length() + 1;
        char* sSum = Malloc<char>(iLength);
        memcpy(sSum, A, A.Length());
        memcpy(&sSum[A.Length()], B, B.Length()+1);

        CString Out;
        Out.m_sData = sSum;
        Out.m_iLength = iLength;
        return Out;
    }

    CString operator+(const CString& A, const char* B) {
        const size_t iBLen = strlen(B);
        const size_t iLength = A.Length() + iBLen + 1;
        
        char* sSum = Malloc<char>(iLength);
        memcpy(sSum, A, A.Length());
        memcpy(&sSum[A.Length()], B, iBLen+1);

        CString Out;
        Out.m_sData = sSum;
        Out.m_iLength = iLength;
        return Out;
    }
    
    char* CString::Pointer() {
        return m_sData;
    }

    CString::operator char *() {
        return m_sData;
    }
}
