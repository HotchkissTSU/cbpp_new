#include "Meta.h"

namespace bitum::meta {
    class CTest : public IMetaclass {
        int m_int;

        public:
            const CMetaFields& GetFields() const {
                static CMetaFields s_fields {
                    {
                        "test",
                        &m_int,
                        INT
                    }
                };

                return s_fields;
            }
    };    
}
