#ifndef CBPP_CML_TOKENS_H
#define CBPP_CML_TOKENS_H

/*
    CBPP Markdown Language

    For more info see docs/CML.txt
*/

#include <stdint.h>
#include <uchar.h>

#include "cbpp_api/Array.h"
#include "cbpp_api/Table.h"
#include "cbpp_api/String.h"
#include "cbpp_api/Stack.h"

// Maximal stack depth for the CML parser
#define CBPP_CML_MAX_DEPTH 128

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
        cbpp::CSubString sLexeme;
        size_t iLine, iColumn;
        bool bIsRef = false;
    };

    class CTokenizer {
        const cbpp::CString& m_sSource;

        cbpp::CArray<Token> m_aTokens;
        
        size_t m_iLexemStart = 0, m_iLexemLength = 0;
        size_t m_iCounter = 0;

        ETokenizerState m_iState = ETokenizerState::Start;
        size_t m_iLine = 1, m_iCol = 1;
        bool m_bHasBSlash = false, m_bFileRefFlag = false;

        char32_t m_iCurrentChar;

        void ProcessCharacter(char cCurrent);

        public:
            CTokenizer(const CString& sSource);

            void ProcessString();

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
        StrayBlock,
        StrayArray,
        IllBlock,               // Badly formatted block (curvy braces mismatch)
        IllArray,               // Badly formatted array (square braces mismatch)
        BadFileRef
    };

    struct ErrorInfo {
        EErrorType iType;
        size_t iLine, iCol;
    };

    class CValue {
        friend class CParser;

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
            CValue(const char* sValue);

            void SetType(EValueType iType);

            void SetValue(int32_t iValue);
            void SetValue(float iValue);
            void SetValue(const char* sValue);

            int32_t GetInt() const;
            float GetFloat() const;
            const char* GetString() const;

            EValueType Type() const;

            ~CValue();
    };

    class IObject {
        friend IObject* CreateObject(EValueType);

        protected:
            IObject() = default;

        public:
            typedef CBinTable<CString, IObject*> objmap_t;
            typedef CArray<IObject*> objlist_t;

            virtual EValueType Type() const = 0;
            virtual CValue* Value() = 0;

            /*
                Get children table.
                Returns NULL if used on anything except EValueType::Object
            */
            virtual objmap_t* GetChildren() = 0;

            /*
                Get children list.
                Returns NULL if used on anything except EValueType::Array
            */
            virtual objlist_t* GetArray() = 0;

            /*
                Push a sub-object.
                Works only on objects and arrays, and name is ignored when
                pushing in the array
            */
            virtual bool PushChild(const char* sName, IObject* pChild) = 0;

            virtual bool HasChild(const char* sName) const = 0;
            virtual size_t Length() const = 0;

            virtual IObject* GetByName(const char* sName) const = 0;
            virtual IObject* GetByIndex(size_t iIndex) const = 0;

            virtual ~IObject() = default;
    };

    IObject* CreateObject(EValueType iType);
    void PrintObject(IObject* pObj, size_t iDepth = 0, const char* sName = "ROOT");
    void WriteObject(IObject* pObject, FILE* pStream = stdout);

    enum class EBraceType : uint32_t {
        Block,          // {}
        Array           // []
    };

    bool CheckBraceMatch(EToken iOpener, EToken iCloser);
    const char* GetErrorName(EErrorType iType);

    class CParser {
        CStack<IObject*> m_aStack;
        CStack<EToken> m_aBracesStack;

        CString m_sSource;

        Token m_ErroredToken;
        EErrorType m_iLastError = EErrorType::Ok;

        IObject* m_pRoot = CreateObject(EValueType::Object);

        bool m_bExpectObject = false;
        CSubString m_sCurrentIdentifier;

        EErrorType ProcessToken(Token& Data);

        public:
            bool ParseString(const char* sCode, bool bAllowInclude = true);

            bool HasErrors() const;
            size_t GetErrorLog(char* sBuffer, size_t iMaxSize) const;

            void Reset();
            void Print() const;

            IObject* Root();

            ~CParser();
    };
}

#endif
