#include "cbpp/CML.h"

namespace cbpp::cml {
    EErrorType CParser::ProcessIdentifier(Token& Data) {
        char* sBuffer = LexemBuffer();

        if(m_bExpectObject) {
            return EErrorType::StrayIdentifier;
        }

        IObject* pCurrent = m_aStack.Head();

        Data.sLexeme.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE);
        if(pCurrent->HasChild(sBuffer)) {
            return EErrorType::Redefinition;
        }
        
        m_bExpectObject = true;
        m_sCurrentIdentifier = Data.sLexeme;

        return EErrorType::Ok;
    }

    EErrorType CParser::ProcessNumber(Token& Data) {
        char* sBuffer = LexemBuffer();

        IObject* pNumberObj;

        Data.sLexeme.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE);
        
        int iIntType = IsNumber(sBuffer);

        switch(iIntType) {
            case 1: {
                pNumberObj = CreateObject(EValueType::Float);
                pNumberObj->Value()->SetValue( strtof(sBuffer, NULL) );
                break;
            }

            case 0: {
                pNumberObj = CreateObject(EValueType::Integer);
                pNumberObj->Value()->SetValue( atoi(sBuffer) );
                break;
            }

            case -1: {
                return EErrorType::BadNumber;
            }
        }
        
        IObject* pCurrent = m_aStack.Head();

        if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
            return EErrorType::StrayNumber;
        }

        m_sCurrentIdentifier.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE);
        pCurrent->PushChild(sBuffer, pNumberObj);
        
        m_bExpectObject = false;

        return EErrorType::Ok;
    }

    EErrorType CParser::ProcessString(Token& Data) {
        char* sBuffer = LexemBuffer();

        IObject* pStringObj;
        Data.sLexeme.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE); // string data

        if(Data.iRef == EQualifier::FileBinRef || Data.iRef == EQualifier::FileTextRef) {
            pStringObj = CreateRefObject(Data);
            if(pStringObj == NULL) {
                return EErrorType::BadFileRef;
            }
        } else {
            pStringObj = CreateObject(EValueType::String);
            pStringObj->Value()->SetValue(sBuffer);
        }

        IObject* pCurrent = m_aStack.Head();

        if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
            return EErrorType::StrayString;
        }
        
        m_sCurrentIdentifier.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE); // string name
        pCurrent->PushChild(sBuffer, pStringObj);
        
        m_bExpectObject = false;

        return EErrorType::Ok;
    }

    EErrorType CParser::ProcessBlock(Token& Data) {
        char* sBuffer = LexemBuffer();

        switch(Data.iType) {
            case EToken::BlockOpen: {
                if(m_aStack.Length() >= CBPP_CML_MAX_DEPTH) {
                    return EErrorType::StackOverflow;
                }

                IObject* pCurrent = m_aStack.Head();

                if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
                    return EErrorType::StrayBlock;
                }

                IObject* pObj = CreateObject(EValueType::Object);
                m_sCurrentIdentifier.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE);
                pCurrent->PushChild(sBuffer, pObj);
                m_aStack.Push(pObj);

                m_bExpectObject = false;

                break;
            }

            case EToken::BlockClose: {
                if(m_aStack.Length() >= 1) {
                    m_aStack.Pop();
                } else { // Attempt to pop root block
                    return EErrorType::IllBlock;
                }

                break;
            }
        }

        return EErrorType::Ok;
    }
    
    EErrorType CParser::ProcessArray(Token& Data) {
        char* sBuffer = LexemBuffer();

        switch(Data.iType) {
            case EToken::ArrayOpen: {
                if(m_aStack.Length() >= CBPP_CML_MAX_DEPTH) {
                    return EErrorType::StackOverflow;
                }

                IObject* pCurrent = m_aStack.Head();

                if(pCurrent->Type() == EValueType::Object && !m_bExpectObject) {
                    return EErrorType::StrayArray; // sounds like a sick band title
                }

                IObject* pArr = CreateObject(EValueType::Array);
                m_sCurrentIdentifier.Bufferize(sBuffer, CBPP_CML_LEXEM_BUFFER_SIZE);
                pCurrent->PushChild(sBuffer, pArr);
                m_aStack.Push(pArr);

                break;
            }

            case EToken::ArrayClose: {
                if(m_aStack.Length() >= 1) {
                    m_aStack.Pop();
                }else{
                    return EErrorType::IllArray;
                }

                break;
            }
        }

        return EErrorType::Ok;
    }

    EErrorType CParser::ProcessKeyword(Token& Data) {
        /*char* sBuffer = LexemBuffer();

        if(m_bExpectObject) {
            return EErrorType::StrayKeyword;
        }

        Data.sLexeme.Bufferize(sBuffer, sizeof(sBuffer));

        if(strcmp(sBuffer, "include") == 0) {
            //m_bIncluding = true;
        }*/

        return EErrorType::Ok;
    }

    EErrorType CParser::ProcessStruct(Token& Data) {
        return EErrorType::IllBlock;
    }
}
