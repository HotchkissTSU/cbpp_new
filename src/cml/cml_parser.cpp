#include "cbpp/CML.h"

#include "cbpp/Filesystem.h"

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
            case EErrorType::BadFileRef:        return "Undefined file reference";
            //case EErrorType::StrayKeyword:      return "Stray keyword";
            //case EErrorType::BadConstRef:       return "Undefined constant reference";
            //case EErrorType::BadConstType:      return "Bad constant type";
            case EErrorType::IncludeNonString:  return "'include' followed by a non-string value";
            case EErrorType::BadQualifier:      return "Bad qualifier";
            case EErrorType::StackOverflow:     return "Stack overflow";

            default:                            return "(null)";
        }
    }

    CParser::CParser() {
        m_aStack.Reserve(CBPP_CML_MAX_DEPTH);
    }
    
    bool CParser::ParseString(const char* sCode, bool bAllowInclude) {
        m_aArrBraceStack.Clear();
        m_aObjBraceStack.Clear();

        m_aStack.Clear();
        m_bExpectObject = false;

        m_aStack.Push(m_pRoot);

        m_sSource = sCode;

        CTokenizer Lexer(m_sSource);
        Lexer.ProcessString();

        CArray<Token>& aTokens = Lexer.GetTokens();
        for(size_t i = 0; i < aTokens.Length(); i++) {
            EErrorType iRet = ProcessToken(aTokens[i]);

            if(iRet != EErrorType::Ok) {
                if(aTokens.Length() > 1 && iRet != EErrorType::Redefinition) {
                    m_ErroredToken = aTokens[i-1];
                }else{
                    m_ErroredToken = aTokens[i];
                }

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
    }
    
    IObject* CParser::CreateRefObject(Token& Data) {
        char* sBuffer = LexemBuffer();
        Data.sLexeme.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE);

        switch(Data.iRef) {
            case EQualifier::FileTextRef: {
                IObject* pStringObj = CreateObject(EValueType::String);

                IFile* hFile = OpenFile(sBuffer, "rb");
                if(hFile != NULL) {
                    size_t iLen = hFile->Length();

                    pStringObj->Value()->m_Value.str = Malloc<char>(iLen+1);
                    pStringObj->Value()->m_Value.str[iLen] = '\0';
                    hFile->ReadAll(pStringObj->Value()->m_Value.str);
                } else {
                    Delete(pStringObj);
                    return NULL;
                }

                return pStringObj;
            }
            
            case EQualifier::FileBinRef: {
                IObject* pBinaryObj = CreateObject(EValueType::Binary);

                IFile* hFile = OpenFile(sBuffer, "rb");

                if(hFile != NULL) {
                    size_t iLen = hFile->Length();

                    pBinaryObj->Value()->m_Value.str = Malloc<char>(iLen);
                    hFile->ReadAll(pBinaryObj->Value()->m_Value.str);
                    pBinaryObj->Value()->m_iLength = iLen;
                } else {
                    Delete(pBinaryObj);
                    return NULL;
                }

                return pBinaryObj;
            }

            default: return NULL;
        }
    }

    char* CParser::LexemBuffer() const {
        static char s_sBuffer[CBPP_CML_LEXEM_BUFFER_SIZE];
        return s_sBuffer;
    }

    EErrorType CParser::ProcessToken(Token& Data) {
        /*if(Data.iType != EToken::Identifier && Data.iRef == EQualifier::ConstDecl) {
            return EErrorType::BadQualifier;
        }

        if(Data.iType == EToken::Number && Data.iRef != EQualifier::None) {
            return EErrorType::BadQualifier;
        }

        if(Data.iType == EToken::String && Data.iRef == EQualifier::ConstRef) {
            return EErrorType::BadQualifier;
        }*/
    
        procfunc_t fpFunc = m_aTokenProcessors[(int32_t)(Data.iType)];

        return (*this.*fpFunc)(Data);
    }
    
    size_t CParser::GetErrorLog(char* sBuffer, size_t iMaxSize) const {
        char* sBuff = LexemBuffer();

        m_ErroredToken.sLexeme.Bufferize(sBuff, CBPP_CML_LEXEM_BUFFER_SIZE);

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

        const decltype(m_dConstants)::pairs_t& Pairs = m_dConstants.Data();
        for(size_t i = 0; i < Pairs.Length(); i++) {
            Delete(Pairs[i].Value);
        }
    }
}
