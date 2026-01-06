#ifndef CBPP_BENCHMARK_H
#define CBPP_BENCHMARK_H

#include <time.h>

#define CbBenchmark() cbpp::CBenchmarker __cb_benchmark(__FUNCTION__);

namespace cbpp {
    class CBenchmarker {
        const char* m_sFuncName;
        timeval m_iBegin;

        public:
            CBenchmarker(const char* sFuncName);
            ~CBenchmarker();
    };
}

#endif
