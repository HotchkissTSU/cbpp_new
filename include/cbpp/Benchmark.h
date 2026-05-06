#ifndef CBPP_BENCHMARK_H
#define CBPP_BENCHMARK_H

#include "cbpp/Time.h"

/*
    Automatically benchmark from macro position to the current scope end
*/
#define CbBenchmark(_code) cbpp::CBenchmarker __cb_benchmark(__FUNCTION__)

namespace cbpp {
    class CBenchmarker {
        const char* m_sFuncName;
        
        uint64_t m_iCPUBeg, m_iWallBeg;

        public:
            CBenchmarker(const char* sFuncName);
            ~CBenchmarker();
    };
}

#endif
