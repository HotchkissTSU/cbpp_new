#ifndef CBPP_CML_TOKENS_H
#define CBPP_CML_TOKENS_H

/*
    CBPP Markdown Language
*/

#include <stdint.h>

#include "cbpp_api/Array.h"
#include "cbpp_api/String.h"

namespace cbpp::cml {
    enum class EToken : uint32_t {
        Keyword,
        Identifier,
        Assign,
        BlockOpen,
        BlockClose,
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
        InComment,
        Error
    };

    /*class CTokenValue {
        union ValueType {
            int32_t I32;
            float F32;
            cbpp::CString S;
        } Value;

        public:
            CTokenValue(EToken iType, const char* sData);

            ValueType& GetValue();
    };*/

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

        public:
            void ProcessCharacter(char cCurrent);
            void Finish();

            void Print();
    };
}

#endif
