#include "cbpp/CML.h"

#include <ctype.h>
#include <stdio.h>

#define CHECK_REDEF                                                                                     \
    CObject pHead = aStack.Head();                                                                      \
    if(!bInsideArray && pHead[m_sCurrentName.String()] != cml::NIL) {                                   \
        return EErrorType::Redefinition;                                                                \
    }

namespace cbpp::cml {
    const char* StringError(EErrorType iType) {
        switch(iType) {
            case EErrorType::Ok:                return "Ok";
            case EErrorType::IllBlock:          return "Bad block";
            case EErrorType::IllArray:          return "Bad array";
            case EErrorType::Redefinition:      return "Redifinition in the local scope";
            case EErrorType::StrayBlock:        return "Stray block";
            case EErrorType::StrayArray:        return "Stray array";
            case EErrorType::StrayIdentifier:   return "Stray identifier";
            case EErrorType::StrayNumber:       return "Stray number";
            case EErrorType::StrayString:       return "Stray string";
            case EErrorType::BadFileRef:        return "Undefined file reference";
            case EErrorType::BadNumber:         return "Not a number";
            case EErrorType::StackOverflow:     return "Stack overflow";
            case EErrorType::StackUnderflow:    return "Stack underflow";
            case EErrorType::NoFile:            return "File not found";
            case EErrorType::UnexpectedEOF:     return "Unexpected EOF";
            case EErrorType::VersionMismatch:   return "Source version is incompatible";
            case EErrorType::BadReference:      return "Non-string value is referenced";
            case EErrorType::BadVersion:        return "Bad version, must be a positive integer";

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

    CParser::IncludeNode::~IncludeNode() {
        if(pFile != NULL) {
            CloseFile(pFile);
            pFile = NULL;
        }
    }

    bool CParser::IsValidNameStart(int iChar) {
        return isalpha(iChar) || (iChar == '_');
    }

    EKeyword CParser::IsKeyword(const char* sName) {
        if( strcmp(sName, "include") == 0 ) {
            return EKeyword::Include;
        } else if( strcmp(sName, "version") == 0 ) {
            return EKeyword::Version;
        } else {
            return EKeyword::Name;
        }
    }

    EErrorType CParser::AddFile(const char* sPath) {
        cbpp::IFile* pFile = cbpp::OpenFile(sPath, "rb");
        if(pFile == NULL) {
            return EErrorType::NoFile;
        }

        m_aFilesStack.Push( {sPath, pFile} );

        return EErrorType::Ok;
    }

    int CParser::Peek() {
        cbpp::IFile* pFile = m_aFilesStack.Head().pFile;

        int iChar = pFile->GetChar();

        if(iChar == EOF) {      // this file is over, step out and resume      
            m_aFilesStack.Pop();  

            if(m_aFilesStack.Length() > 0) {
                return Peek();          // peek again
            } else {
                return 0;
            }
        }
        
        m_iCol++;
        if(iChar == '\n') {
            m_iCol = 0;
            m_iLine++;
        }

        return iChar;
    }

    EErrorType CParser::ParseName(int iChar) {
        m_sLexemBuffer.Clear();

        m_sLexemBuffer.PushBack(iChar);

        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if( isalnum(iCurrent) || (iCurrent == '_') ) {
                m_sLexemBuffer.PushBack(iCurrent);
            } else {    // name ends
                m_sLexemBuffer.PushBack('\0');
                return EErrorType::Ok;
            }
        }

        return EErrorType::UnexpectedEOF;
    }

    EErrorType CParser::ParseString(int iChar) {
        m_sLexemBuffer.Clear();

        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();
            if(iCurrent == '"' || iCurrent == '\'') {
                m_sLexemBuffer.PushBack('\0');
                return EErrorType::Ok;
            }

            m_sLexemBuffer.PushBack(iCurrent);
        }

        return EErrorType::UnexpectedEOF;
    }
    
    EErrorType CParser::ParseNumber(int iChar) {
        m_sLexemBuffer.Clear();
        m_sLexemBuffer.PushBack(iChar);

        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if(isdigit(iCurrent) || (iCurrent == '.') || (iCurrent == 'e') || (iCurrent == 'E') || (iCurrent == '-')) {
                m_sLexemBuffer.PushBack(iCurrent);
            } else {
                m_sLexemBuffer.PushBack('\0');
                return EErrorType::Ok;
            }
        }

