#include "cbpp/CML.h"

#include <ctype.h>
#include <stdio.h>

namespace cbpp::cdf {
    const char* StringError(ETextError iType) {
        switch(iType) {
            case ETextError::Ok:                return "Ok";
            case ETextError::IllBlock:          return "Bad block";
            case ETextError::IllArray:          return "Bad array";
            case ETextError::Redefinition:      return "Redefinition in the local scope";
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
            case ETextError::BadInheritance:    return "Only tables can inherit, and only from tables";
            case ETextError::ParentNotFound:    return "Parent object not found";
            case ETextError::InheritTypeMix:    return "Can`t overwrite values of different types";

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
        } else if( strcmp(sName, "inherit") == 0 ) {
            return EKeyword::Inherit;
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

    ETextError CTextParser::ProcessName() {
        EKeyword iKW = this->IsKeyword(m_sLexemBuffer.Data());

        switch (iKW) {
            case EKeyword::Name: {
                if(m_iPromise == EPromise::Value || m_bInsideArray) {
                    return ETextError::StrayIdentifier;
                }

                m_iPromise = EPromise::Value;

                m_sCurrentName.Set( m_sLexemBuffer.Data() );
                break;
            }
            
            case EKeyword::Version: {
                if(m_iPromise != EPromise::None) {
                    return ETextError::StrayKeyword;
                }

                m_iPromise = EPromise::Version;

                break;
            }
            
            case EKeyword::Include: {
                if(m_iPromise != EPromise::None) {
                    return ETextError::StrayKeyword;
                }

                m_iPromise = EPromise::IncludePath;

                break;
            }

            case EKeyword::True: {
                if(m_iPromise != EPromise::Value && !m_bInsideArray) {
                    return ETextError::StrayKeyword;
                }

                m_iPromise = EPromise::None;

                const char* sName = m_bInsideArray ? NULL : m_sCurrentName.String();
                this->PushInt( sName, 1 );
                break;
            }

            case EKeyword::False: {
                if(m_iPromise != EPromise::Value && !m_bInsideArray) {
                    return ETextError::StrayKeyword;
                }

                m_iPromise = EPromise::None;

                const char* sName = m_bInsideArray ? NULL : m_sCurrentName.String();
                this->PushInt( sName, 0 );
                break;
            }

            case EKeyword::Inherit: {
                if(m_iPromise != EPromise::None) {
                    return ETextError::StrayKeyword;
                }

                if( m_aStack.Head().Class() != EObjectClass::Object ) {
                    return ETextError::BadInheritance;
                }

                m_iPromise = EPromise::ParentPath;

                break;
            }
        }

        return ETextError::Ok;
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

                ETextError iRet = this->ProcessName();
                if(iRet != ETextError::Ok) { return iRet; }

                iRet = this->ProcessChar(iCurrent);
                if(iRet != ETextError::Ok) { return iRet; }

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
                bExpectEscape = false;

                switch (iCurrent) {
                    case 'n': {
                        m_sLexemBuffer.PushBack('\n');
                        break;
                    }

                    case 't': {
                        m_sLexemBuffer.PushBack('\t');
                        break;
                    }

                    case '\\': {
                        m_sLexemBuffer.PushBack('\\');
                        break;
                    }

                    case '"': {
                        m_sLexemBuffer.PushBack('"');
                        break;
                    }

                    default: {
                        m_sLexemBuffer.PushBack('\0');
                        return ETextError::BadEscapeChar;
                    }
                }
                
            } else {
                if(iCurrent == '\\') {
                    bExpectEscape = true;
                    continue;
                }

                if(iCurrent == '"') {
                    m_sLexemBuffer.PushBack('\0');
                    return ETextError::Ok;
                }

                m_sLexemBuffer.PushBack(iCurrent);
            }
        }

        return ETextError::UnexpectedEOF;
    }

