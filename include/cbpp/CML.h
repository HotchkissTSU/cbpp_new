#ifndef CBPP_CML_API_H
#define CBPP_CML_API_H

#include <stdint.h>
#include <stddef.h>

#include "cbpp/Filesystem.h"
#include "cbpp/Array.h"
#include "cbpp/Stack.h"

#define CBPP_CML_VERSION 100        // Actual language version
#define CBPP_CML_VERSION_LEAST 100  // Any versions below this are considered deprecated

#define CBPP_CML_STACK_LIMIT 128

namespace cbpp::cml {
    typedef int32_t int_t;
    typedef float float_t;

    enum class EObjectClass {
        Object,
        Array,
        Binary,
        Integer,
        Float,
        String
    };

    const char* ClassString(EObjectClass);

    enum class EErrorType : uint32_t {
        Ok,                     // We`re cool

        Redefinition,           // Multiple definition of an identifier in the single scope
        StrayIdentifier,        // Random identifier that does not connect to anything
        StrayNumber,            // Random out-of-context number
        StrayString,            // Random out-of-context string
        StrayBlock,             // Nameless block
        StrayArray,             // Nameless array
        IllBlock,               // Badly formatted block (curvy braces mismatch)
        IllArray,               // Badly formatted array (square braces mismatch)

        BadFileRef,             // Can`t open said file path
        BadReference,           // Anything except string is marked as reference

        BadNumber,              // Badly formatted number
        NoFile,                 // Source file not found
        UnexpectedEOF,          // EOF jumpscare in the middle of something
        VersionMismatch,        // Source file`s CML version is newer or too old
        BadVersion,             // Version is negative or is a float

        StackOverflow,          // Stack depth exceeded
        StackUnderflow          // Attempt to pop an empty stack
    };

    enum class EKeyword : uint32_t {
        Name,
        Include,
        Version
    };

    enum class ERefType : uint32_t {
        NoLink,
        Text,
        Binary
    };

    const char* StringError(EErrorType);

    class IObject;

    class CObject {
        IObject* m_pObj = NULL;

        public:
            CObject() = default;
            CObject(IObject* pData);
            operator IObject*() const;
            IObject* GetPointer() const;

            EObjectClass Class() const;
            size_t Length() const;

            bool operator==(const CObject& pOther) const;
            bool operator!=(const CObject& pOther) const;

            CObject& operator=(int_t iValue);
            CObject& operator=(float_t fValue);
            CObject& operator=(const char* sValue);
            void SetBinaryData(const uint8_t* pData, size_t iLength);

            CObject operator[](size_t iIndex);
            CObject operator[](const char* sName);

            const char* IndexName(size_t iIndex) const;

            void Push(CObject pObj);
            void Push(const char* sName, CObject pObj);

            operator int_t() const;
            operator float_t() const;
            operator const char*() const;
            operator uint8_t*();
    };

    void PrintObject(CObject pObj, size_t iDepth = 0);

    // Null value to signal errors
    extern const CObject NIL;

    CObject CreateObject(EObjectClass iClass);
    void DeleteObject(CObject pObj);

    class CParser {
        struct IncludeNode {
            cbpp::CString sPath;
            cbpp::IFile* pFile = NULL;

            size_t iLine = 1;

            ~IncludeNode();
        };

        cbpp::CArray<char> m_sLexemBuffer;
        cbpp::CString m_sCurrentName;

        cbpp::CStack<IncludeNode> m_aFilesStack;

        CObject m_pRootObject = NIL;

        size_t m_iLine = 0, m_iCol = 0;

        bool m_bExpectValue = false;
        bool m_bExpectVersion = false;
        bool m_bExpectInclude = false;

        ERefType m_iRefType = ERefType::NoLink;

        int Peek();

        bool IsValidNameStart(int);
        bool CheckRedef(const char*);
        EKeyword IsKeyword(const char*);

        EErrorType ParseName(int);
        EErrorType ParseString(int);
        EErrorType ParseNumber(int);

        EErrorType AddFile(const char*);

        CObject ResolveFileRef();

        public:
            CParser() = default;

            void Reset();

            EErrorType Parse(const char* sPath);

            CObject Root();
            CObject AccessPath(const char* sPath);

            size_t FormatError(EErrorType iCode, char* sBuffer, size_t iBufferLn);

            ~CParser();
    };
}

#endif