        return EErrorType::UnexpectedEOF;
    }

    EErrorType CParser::Parse(const char* sPath) {
        EErrorType iRet = AddFile(sPath);

        if(iRet != EErrorType::Ok) { return iRet; }

        m_pRootObject = CreateObject(EObjectClass::Object);

        cbpp::CStack<CObject> aStack;

        aStack.Push(m_pRootObject);

        bool bInsideArray = false;

        int iCurrent;
        while(iCurrent != 0) {
            iCurrent = this->Peek();

            if(this->IsValidNameStart(iCurrent)) {                  // NAME
                if(m_bExpectValue || bInsideArray) {
                    return EErrorType::StrayIdentifier;
                }

                EErrorType iRet = ParseName(iCurrent);
                if(iRet != EErrorType::Ok) { return iRet; }

                EKeyword iKW = IsKeyword(m_sLexemBuffer.Data());

                switch (iKW) {
                    case EKeyword::Name: {
                        m_sCurrentName.Set( m_sLexemBuffer.Data() );
                        m_bExpectValue = true;
                        break;
                    }

                    case EKeyword::Version: {
                        m_bExpectVersion = true;
                        break;
                    }

                    case EKeyword::Include: {
                        m_bExpectInclude = true;
                        break;
                    }
                }
                
            } else if(iCurrent == '"' || iCurrent == '\'') {        // STRING
                if(!m_bExpectValue && !bInsideArray && !m_bExpectInclude) {
                    return EErrorType::StrayString;
                }

                CHECK_REDEF

                EErrorType iRet = this->ParseString(iCurrent);
                if(iRet != EErrorType::Ok) { return iRet; }

                if(m_bExpectInclude) {
                    iRet = AddFile(m_sLexemBuffer.Data());
                    if(iRet != EErrorType::Ok) {
                        return iRet;
                    }

                    m_bExpectInclude = false;

                } else {
                    CObject pStringObj = CreateObject(EObjectClass::String);
                    pStringObj = m_sLexemBuffer.Data();

                    if(!bInsideArray) {
                        pHead.Push(m_sCurrentName.String(), pStringObj);
                    } else {
                        pHead.Push(pStringObj);
                    }

                    m_bExpectValue = false;
                }

            } else if(isdigit(iCurrent) || (iCurrent == '-') || (iCurrent == '.')) {    // NUMBER
                if(!m_bExpectValue && !bInsideArray && !m_bExpectVersion) {
                    return EErrorType::StrayNumber;
                }

                CHECK_REDEF

                EErrorType iRet = this->ParseNumber(iCurrent);
                if(iRet != EErrorType::Ok) { return iRet; }

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
                        return EErrorType::BadNumber;
                    }
                }
                
                if(m_bExpectVersion) {
                    if(iNumberTest != 0 || iData <= 0) {
                        return EErrorType::BadVersion;
                    }

                    if(iData > CBPP_CML_VERSION || iData < CBPP_CML_VERSION_LEAST) {
                        return EErrorType::VersionMismatch;
                    }

                    m_bExpectVersion = false;

                } else {
                    CObject pObj = CreateObject(iClass);

                    if(iNumberTest == 1) {
                        pObj = fData;
                    } else {
                        pObj = iData;
                    }

                    if(!bInsideArray) {
                        pHead.Push(m_sCurrentName.String(), pObj);
                    } else {
                        pHead.Push(pObj);
                    }

                    m_bExpectValue = false;
                }

            } else if(iCurrent == '{') {                    // BLOCK OPENING
                if(!m_bExpectValue && !bInsideArray) {
                    return EErrorType::StrayBlock;
                }

                if(aStack.Length() == CBPP_CML_STACK_LIMIT) {
                    return EErrorType::StackOverflow;
                }
                
                CHECK_REDEF
                
                CObject pDict = CreateObject(EObjectClass::Object);

                if(!bInsideArray) {
                    pHead.Push(m_sCurrentName.String(), pDict);
                } else {
                    pHead.Push(pDict);
                }

                aStack.Push(pDict);
                
                m_bExpectValue = false;

            } else if(iCurrent == '}') {                    // BLOCK CLOSING
                if(aStack.Length() <= 1) {
                    return EErrorType::StackUnderflow;
                }

                aStack.Pop();

            } else if(iCurrent == '[') {                    // ARRAY OPENING
                if(!m_bExpectValue && !bInsideArray) {
                    return EErrorType::StrayArray;
                }

                if(aStack.Length() == CBPP_CML_STACK_LIMIT) {
                    return EErrorType::StackOverflow;
                }

                CHECK_REDEF

                CObject pArr = CreateObject(EObjectClass::Array);
        
                if(!bInsideArray) {
                    pHead.Push(m_sCurrentName.String(), pArr);
                } else {
                    pHead.Push(pArr);
                }

                aStack.Push(pArr);

                m_bExpectValue = false;

            } else if(iCurrent == ']') {                    // ARRAY CLOSING
                if(aStack.Length() <= 1) {
                    return EErrorType::StackUnderflow;
                }

                aStack.Pop();
            }

            bInsideArray = aStack.Head().Class() == EObjectClass::Array;
        }
        
        return EErrorType::Ok;
    }

    CObject CParser::Root() {
        return m_pRootObject;
    }

    CParser::~CParser() {
        DeleteObject(m_pRootObject);
    }
}
