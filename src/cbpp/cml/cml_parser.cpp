#include "cbpp/cml/cml.h"

namespace cbpp::cml {
    bool CParser::ParseString(const char* sCode, bool bAllowInclude) {
        m_aStack.Push(m_pRoot);

        m_sSource = sCode;

        CTokenizer Lexer(m_sSource);
        Lexer.ProcessString();

        CArray<Token>& aTokens = Lexer.GetTokens();
        for(size_t i = 0; i < aTokens.Length(); i++) {
            ProcessToken(aTokens[i]);
        }

        return true;
    }

    void CParser::ProcessToken(Token& Data) {
        char sBuffer[128];

        switch (Data.iType) {
            case EToken::Identifier:
                if(m_bExpectObject) { // Stray identifier
                    return;
                }

                m_bExpectObject = true;
                m_sCurrentIdentifier = Data.sLexeme;
                break;

            case EToken::Number: {
                if(!m_bExpectObject) { // Stray number
                    return;
                }

                IObject* pCurrent = m_aStack.Head();
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
                if(!m_bExpectObject) { // Stray string
                    return;
                }

                IObject* pCurrent = m_aStack.Head();
                IObject* pStringObj = CreateObject(EValueType::String);
                Data.sLexeme.Bufferize(sBuffer, sizeof(sBuffer));

                pStringObj->Value()->SetValue(sBuffer);

                m_sCurrentIdentifier.Bufferize(sBuffer, sizeof(sBuffer));
                pCurrent->PushChild(sBuffer, pStringObj);

                m_bExpectObject = false;

                break;
            }
            
            case EToken::BlockOpen: {
                if(!m_bExpectObject) { // Stray block
                    return;
                }

                IObject* pObj = CreateObject(EValueType::Object);
                m_sCurrentIdentifier.Bufferize(sBuffer, sizeof(sBuffer));
                m_aStack.Head()->PushChild(sBuffer, pObj);
                m_aStack.Push(pObj);

                m_bExpectObject = false;

                break;
            }

            case EToken::BlockClose: {
                if(m_aStack.Length() > 1) {
                    m_aStack.Pop();
                } else { // Attempt to pop root block
                    return;
                }
                break;
            }
        }
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
