#include "cbpp/Benchmark.h"

#include <stdio.h>
#include <math.h>
#include <stdint.h>

namespace cbpp {
    CBenchmarker::CBenchmarker(const char* sFuncName) : m_sFuncName(sFuncName) {
        m_iBegin = clock();
    }

    CBenchmarker::~CBenchmarker() {
        clock_t iEnd = clock();

        double fMS = (double)((1000.0 * (iEnd - m_iBegin)) / CLOCKS_PER_SEC);

        printf("Benchmark for %s: %.3fms\n", m_sFuncName, fMS);
    }
}
