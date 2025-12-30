#include "cbpp/cdf/object.h"

namespace cbpp::cdf {
    template <typename num_t> class CNumberObject : public CObjectStorage {
        public:
            CNumberObject(num_t Value) : CObjectStorage<num_t>(EObjectClass::Number, sizeof(num_t)) {
                this->PushData(&Value, sizeof(Value));
            }
    };

    typedef CNumberObject<int32_t> CInt32Object;
    typedef CNumberObject<int64_t> CInt64Object;
    typedef CNumberObject<float>   CFloatObject;
    typedef CNumberObject<double>  CDoubleObject;

    class CObject : public CObjectStorage {
        public:
            void PushObject(const char* sObjName, IObject* pChild);
    };
}
