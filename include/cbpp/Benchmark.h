#ifndef CBPP_BENCHMARK_H
#define CBPP_BENCHMARK_H

#include <time.h>

/*
    Automatically benchmark from macro position to the current scope end
*/
#define CbBenchmark() cbpp::CBenchmarker __cb_benchmark(__FUNCTION__)

namespace cbpp {
    class CBenchmarker {
        const char* m_sFuncName;
        clock_t m_iBegin;

        public:
            CBenchmarker(const char* sFuncName);
            ~CBenchmarker();
    };
}

#endif
