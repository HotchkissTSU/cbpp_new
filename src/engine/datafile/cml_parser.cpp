#include "cbpp/CML.h"

#include <ctype.h>
#include <stdio.h>

namespace cbpp::cdf {
    const char* StringError(ETextError iType) {
        switch(iType) {
            case ETextError::Ok:                return "Ok";
            case ETextError::IllBlock:          return "Bad block";
            case ETextError::IllArray:          return "Bad array";
            case ETextError::Redefinition:      return "Redifinition in the local scope";
            case ETextError::StrayBlock:        return "Stray block";
            case ETextError::StrayArray:        return "Stray array";
            case ETextError::StrayIdentifier:   return "Stray identifier";
            case ETextError::StrayNumber:       return "Stray number";
            case ETextError::StrayString:       return "Stray string";
            case ETextError::BadFileRef:        return "The referenced file could not be found";
            case ETextError::BadNumber:         return "Not a number";
            case ETextError::StackOverflow:     return "Stack overflow";
            case ETextError::StackUnderflow:    return "Stack underflow";
            case ETextError::NoFile:            return "Source file not found";
            case ETextError::UnexpectedEOF:     return "Unexpected EOF";
            case ETextError::VersionMismatch:   return "Source version is incompatible";
            case ETextError::BadReference:      return "Non-string value is referenced";
            case ETextError::BadVersion:        return "Bad version, must be a positive integer";
            case ETextError::RefHugeFile:       return "The referenced file is too big";
            case ETextError::BadNumberSuffix:   return "Bad number type suffix";
            case ETextError::StrayKeyword:      return "Stray keyword";

            default:                            return "(null)";
        }
    }

    const char* StringError(EPathError iType) {
        switch(iType) {
            case EPathError::Ok:                return "Ok";
            case EPathError::ObjIndex:          return "Indexing an object";
            case EPathError::NotFound:          return "Entry not found";
            case EPathError::ArrayAccess:       return "Accessing an array as a table";
            case EPathError::BadIndex:          return "Bad array index";
            case EPathError::BadSeparator:      return "Bad name separator";

            default:                            return "(null)";
        }
    }

    const char* ClassString(EObjectClass iClass) {
        switch (iClass) {
            case EObjectClass::Array:           return "Array";
            case EObjectClass::Binary:          return "Binary";
            case EObjectClass::Float:           return "Float";
            case EObjectClass::Integer:         return "Integer";
            case EObjectClass::Object:          return "Table";
            case EObjectClass::String:          return "String";
            
            default:                            return "(null)";
        }
    }

    CTextParser::IncludeNode::~IncludeNode() {
        if(pFile != NULL) {
            CloseFile(pFile);
            pFile = NULL;
        }
    }

    bool CTextParser::IsValidNameStart(int iChar) {
        return isalpha(iChar) || (iChar == '_');
    }

    CTextParser::EKeyword CTextParser::IsKeyword(const char* sName) {
        if( strcmp(sName, "include") == 0 ) {
            return EKeyword::Include;
        } else if( strcmp(sName, "version") == 0 ) {
            return EKeyword::Version;
        } else if( strcmp(sName, "true") == 0 ) {
            return EKeyword::True;
        } else if( strcmp(sName, "false") == 0 ) {
            return EKeyword::False;
        } else {
            return EKeyword::Name;
        }
    }

    ETextError CTextParser::AddFile(const char* sPath) {
        cbpp::IFile* pFile = cbpp::OpenFile(sPath, "rb");
        if(pFile == NULL) {
            return ETextError::NoFile;
        }

        m_aFilesStack.Push( {sPath, pFile, m_iLine} );

        m_iCol = 1;
        m_iLine = 1;

        return ETextError::Ok;
    }

    int CTextParser::Peek() {
        if(m_aFilesStack.Length() == 0) {
            return 0;
        }

        cbpp::IFile* pFile = m_aFilesStack.Head().pFile;

        int iChar = pFile->GetChar();

        if(iChar == EOF) {      // this file is over, step out and resume    
            m_iCol = 1;
            m_iLine = m_aFilesStack.Head().iLine;  

            m_aFilesStack.Pop();  

            if(m_aFilesStack.Length() > 0) {                
                return Peek();          // peek again
            } else {
                return 0;               // include stack is empty, we are basically done
            }
        }
        
        m_iCol++;
        if(iChar == '\n') {
            m_iCol = 1;
            m_iLine++;
        }

        return iChar;
    }

