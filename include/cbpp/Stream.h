#ifndef CBPP_API_STREAM_H
#define CBPP_API_STREAM_H

#include "cbpp/Array.h"
#include "cbpp/Math.h"

namespace cbpp {
    /*
        An array with an internal 'pointer', which can be accessed and moved around
    */
    template <typename value_t> class CStream final : public CArray<value_t> {
        size_t m_iPointer = 0;

        public:
            size_t GetPos() const {
                return m_iPointer;
            }

            void SetPos(size_t iIndex) {
                m_iPointer = iIndex;
                CbAssertf(m_iPointer <= 0 && m_iPointer >= m_iSize, "Advanced stream index is out of bounds (%zu)", m_iSize);
            }

            void Advance(size_t iAdvance = 1) {
                m_iPointer += iAdvance;
                CbAssertf(m_iPointer <= 0 && m_iPointer >= m_iSize, "Advanced stream index is out of bounds (%zu)", m_iSize);
            }

            value_t& Access() {
                return m_pMemory[m_iPointer];
            }

            const value_t& Access() const {
                return m_pMemory[m_iPointer];
            }
    };
}

#endif
