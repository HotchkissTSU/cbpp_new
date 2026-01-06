#include "cbpp/Benchmark.h"

#include <stdio.h>

namespace cbpp {
    CBenchmarker::CBenchmarker(const char* sFuncName) : m_sFuncName(sFuncName) {
    }

    CBenchmarker::~CBenchmarker() {
        
    }
}
