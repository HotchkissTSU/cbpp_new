#ifndef CBPP_CML_TOKENS_H
#define CBPP_CML_TOKENS_H

/*
    CBPP Markdown Language
*/

#include <stdint.h>

#include "cbpp_api/Array.h"
#include "cbpp_api/Table.h"
#include "cbpp_api/String.h"

namespace cbpp::cml {
    enum class EToken : uint32_t {
        Keyword,
        Identifier,
        BlockOpen,
        BlockClose,
        ArrayOpen,
        ArrayClose,
        Number,
        String
    };

    const char* GetTokenName(EToken iType);

    bool IsKeyword(const char* sData);

    enum class ETokenizerState : uint32_t {
        Start,
        InNumber,
        InIdentifier,
        InString,
        InBlock,
        InComment
    };

    struct Token {
        EToken iType;
        cbpp::CString sLexeme;
        size_t iLine, iColumn;
    };

    class CTokenizer {
        cbpp::CArray<Token> m_aTokens;
        cbpp::CArray<char> m_aCurrentLexeme;

        ETokenizerState m_iState = ETokenizerState::Start;
        size_t m_iLine = 1, m_iCol = 1;
        bool m_bHasBSlash = false;

        public:
            void ProcessString(const char* sString);

            void ProcessCharacter(char cCurrent);
            void Finish();

            CArray<Token>& GetTokens();

            void Print();
    };

    enum class EValueType : uint32_t {
        Integer,
        Float,
        String,
        
        Array,
        Object
    };

    enum class EErrorType : uint32_t {
        Ok,                     // We`re cool
        Redefinition,           // Multiple definition of an identifier in the single scope
        StrayIdentifier,        // Random identifier that does not connect to anything
        StrayNumber,            // Random out-of-context number
        StrayString,            // Random out-of-context string
        IllBlock,               // Badly formatted block (curvy braces mismatch)
        IllArray                // Badly formatted array (square braces mismatch)
    };

    struct ErrorInfo {
        EErrorType iType;
        size_t iLine, iCol;
    };

    class CValue {
        union {
            int32_t i32;
            float f32;
            char* str;
        } m_Value;

        EValueType m_iType = EValueType::Integer;

        public:
            CValue() = default;

            CValue(int32_t iValue);
            CValue(float fValue);
            CValue(const CString& sValue);

            int32_t& Int();
            float& Float();
            CString& String();

            ~CValue();
    };

    class IObject {
        public:
            virtual EValueType Type() = 0;
    };
}

#endif
