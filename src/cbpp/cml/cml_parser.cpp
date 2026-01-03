#include "cbpp/cml/cml.h"

namespace cbpp::cml {
    bool CheckBraceMatch(EToken iOpener, EToken iCloser) {
        switch(iOpener) {
            case EToken::ArrayOpen: return iCloser == EToken::ArrayClose;
            case EToken::BlockOpen: return iCloser == EToken::BlockClose;
        }
        return false;
    }

    const char* GetErrorName(EErrorType iType) {
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

            default:                            return "(null)";
        }
    }

    bool CParser::ParseString(const char* sCode, bool bAllowInclude) {
        m_aStack.Push(m_pRoot);

        m_sSource = sCode;

        CTokenizer Lexer(m_sSource);
        Lexer.ProcessString();

        CArray<Token>& aTokens = Lexer.GetTokens();
        for(size_t i = 0; i < aTokens.Length(); i++) {
            EErrorType iRet = ProcessToken(aTokens[i]);

            if(iRet != EErrorType::Ok) {
                m_ErroredToken = aTokens[i];
                m_iLastError = iRet;
                Reset();
                return false;
            }
        }

        return true;
    }
    
    void CParser::Reset() {
        Delete(m_pRoot);
        m_pRoot = CreateObject(EValueType::Object);

        m_aBracesStack.Clear();
        m_aStack.Clear();
        m_bExpectObject = false;
    }

    EErrorType CParser::ProcessToken(Token& Data) {
        char sBuffer[128];

        switch (Data.iType) {
            case EToken::Identifier:
                if(m_bExpectObject) { // Stray identifier
                    return EErrorType::StrayIdentifier;
                }

                m_bExpectObject = true;
                m_sCurrentIdentifier = Data.sLexeme;
                break;

            case EToken::Number: {
                IObject* pCurrent = m_aStack.Head();

                if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
                    return EErrorType::StrayNumber;
                }

                IObject* pNumberObj;

                Data.sLexeme.Bufferize(sBuffer, sizeof(sBuffer));
                char* pDot = strchr(sBuffer, '.');

                if(pDot == NULL) {
                    pNumberObj = CreateObject(EValueType::Integer);
                    pNumberObj->Value()->SetValue(atoi(sBuffer));
                } else {
                    pNumberObj = CreateObject(EValueType::Float);
                    pNumberObj->Value()->SetValue((float)atof(sBuffer));
                }

                m_sCurrentIdentifier.Bufferize(sBuffer, sizeof(sBuffer));
                pCurrent->PushChild(sBuffer, pNumberObj);

                m_bExpectObject = false;

                break;
            }
            
            case EToken::String: {
                IObject* pCurrent = m_aStack.Head();

                if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
                    return EErrorType::StrayString;
                }

                IObject* pStringObj = CreateObject(EValueType::String);
                Data.sLexeme.Bufferize(sBuffer, sizeof(sBuffer));

                pStringObj->Value()->SetValue(sBuffer);

                m_sCurrentIdentifier.Bufferize(sBuffer, sizeof(sBuffer));
                pCurrent->PushChild(sBuffer, pStringObj);

                m_bExpectObject = false;

                break;
            }
            
            case EToken::BlockOpen: {
                IObject* pCurrent = m_aStack.Head();

                if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
                    return EErrorType::StrayBlock;
                }

                IObject* pObj = CreateObject(EValueType::Object);
                m_sCurrentIdentifier.Bufferize(sBuffer, sizeof(sBuffer));
                pCurrent->PushChild(sBuffer, pObj);
                m_aStack.Push(pObj);

                m_bExpectObject = false;

                m_aBracesStack.Push(EToken::BlockOpen);

                break;
            }
            
            case EToken::BlockClose: {
                if(m_aStack.Length() > 1) {
                    m_aStack.Pop();
                } else { // Attempt to pop root block
                    return EErrorType::IllBlock;
                }

                EToken iTopBrace = m_aBracesStack.Head();
                if(!CheckBraceMatch(iTopBrace, Data.iType)) {   // brace stack mismatch
                    return EErrorType::IllBlock;
                } else if(m_aBracesStack.Length() > 0) {      
                    m_aBracesStack.Pop();                       // all ok
                } else {
                    return EErrorType::IllBlock;                // no opener, stack empty
                }

                break;
            }
            
            case EToken::ArrayOpen: {
                IObject* pCurrent = m_aStack.Head();

                if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
                    return EErrorType::StrayArray; // sounds like a sick band title
                }

                IObject* pArr = CreateObject(EValueType::Array);
                m_sCurrentIdentifier.Bufferize(sBuffer, sizeof(sBuffer));
                pCurrent->PushChild(sBuffer, pArr);
                m_aStack.Push(pArr);

                m_aBracesStack.Push(EToken::ArrayOpen);

                break;
            }
            
            case EToken::ArrayClose: {
                if(m_aStack.Length() > 1) {
                    m_aStack.Pop();
                }else{
                    return EErrorType::IllArray;
                }

                EToken iTopBrace = m_aBracesStack.Head();
                if(!CheckBraceMatch(iTopBrace, Data.iType)) {   // brace stack mismatch
                    return EErrorType::IllArray;
                } else if(m_aBracesStack.Length() > 0) {      
                    m_aBracesStack.Pop();                       // all ok
                } else {
                    return EErrorType::IllArray;                // no opener, stack empty
                }
            }
        }

        return EErrorType::Ok;
    }

    size_t CParser::GetErrorLog(char* sBuffer, size_t iMaxSize) const {
        char sBuff[128];

        m_ErroredToken.sLexeme.Bufferize(sBuff, sizeof(sBuff));

        return snprintf(sBuffer, iMaxSize, "Error: %s, Line %i, Column %i (near '%s')", 
                        GetErrorName(m_iLastError), 
                        m_ErroredToken.iLine, m_ErroredToken.iColumn,
                        sBuff);
    }

    IObject* CParser::Root() {
        return m_pRoot;
    }

    CParser::~CParser() {
        if(m_pRoot != NULL) {
            Delete(m_pRoot);
        }
    }
}
