#include "cbpp/Error.h"
#include "cbpp/String.h"

namespace cbpp {
    CLogger::CLogger(const char* sLogName) : m_sName(StringDup(sLogName)) {
        
    }
}
