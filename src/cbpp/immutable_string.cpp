#include "cbpp/immutable_string.h"

#include <string.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    CImmutableString::CImmutableString() : m_sData(NULL) {}
    CImmutableString::CImmutableString(const char* sData) {
        const size_t iLength = strlen(sData);
        m_sData = Malloc<char>(iLength+1);
        m_sData[iLength] = '\0';
        memcpy(m_sData, sData, iLength);
    }

    CImmutableString::CImmutableString(const CImmutableString& Other) {
        const size_t iLength = Other.Length();
        m_sData = Malloc<char>(iLength+1);
        m_sData[iLength] = '\0';
        memcpy(m_sData, Other.String(), iLength);
    }
    
    CImmutableString& CImmutableString::operator=(const CImmutableString& Other) {
        const size_t iLength = Other.Length();
        m_sData = Realloc<char>(m_sData, iLength+1);

        return *this;
    }

    bool CImmutableString::operator==(const char* sOther) const {
        if(sOther == NULL && m_sData == NULL) { return true; }
        if(sOther == NULL) { return false; }
        if(m_sData == NULL) { return false; }
        return strcmp(sOther, m_sData) == 0;
    }

    bool CImmutableString::operator<(const char* sOther) const {
        if(sOther == NULL) { return false; }
        if(m_sData == NULL) { return true; }
        return strcmp(sOther, m_sData) < 0;
    }

    bool CImmutableString::operator>(const char* sOther) const {
        if(sOther == NULL) { return true; }
        if(m_sData == NULL) { return false; }
        return strcmp(sOther, m_sData) > 0;
    }

    CImmutableString::operator cbpp::cstring_t() const {
        return (cstring_t)m_sData;
    }

    cstring_t CImmutableString::String() const {
        return (cstring_t)m_sData;
    }

    size_t CImmutableString::Length() const {
        return strlen(m_sData);
    }

    CImmutableString::~CImmutableString() {
        if(m_sData != NULL) { Free(m_sData); }
    }

    CImmutableString::operator bool() const {
        return m_sData != NULL;
    }
}
