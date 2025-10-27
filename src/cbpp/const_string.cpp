#include "cbpp/const_string.h"

#include <stddef.h>
#include <string.h>

#include "cbpp/malloc_wrapper.h"

namespace cbpp {
    CConstString::CConstString() : m_sData(NULL) {}
    CConstString::CConstString(const char* sData) : m_sData(sData) {}

    bool CConstString::operator==(const char* sOther) const {
        return strcmp(sOther, m_sData) == 0;
    }

    bool CConstString::operator<(const char* sOther) const {
        return strcmp(sOther, m_sData) < 0;
    }

    bool CConstString::operator>(const char* sOther) const {
        return strcmp(sOther, m_sData) > 0;
    }

    CConstString::operator cbpp::cstring_t() const {
        return m_sData;
    }

    cstring_t CConstString::String() const {
        return m_sData;
    }

    size_t CConstString::Length() const {
        return strlen(m_sData);
    }

    char* StringDup(const char* sSource, size_t* pLength) {
        const size_t iLength = strlen(sSource);
        char* pNew = Malloc<char>(iLength+1);
        memcpy(pNew, sSource, iLength+1);

        if(pLength != NULL) { *pLength = iLength; }

        return pNew;
    }

    CConstString::operator bool() const {
        return m_sData != NULL;
    }
}
