#ifndef CBPP_CML_TOKENS_H
#define CBPP_CML_TOKENS_H

/*
    CBPP Markdown Language

    For more info see docs/CML.txt
*/

#include <stdint.h>
#include <uchar.h>

#include "cbpp/Array.h"
#include "cbpp/Table.h"
#include "cbpp/String.h"
#include "cbpp/Stack.h"

// Maximal stack depth for the CML parser
#define CBPP_CML_MAX_DEPTH 128

/*
    Maximal length for a single lexem
    Affects variable names and all values which are directly written into the code.
*/
#define CBPP_CML_MAX_LEXEM_LENGTH 128

#define CBPP_CML_LEXEM_BUFFER_SIZE (CBPP_CML_MAX_LEXEM_LENGTH+1)

namespace cbpp::cml {
    enum class EToken : uint32_t {
        Keyword,
        Identifier,
        BlockOpen,
        BlockClose,
        ArrayOpen,
        ArrayClose,
        Number,
        String,
        StructOpen,
        StructClose,

        AMOUNT
    };

    enum class ETokenizerState : uint32_t {
        Start,
        InNumber,
        InIdentifier,
        InString,
        InBlock,
        InComment
    };

    enum class EQualifier : uint32_t {
        None,
        FileTextRef,
        FileBinRef
    };
    
    enum class EValueType : uint32_t {
        Integer,
        Float,
        String,
        Binary,
        
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
        BadFileRef,             // Can`t open said file path

        BadNumber,

        StackOverflow,

        IWannaInclude
    };

    const char* GetTokenName(EToken iType);

    bool IsKeyword(const char* sData);

    struct Token {
        EToken iType;
        cbpp::CSubString sLexeme;
        size_t iLine, iColumn;
        EQualifier iRef = EQualifier::None;
    };

    class CTokenizer {
        const cbpp::CString& m_sSource;

        cbpp::CArray<Token> m_aTokens;
        
        size_t m_iLexemStart = 0, m_iLexemLength = 0;
        size_t m_iCounter = 0;

        ETokenizerState m_iState = ETokenizerState::Start;
        size_t m_iLine = 1, m_iCol = 1;
        bool m_bHasBSlash = false;

        EQualifier m_iRefType = EQualifier::None;

        char32_t m_iCurrentChar;

        void ProcessLastChar(char cCurrent);
        void ProcessCharacter(char cCurrent);

        public:
            CTokenizer(const CString& sSource);

            void ProcessString();

            void Finish();

            CArray<Token>& GetTokens();

            void Print();
    };

    class CValue {
        friend class CParser;
        friend class CValueObject;

        union {
            int32_t i32;
            float f32;
            char* str;
        } m_Value;

        EValueType m_iType = EValueType::Integer;
        size_t m_iLength = 0;

        public:
            CValue() = default;

            CValue(int32_t iValue);
            CValue(float fValue);
            CValue(const char* sValue);
            CValue(const char* pValue, size_t iLength);

            void SetType(EValueType iType);

            void SetValue(int32_t iValue);
            void SetValue(float iValue);
            void SetValue(const char* sValue);
            void SetValue(const char* pValue, size_t iLength);

            int32_t GetInt() const;
            float GetFloat() const;
            const char* GetString() const;
            const char* GetBinary() const;

            size_t GetLength() const;

            EValueType Type() const;

            ~CValue();
    };

    class IObject {
        friend IObject* CreateObject(EValueType);

        friend class CObject;
        friend class CArrayObject;

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

            virtual IObject* GetCopy() = 0;

            virtual ~IObject() = default;
    };

    IObject* CreateObject(EValueType iType);
    void PrintObject(IObject* pObj, size_t iDepth = 0, const char* sName = "ROOT");
    void WriteObject(IObject* pObject, FILE* pStream = stdout);

    bool CheckBraceMatch(EToken iOpener, EToken iCloser);
    const char* GetErrorName(EErrorType iType);

    class CParser {
        typedef EErrorType (CParser::*procfunc_t)(Token&);

        CStack<IObject*> m_aStack;
        
        CStack<EToken> m_aObjBraceStack, m_aArrBraceStack;

        CString m_sSource;

        CBinTable<CSubString, IObject*> m_dConstants;

        Token m_ErroredToken;
        EErrorType m_iLastError = EErrorType::Ok;

        IObject* m_pRoot = CreateObject(EValueType::Object);

        bool m_bExpectObject = false, m_bDeclaring = false;

        CSubString m_sCurrentIdentifier;

        char* LexemBuffer() const;

        IObject* CreateRefObject(Token& Data);

        EErrorType ProcessIdentifier(Token& Data);
        EErrorType ProcessNumber(Token& Data);
        EErrorType ProcessString(Token& Data);
        EErrorType ProcessKeyword(Token& Data);
        EErrorType ProcessBlock(Token& Data);
        EErrorType ProcessArray(Token& Data);
        EErrorType ProcessStruct(Token& Data);

        EErrorType ProcessToken(Token& Data);

        constexpr static procfunc_t m_aTokenProcessors[] = {
            &CParser::ProcessKeyword,
            &CParser::ProcessIdentifier,
            &CParser::ProcessBlock,
            &CParser::ProcessBlock,
            &CParser::ProcessArray,
            &CParser::ProcessArray,
            &CParser::ProcessNumber,
            &CParser::ProcessString,
            &CParser::ProcessStruct,
            &CParser::ProcessStruct
        };

        public:
            CParser();

            bool ParseString(const char* sCode);

            bool HasErrors() const;
            size_t GetErrorLog(char* sBuffer, size_t iMaxSize) const;

            void Reset();
            void Print() const;

            IObject* Root();

            ~CParser();
    };
}

#endif
