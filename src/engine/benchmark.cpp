#include "cbpp/Benchmark.h"

#include <stdio.h>
#include <math.h>

namespace cbpp {
    CBenchmarker::CBenchmarker(const char* sFuncName) : m_sFuncName(sFuncName) {
        m_iBegin = clock();
    }

    CBenchmarker::~CBenchmarker() {
        clock_t iEnd = clock();
        int iMS = (int)(((double)(iEnd - m_iBegin) / (double)CLOCKS_PER_SEC) * 1000000.0);
        printf("Benchmark for %s: %ims\n", m_sFuncName, iMS);
    }
}
