#ifndef CBPP_CML_OBJECTS_H
#define CBPP_CML_OBJECTS_H

#include "cbpp/CML.h"
#include "cbpp/String.h"
#include "cbpp/Array.h"
#include "cbpp/Table.h"

namespace cbpp::cml {
    class CBinaryObject final : public IObject {
        cbpp::CArray<uint8_t> m_aData;

        public:
            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;

            void SetValue(int_t);
            void SetValue(float_t);
            void SetValue(const char*);

            uint8_t* AsBinary();

            IObject* At(const char*);
            IObject* At(size_t);

            EObjectClass Class() const;

            size_t Length() const;

            virtual ~CBinaryObject() override = default;
    };

    class CIntObject final : public IObject {
        int_t m_iData = 0;

        public:
            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;

            void SetValue(int_t);
            void SetValue(float_t);
            void SetValue(const char*);

            IObject* At(const char*);
            IObject* At(size_t);

            EObjectClass Class() const;

            size_t Length() const;

            virtual ~CIntObject() override = default;
    };

    class CFloatObject final : public IObject {
        float_t m_fData = 0;

        public:
            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;

            void SetValue(int_t);
            void SetValue(float_t);
            void SetValue(const char*);

            IObject* At(const char*);
            IObject* At(size_t);

            EObjectClass Class() const;

            size_t Length() const;

            virtual ~CFloatObject() override = default;
    };

    class CStringObject final : public IObject {
        cbpp::CString m_sData;

        public:
            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;

            void SetValue(int_t);
            void SetValue(float_t);
            void SetValue(const char*);

            IObject* At(const char*);
            IObject* At(size_t);

            EObjectClass Class() const;

            size_t Length() const;

            virtual ~CStringObject() override = default;
    };

    class CArrayObject final : public IObject {
        cbpp::CArray<IObject*> m_pData;

        public:
            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;

            void SetValue(int_t);
            void SetValue(float_t);
            void SetValue(const char*);

            IObject* At(const char*);
            IObject* At(size_t);

            EObjectClass Class() const;

            size_t Length() const;

            virtual ~CArrayObject() override = default;
    };

    class CDictObject final : public IObject {
        cbpp::CBinTable<cbpp::CString, IObject*> m_dTable;

        public:
            int_t AsInt() const;
            float_t AsFloat() const;
            const char* AsString() const;

            void SetValue(int_t);
            void SetValue(float_t);
            void SetValue(const char*);

            IObject* At(const char*);
            IObject* At(size_t);

            EObjectClass Class() const;

            size_t Length() const;

            virtual ~CDictObject() override = default;
    };
}

#endif
