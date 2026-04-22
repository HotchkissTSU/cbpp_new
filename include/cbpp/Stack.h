#ifndef CBPP_STACK_H
#define CBPP_STACK_H

#include "cbpp/Array.h"

namespace cbpp {
    template <typename value_t> class CStack {
        CArray<value_t> m_aData;

        public:
            CStack() = default;

            value_t& Head() {
                return m_aData.Last();
            }

            const value_t& Head() const {
                return m_aData.Last();
            }

            void Pop() {
                m_aData.PopBack();
            }

            void Push(const value_t& Value) {
                m_aData.PushBack(Value);
            }

            void Push(value_t&& Value) {
                m_aData.PushBack(std::move(Value));
            }

            value_t& operator[](size_t iIndex) {
                return m_aData[iIndex];
            }

            size_t Length() const {
                return m_aData.Length();
            }

            void Strink() {
                m_aData.Shrink();
            }

            void Reserve(size_t iSize) {
                m_aData.Reserve(iSize);
            } 

            void Clear() {
                m_aData.Clear();
            }
    };

    /*
        Binary stack for byte arrays reading/composing.
        Only safe for primitive types, because it simply
        moves bytes around without RAII stuff
    */
    class CBinaryStack {
        uint8_t* m_pData = NULL;
        size_t m_iSize = 0;

        public:
            CBinaryStack() = default;
            CBinaryStack(const uint8_t* pData, size_t iLength);

            void Clear();
            const uint8_t* GetBuffer() const;
            uint8_t* GetBuffer();

            size_t Length() const;

            void PushBytes(const uint8_t* pData, size_t iLength);
            size_t PopBytes(uint8_t* pBuffer, size_t iLength);

            template <typename T> void PushData(const T& Data) { PushBytes((uint8_t*)(&Data), sizeof(Data)); }
            template <typename T> size_t PopData(T& Buffer) { return PopBytes((uint8_t*)(&Buffer), sizeof(T)); }

            ~CBinaryStack();
    };
}

#endif