    ETextError CTextParser::ProcessNumber() {
        if(!m_bInsideArray && m_iPromise == EPromise::None) {
            return ETextError::StrayNumber;
        }

        if(m_iRefType != ERefType::NoLink) {
            return ETextError::BadReference;
        }

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
        
        if(m_iPromise == EPromise::Version) {
            if(iNumberTest != 0 || iData <= 0) {
                return ETextError::BadVersion;
            }

            if(iData > CBPP_CML_VERSION || iData < CBPP_CML_VERSION_LEAST) {
                return ETextError::VersionMismatch;
            }
            
        } else {
            if(!m_bInsideArray && m_iPromise == EPromise::None) {
                return ETextError::StrayNumber;
            }

            const char* sName = m_bInsideArray ? NULL : m_sCurrentName.String();

            if(iClass == EObjectClass::Integer) {
                this->PushInt(sName, iData);
            } else {
                this->PushFloat(sName, fData);
            }
        }

        m_iPromise = EPromise::None;

        return ETextError::Ok;
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
                m_sLexemBuffer.PushBack('\0');

                if(isalpha(iCurrent)) { // we have a type suffix
                    if(iCurrent == 'i') {
                        m_iForceNumberType = EForceNumberType::ForceInteger;
                    } else if(iCurrent == 'f') {
                        m_iForceNumberType = EForceNumberType::ForceFloat;
                    } else {
                        return ETextError::BadNumberSuffix;
                    }
                }
                
                ETextError iRet = this->ProcessNumber();
                if(iRet != ETextError::Ok) { return iRet; }

                if( m_iForceNumberType == EForceNumberType::None ) {
                    iRet = this->ProcessChar(iCurrent);
                } else {
                    m_iForceNumberType = EForceNumberType::None;
                }

                if(iRet != ETextError::Ok) { return iRet; }

                return ETextError::Ok;
            }
        }
        
        return ETextError::UnexpectedEOF;
    }

    ETextError CTextParser::ProcessChar(int iCurrent) {
        if(this->IsValidNameStart(iCurrent)) {                                      // NAME
            ETextError iRet = this->ParseName(iCurrent);
            if(iRet != ETextError::Ok) { return iRet; }
            
        } else if(iCurrent == '"') {                                                // STRING
            if(!m_bInsideArray && m_iPromise == EPromise::None) {
                return ETextError::StrayString;
            }
            
            ETextError iRet = this->ParseString(iCurrent);
            if(iRet != ETextError::Ok) { return iRet; }

            switch(m_iPromise) {
                case EPromise::ParentPath: {
                    CObject pHead = m_aStack.Head();
                    CObject pParent = m_pRootObject.Access(m_sLexemBuffer.Data());

                    if(pParent == cdf::NIL) {
                        return ETextError::ParentNotFound;
                    }
                    
                    for(size_t i = 0; i < pParent.Length(); i++) {
                        const char* sName = pParent.IndexName(i);
                        CObject pParents = pParent[i];
                        CObject pOurs = pHead[sName];

                        if(pOurs == cdf::NIL) {
                            pHead.Push(sName, CopyObject(pParents));            // new value
                            
                        } else {
                            if(pOurs.Class() != pParents.Class()) {
                                return ETextError::InheritTypeMix;
                            }
                            
                            if(pOurs.Class() == EObjectClass::Array) {          // arrays are concatenated
                                for(size_t k = 0; k < pParents.Length(); k++) {
                                    pOurs.Push( CopyObject(pParents[k]) );
                                }

                            } else {
                                DeleteObject(pOurs);                            // other values are overriden
                                pHead.Push(sName, CopyObject(pParents)); 
                            }
                        }
                    }
                    
                    break;
                }
                
                case EPromise::IncludePath: {
                    if(m_bAllowIncludes) {
                        iRet = AddFile(m_sLexemBuffer.Data());  // including
                        if(iRet != ETextError::Ok) {
                            return iRet;
                        }
                    }

                    break;
                }
                
                default: {
                    if(!m_bInsideArray && m_iPromise == EPromise::None) {
                        return ETextError::StrayString;
                    }

                    CObject pStringObj;

                    CObject pHead = m_aStack.Head();
                    if(!m_bInsideArray && pHead[m_sCurrentName.String()] != cdf::NIL) {
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

                    if(!m_bInsideArray) {
                        pHead.Push(m_sCurrentName.String(), pStringObj);
                    } else {
                        pHead.Push(pStringObj);
                    }
                }                    
            }
            
            m_iPromise = EPromise::None;
            
        } else if(isdigit(iCurrent) || (iCurrent == '-') || (iCurrent == '.')) {    // NUMBER
            ETextError iRet = this->ParseNumber(iCurrent);
            if(iRet != ETextError::Ok) { return iRet; }
            
        } else if(iCurrent == '{') {                                                // BLOCK OPENING
            if(!m_bInsideArray && m_iPromise == EPromise::None) {
                return ETextError::StrayBlock;
            }

            if(m_iRefType != ERefType::NoLink) {
                return ETextError::BadReference;
            }

            if(m_aStack.Length() == CBPP_CML_STACK_LIMIT) {
                return ETextError::StackOverflow;
            }
            
            CObject pHead = m_aStack.Head();
            if(!m_bInsideArray && pHead[m_sCurrentName.String()] != cdf::NIL) {
                return ETextError::Redefinition;
            }
            
            CObject pDict = CreateObject(EObjectClass::Object);

            if(!m_bInsideArray) {
                pHead.Push(m_sCurrentName.String(), pDict);
            } else {
                pHead.Push(pDict);
            }

            m_aStack.Push(pDict);
            
            m_iPromise = EPromise::None;

        } else if(iCurrent == '}' || iCurrent == ']') {                             // SCOPE CLOSING
            if(m_aStack.Length() <= 1) {
                return ETextError::StackUnderflow;
            }

            m_aStack.Pop();

        } else if(iCurrent == '[') {                                                // ARRAY OPENING
            if(m_iPromise == EPromise::None && !m_bInsideArray) {
                return ETextError::StrayArray;
            }

            if(m_iRefType != ERefType::NoLink) {
                return ETextError::BadReference;
            }

            if(m_aStack.Length() == CBPP_CML_STACK_LIMIT) {
                return ETextError::StackOverflow;
            }

            CObject pHead = m_aStack.Head();
            if(!m_bInsideArray && pHead[m_sCurrentName.String()] != cdf::NIL) {
                return ETextError::Redefinition;
            }

            CObject pArr = CreateObject(EObjectClass::Array);
    
            if(!m_bInsideArray) {
                pHead.Push(m_sCurrentName.String(), pArr);
            } else {
                pHead.Push(pArr);
            }

            m_aStack.Push(pArr);

            m_iPromise = EPromise::None;

        } else if(iCurrent == '#') {                                                // COMMENTARY
            int iCommChar = 1;
            while(iCommChar != 0) {
                iCommChar = this->Peek();
                if(iCommChar == '\n' || iCommChar == '#') {
                    break;
                }
            }
            
        } else if(iCurrent == '@') {
            m_iRefType = ERefType::Text;                                            // REFERENCES

        } else if(iCurrent == '&') {
            m_iRefType = ERefType::Binary;
            
        } else if(iCurrent == '\n') {
            if(m_iPromise != EPromise::None) {
                m_iLine -= 2;
                return ETextError::StrayKeyword;
            }
        }
        
        return ETextError::Ok;
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
        m_iPromise = EPromise::None;
        m_bInsideArray = false;

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

        if( sName == NULL ) {
            pHead.Push(fValue);
        } else {
            pHead.Push(sName, fValue);
        }

        return ETextError::Ok;
    }

    ETextError CTextParser::PushInt(const char* sName, int_t iValue) {
        if(m_aStack.Length() == 0) {
            return ETextError::StackUnderflow;
        }

        CObject pHead = m_aStack.Head();

        if(pHead[sName] != cdf::NIL) {
            return ETextError::Redefinition;
        }

        if( sName == NULL ) {
            pHead.Push(iValue);
        } else {
            pHead.Push(sName, iValue);
        }

        return ETextError::Ok;
    }

    ETextError CTextParser::Parse(const char* sPath, bool bAllowIncludes) {
        this->Reset();

        m_bAllowIncludes = bAllowIncludes;

        ETextError iRet = AddFile(sPath);

        if(iRet != ETextError::Ok) { return iRet; }

        m_pRootObject = CreateObject(EObjectClass::Object);

        m_aStack.Push(m_pRootObject);

        int iCurrent = 1;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            m_bInsideArray = m_aStack.Head().Class() == EObjectClass::Array;

            iRet = this->ProcessChar(iCurrent);
            if(iRet != ETextError::Ok) { return iRet; }
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

    CObject CTextParser::Root() {
        return m_pRootObject;
    }

    CObject CTextParser::operator[](const char* sPath) {
        if(m_pRootObject == cdf::NIL) { return cdf::NIL; }
        return m_pRootObject.Access(sPath);
    }

    CTextParser& CTextParser::operator=(const CTextParser& Other) {
        if(m_pRootObject != cdf::NIL) {
            DeleteObject(m_pRootObject);
        }

        m_pRootObject = CopyObject(Other.m_pRootObject);

        return *this;
    }

    CTextParser::~CTextParser() {
        DeleteObject(m_pRootObject);
    }
}