    ETextError CTextParser::ParseName(int iChar) {
        m_sLexemBuffer.Clear();

        m_sLexemBuffer.PushBack(iChar);

        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if( isalnum(iCurrent) || (iCurrent == '_') ) {
                m_sLexemBuffer.PushBack(iCurrent);
            } else {    // name ends
                m_sLexemBuffer.PushBack('\0');
                return ETextError::Ok;
            }
        }

        return ETextError::UnexpectedEOF;
    }
    
    ETextError CTextParser::ParseString(int iChar) {
        m_sLexemBuffer.Clear();

        bool bExpectEscape = false;
        
        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if(bExpectEscape == true) {
                if(iCurrent == 'n') {
                    m_sLexemBuffer.PushBack('\n');
                    bExpectEscape = false;

                } else if(iCurrent == 't') {
                    m_sLexemBuffer.PushBack('\t');
                    bExpectEscape = false;

                } else if(iCurrent == '\\') {
                    m_sLexemBuffer.PushBack('\\');
                    bExpectEscape = false;

                } else if(iCurrent == '"') {
                    m_sLexemBuffer.PushBack('"');
                    bExpectEscape = false;

                } else if(iCurrent == '\'') {
                    m_sLexemBuffer.PushBack('\'');
                    bExpectEscape = false;

                } else {
                    m_sLexemBuffer.PushBack('\0');
                    return ETextError::BadEscapeChar;
                }
                
            } else {
                if(iCurrent == '\\') {
                    bExpectEscape = true;
                    continue;
                }

                if(iCurrent == '"' || iCurrent == '\'') {
                    m_sLexemBuffer.PushBack('\0');
                    return ETextError::Ok;
                }

                m_sLexemBuffer.PushBack(iCurrent);
            }
        }

        return ETextError::UnexpectedEOF;
    }

    ETextError CTextParser::ParseNumber(int iChar) {
        m_sLexemBuffer.Clear();
        m_sLexemBuffer.PushBack(iChar);

        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if(isdigit(iCurrent) || (iCurrent == '.') || (iCurrent == 'e') || (iCurrent == 'E') || (iCurrent == '-')) {
                m_sLexemBuffer.PushBack(iCurrent);
            } else {
                if(isalpha(iCurrent) && islower(iCurrent)) { // type suffix
                    if(iCurrent == 'i') {
                        m_iForceNumberType = EForceNumberType::ForceInteger;
                    } else if(iCurrent == 'f') {
                        m_iForceNumberType = EForceNumberType::ForceFloat;
                    } else {
                        m_sLexemBuffer.PushBack('\0');
                        return ETextError::BadNumberSuffix;
                    }
                }

                m_sLexemBuffer.PushBack('\0');
                return ETextError::Ok;
            }
        }
        
        return ETextError::UnexpectedEOF;
    }

    CObject CTextParser::ResolveFileRef(ETextError& iCode) {
        cbpp::IFile* pFile = OpenFile(m_sLexemBuffer.Data(), "rb");

        if(pFile == NULL) {
            iCode = ETextError::BadFileRef;
            return cdf::NIL;
        }

        char* pBuffer = NULL;
        const size_t iFileLen = pFile->Length();

        if(iFileLen > CBPP_CML_MAX_REFFILE) {
            iCode = ETextError::RefHugeFile;
            return cdf::NIL;
        }

        CObject pRet;

        if(m_iRefType == ERefType::Text) {
            pBuffer = Malloc<char>( iFileLen+1 );
            pBuffer[iFileLen] = '\0';
            pFile->ReadAll(pBuffer);

            pRet = CreateObject(EObjectClass::String);
            pRet = (const char*)(pBuffer);

        } else if(m_iRefType == ERefType::Binary) {
            pBuffer = Malloc<char>( iFileLen );
            pFile->ReadAll(pBuffer);

            pRet = CreateObject(EObjectClass::Binary);
            pRet.SetBinaryData((const uint8_t*)pBuffer, iFileLen);

        } else {
            CbAssert(true, "how?");
        }

        Free(pBuffer);
        return pRet;
    }

    void CTextParser::Reset() {
        m_bExpectInclude = false;
        m_bExpectValue = false;
        m_bExpectVersion = false;

        m_aFilesStack.Clear();
        m_aStack.Clear();
        m_iRefType = ERefType::NoLink;
        m_iForceNumberType = EForceNumberType::None;
        
        m_iCol = 1; m_iLine = 1;

        DeleteObject(m_pRootObject);
        m_pRootObject = cdf::NIL;
    }

    ETextError CTextParser::PushFloat(const char* sName, float_t fValue) {
        CObject pHead = m_aStack.Head();

        if(pHead[sName] != cdf::NIL) {
            return ETextError::Redefinition;
        }

        CObject pObj = CreateObject(EObjectClass::Float);
        pObj = fValue;

        if( sName == NULL ) {
            pHead.Push(fValue);
        } else {
            pHead.Push(sName, fValue);
        }

        return ETextError::Ok;
    }

    ETextError CTextParser::PushInt(const char* sName, int_t fValue) {
        if(m_aStack.Length() == 0) {
            return ETextError::StackUnderflow;
        }

        CObject pHead = m_aStack.Head();

        if(pHead[sName] != cdf::NIL) {
            return ETextError::Redefinition;
        }

        CObject pObj = CreateObject(EObjectClass::Integer);
        pObj = fValue;

        if( sName == NULL ) {
            pHead.Push(fValue);
        } else {
            pHead.Push(sName, fValue);
        }

        return ETextError::Ok;
    }

    ETextError CTextParser::Parse(const char* sPath, bool bAllowIncludes) {
        this->Reset();

        ETextError iRet = AddFile(sPath);

        if(iRet != ETextError::Ok) { return iRet; }

        m_pRootObject = CreateObject(EObjectClass::Object);

        m_aStack.Push(m_pRootObject);

        bool bInsideArray = false;

        int iCurrent = 1;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if(this->IsValidNameStart(iCurrent)) {                  // NAME
                ETextError iRet = this->ParseName(iCurrent);
                if(iRet != ETextError::Ok) { return iRet; }

                EKeyword iKW = this->IsKeyword(m_sLexemBuffer.Data());

                switch (iKW) {
                    case EKeyword::Name: {
                        if(m_bExpectValue || bInsideArray) {
                            return ETextError::StrayIdentifier;
                        }

                        m_sCurrentName.Set( m_sLexemBuffer.Data() );
                        m_bExpectValue = true;
                        break;
                    }
                    
                    case EKeyword::Version: {
                        if(m_bExpectValue || m_bExpectVersion) {
                            return ETextError::StrayKeyword;
                        }

                        m_bExpectVersion = true;
                        break;
                    }
                    
                    case EKeyword::Include: {
                        if(m_bExpectValue || m_bExpectInclude) {
                            return ETextError::StrayKeyword;
                        }

                        m_bExpectInclude = true;
                        break;
                    }

                    case EKeyword::True: {
                        if(!m_bExpectValue) {
                            return ETextError::StrayKeyword;
                        }

                        const char* sName = bInsideArray ? NULL : m_sCurrentName.String();
                        this->PushInt( sName, 1 );
                        m_bExpectValue = false;
                        break;
                    }

                    case EKeyword::False: {
                        if(!m_bExpectValue) {
                            return ETextError::StrayKeyword;
                        }

                        const char* sName = bInsideArray ? NULL : m_sCurrentName.String();
                        this->PushInt( sName, 0 );
                        m_bExpectValue = false;
                        break;
                    }
                }
                
            } else if(iCurrent == '"' || iCurrent == '\'') {        // STRING
                if(!m_bExpectValue && !bInsideArray && !m_bExpectInclude) {
                    return ETextError::StrayString;
                }

                ETextError iRet = this->ParseString(iCurrent);
                if(iRet != ETextError::Ok) { return iRet; }

                if(m_bExpectInclude) {
                    if(bAllowIncludes) {
                        iRet = AddFile(m_sLexemBuffer.Data());  // including
                        if(iRet != ETextError::Ok) {
                            return iRet;
                        }
                    }

                    m_bExpectInclude = false;

                } else {
                    CObject pStringObj;

                    CObject pHead = m_aStack.Head();
                    if(!bInsideArray && pHead[m_sCurrentName.String()] != cdf::NIL) {
                        return ETextError::Redefinition;
                    }

                    if(m_iRefType == ERefType::NoLink) {
                        pStringObj = CreateObject(EObjectClass::String);
                        pStringObj = m_sLexemBuffer.Data();
                    } else {
                        pStringObj = this->ResolveFileRef(iRet);

                        if(pStringObj == cdf::NIL) {
                            return iRet;
                        }

                        m_iRefType = ERefType::NoLink;
                    }

                    if(!bInsideArray) {
                        pHead.Push(m_sCurrentName.String(), pStringObj);
                    } else {
                        pHead.Push(pStringObj);
                    }

                    m_bExpectValue = false;
                }

            } else if(isdigit(iCurrent) || (iCurrent == '-') || (iCurrent == '.')) {    // NUMBER
                if(!m_bExpectValue && !bInsideArray && !m_bExpectVersion) {
                    return ETextError::StrayNumber;
                }

                if(m_iRefType != ERefType::NoLink) {
                    return ETextError::BadReference;
                }

                ETextError iRet = this->ParseNumber(iCurrent);
                if(iRet != ETextError::Ok) { return iRet; }

                int iNumberTest = cbpp::IsNumber(m_sLexemBuffer.Data());

                float_t fData = 0.0f;
                int_t iData = 0;
                EObjectClass iClass = EObjectClass::Integer;

                switch (iNumberTest) {
                    case 1: {
                        iClass = EObjectClass::Float;
                        fData = (float_t)strtof(m_sLexemBuffer.Data(), NULL);
                        break;
                    }
                        
                    case 0: {
                        iClass = EObjectClass::Integer;
                        iData = (int_t)atoi(m_sLexemBuffer.Data());
                        break;
                    }

                    case -1: {
                        return ETextError::BadNumber;
                    }
                }
                
                switch (m_iForceNumberType) {
                    case EForceNumberType::ForceInteger: {
                        iClass = EObjectClass::Integer;
                        iData = (fData != 0.0f) ? (int_t)fData : iData;         // apply the forced number type
                        break;
                    }

                    case EForceNumberType::ForceFloat: {
                        iClass = EObjectClass::Float;
                        fData = (iData != 0) ? (float_t)iData : fData;
                        break;
                    }
                }
                
                m_iForceNumberType = EForceNumberType::None;
                
                if(m_bExpectVersion) {
                    if(iNumberTest != 0 || iData <= 0) {
                        return ETextError::BadVersion;
                    }

                    if(iData > CBPP_CML_VERSION || iData < CBPP_CML_VERSION_LEAST) {
                        return ETextError::VersionMismatch;
                    }

                    m_bExpectVersion = false;

                } else {
                    const char* sName = bInsideArray ? NULL : m_sCurrentName.String();

                    if(iClass == EObjectClass::Integer) {
                        this->PushInt(sName, iData);
                    } else {
                        this->PushFloat(sName, fData);
                    }

                    m_bExpectValue = false;
                }
                
            } else if(iCurrent == '{') {                    // BLOCK OPENING
                if(!m_bExpectValue && !bInsideArray) {
                    return ETextError::StrayBlock;
                }

                if(m_iRefType != ERefType::NoLink) {
                    return ETextError::BadReference;
                }

                if(m_aStack.Length() == CBPP_CML_STACK_LIMIT) {
                    return ETextError::StackOverflow;
                }
                
                CObject pHead = m_aStack.Head();
                if(!bInsideArray && pHead[m_sCurrentName.String()] != cdf::NIL) {
                    return ETextError::Redefinition;
                }
                
                CObject pDict = CreateObject(EObjectClass::Object);

                if(!bInsideArray) {
                    pHead.Push(m_sCurrentName.String(), pDict);
                } else {
                    pHead.Push(pDict);
                }

                m_aStack.Push(pDict);
                
                m_bExpectValue = false;

            } else if(iCurrent == '}') {                    // BLOCK CLOSING
                if(m_aStack.Length() <= 1) {
                    return ETextError::StackUnderflow;
                }

                m_aStack.Pop();

            } else if(iCurrent == '[') {                    // ARRAY OPENING
                if(!m_bExpectValue && !bInsideArray) {
                    return ETextError::StrayArray;
                }

                if(m_iRefType != ERefType::NoLink) {
                    return ETextError::BadReference;
                }

                if(m_aStack.Length() == CBPP_CML_STACK_LIMIT) {
                    return ETextError::StackOverflow;
                }

                CObject pHead = m_aStack.Head();
                if(!bInsideArray && pHead[m_sCurrentName.String()] != cdf::NIL) {
                    return ETextError::Redefinition;
                }

                CObject pArr = CreateObject(EObjectClass::Array);
        
                if(!bInsideArray) {
                    pHead.Push(m_sCurrentName.String(), pArr);
                } else {
                    pHead.Push(pArr);
                }

                m_aStack.Push(pArr);

                m_bExpectValue = false;

            } else if(iCurrent == ']') {                    // ARRAY CLOSING
                if(m_aStack.Length() <= 1) {
                    return ETextError::StackUnderflow;
                }

                m_aStack.Pop();

            } else if(iCurrent == '#') {                    // COMMENTARY
                int iCommChar = 1;
                while(iCommChar != 0) {
                    iCommChar = this->Peek();
                    if(iCommChar == '\n' || iCommChar == '#') {
                        break;
                    }
                }

            } else if(iCurrent == '@') {
                m_iRefType = ERefType::Text;

            } else if(iCurrent == '&') {
                m_iRefType = ERefType::Binary;
                
            } else if(iCurrent == '\n') {
                if(m_bExpectInclude || m_bExpectVersion) {
                    m_iLine -= 2;
                    return ETextError::StrayKeyword;
                }
            }
            
            bInsideArray = m_aStack.Head().Class() == EObjectClass::Array;
        }
        
        return ETextError::Ok;
    }

    size_t CTextParser::FormatError(ETextError iCode, char* sBuffer, size_t iBufferLn) {
        const char* sError = StringError(iCode);
        const char* sFileSrc;
        
        if(m_aFilesStack.Length() > 0) {
            sFileSrc = m_aFilesStack.Head().sPath.String();
        } else {
            sFileSrc = "(none)";
        }

        const char* pSlash = strrchr(sFileSrc, '/') ;
        const char* sFile = (pSlash == NULL) ? sFileSrc : pSlash + 1;

        return snprintf(sBuffer, iBufferLn, "%s:%i.%i %s (near '%s')", sFile, m_iLine, m_iCol, sError, m_sLexemBuffer.Data());
    }

    size_t CTextParser::FormatError(EPathError iCode, char* sBuffer, size_t iBufferLn) {
        return snprintf( sBuffer, iBufferLn, "%s", StringError(iCode) );
    }

    CObject CTextParser::Root() {
        return m_pRootObject;
    }

    EPathError CTextParser::GetPathError() const { return m_iPathError; }

    CObject CTextParser::operator[](const char* sPath) {
        char* pCurrent = (char*)sPath;

        cbpp::CArray<char> sName, sNumber;
        bool bInsideName = false;

        CObject pCurrentObj = m_pRootObject;

        while(*pCurrent != '\0') {
            char iCurrent = *pCurrent;

            if( IsValidNameStart(iCurrent) && !bInsideName ) {   // name starts
                bInsideName = true;
                sName.PushBack(iCurrent);
                pCurrent++;
                continue;
            }

            if( isalnum(iCurrent) || iCurrent == '_' ) {        // name continues
                sName.PushBack(iCurrent);
            } else {                                            // name ended
                sName.PushBack('\0');

                if(pCurrentObj.Class() == EObjectClass::Array) {
                    m_iPathError = EPathError::ArrayAccess;
                    return cdf::NIL;
                }

                CObject pTest = pCurrentObj[ (const char*)(sName.Data()) ];
                if(pTest == cdf::NIL) {
                    m_iPathError = EPathError::NotFound;
                    return cdf::NIL;
                }

                pCurrentObj = pTest;

                sName.Clear();
                bInsideName = false;
            }

            if(iCurrent == '[') {
                sNumber.Clear();

                while(*pCurrent != '\0' && *pCurrent != ']') {
                    if(isalnum(*pCurrent)) {
                        sNumber.PushBack(*pCurrent);
                    }
                    pCurrent++;
                }

                sNumber.PushBack('\0');
                int iNumberTest = cbpp::IsNumber(sNumber.Data());

                if( iNumberTest == 0 ) {
                    iNumberTest = atoi( sNumber.Data() );
                } else {
                    m_iPathError = EPathError::BadIndex;
                    return cdf::NIL;
                }

                CObject pTest = pCurrentObj[(size_t)iNumberTest];

                if(pTest == cdf::NIL) {
                    m_iPathError = EPathError::NotFound;
                    return cdf::NIL;
                }

                pCurrentObj = pTest;
                pCurrent++;
                
                continue;
            }

            if(iCurrent != '.' && !bInsideName) {
                m_iPathError = EPathError::BadSeparator;
                return cdf::NIL;
            }

            pCurrent++;
        }

        if(sName.Length() > 0) {
            sName.PushBack('\0');

            CObject pTest = pCurrentObj[ (const char*)(sName.Data()) ];
            if(pTest == cdf::NIL) {
                m_iPathError = EPathError::NotFound;
                return cdf::NIL;
            }

            return pTest;
        }

        return pCurrentObj;
    }

    CTextParser::~CTextParser() {
        DeleteObject(m_pRootObject);
    }
}
