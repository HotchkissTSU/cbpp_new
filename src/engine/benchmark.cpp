#include "cbpp/Benchmark.h"

#include <stdio.h>
#include <math.h>
#include <stdint.h>

namespace cbpp {
    CBenchmarker::CBenchmarker(const char* sFuncName) : m_sFuncName(sFuncName) {
        m_iCPUBeg = GetCPUTime();
        m_iWallBeg = GetTime();
    }

    CBenchmarker::~CBenchmarker() {
        uint64_t iCPUEnd = GetCPUTime();
        uint64_t iWallEnd = GetTime();

        double fMSCPU = (double)((iCPUEnd - m_iCPUBeg)/1000000.0);
        double fMSWall = (double)((iWallEnd - m_iWallBeg)/1000000.0);

        printf("Benchmark for %s: CPU: %.3fms WALL: %.3fms\n", m_sFuncName, fMSCPU, fMSWall);
    }
}
