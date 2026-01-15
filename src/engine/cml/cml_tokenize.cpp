#include "cbpp/CML.h"

#include <ctype.h>

#include "engine/string_utils.h"

namespace cbpp::cml {
    const char* GetTokenName(EToken iType) {
        switch (iType) {            
            case EToken::BlockOpen:
                return "BLK_START";

            case EToken::BlockClose:
                return "BLK_END";

            case EToken::ArrayOpen:
                return "ARR_START";

            case EToken::ArrayClose:
                return "ARR_END";

            case EToken::Identifier:
                return "IDENTIFIER";

            case EToken::Number:
                return "NUMBER";

            case EToken::String:
                return "STRING";

            case EToken::Keyword:
                return "KEYWORD";

            default:
                return "(null)";
        }
    }

    CTokenizer::CTokenizer(const CString& sSource) : m_sSource(sSource) {}

    void CTokenizer::ProcessLastChar(char cChar) {        
        if(cChar == ']') {
            m_iLexemStart = m_iCounter;

            m_aTokens.PushBack({
                EToken::ArrayClose,
                CSubString(m_sSource, m_iLexemStart, 1),
                m_iLine, m_iCol
            });
        }

        else if(cChar == '}') {
            m_iLexemStart = m_iCounter;

            m_aTokens.PushBack({
                EToken::BlockClose,
                CSubString(m_sSource, m_iLexemStart, 1),
                m_iLine, m_iCol
            });
        }

        else if(cChar == ')') {
            m_iLexemStart = m_iCounter;

            m_aTokens.PushBack({
                EToken::StructClose,
                CSubString(m_sSource, m_iLexemStart, 1),
                m_iLine, m_iCol
            });
        }
    }

    void CTokenizer::ProcessCharacter(char cChar) {
        if(cChar == '\n') {
            m_iCol = 1;
            m_iLine++;
        }else{
            m_iCol++;
        }

        switch(m_iState) {
            case ETokenizerState::Start:
                if(isdigit(cChar) || cChar == '.') {
                    m_iLexemStart = m_iCounter;
                    m_iLexemLength = 1;

                    m_iState = ETokenizerState::InNumber;
                }

                else if(isalpha(cChar) || cChar == '_') {
                    m_iLexemStart = m_iCounter;
                    m_iLexemLength = 1;

                    m_iState = ETokenizerState::InIdentifier;
                }

                else if(cChar == '"') {
                    m_iLexemStart = m_iCounter + 1;
                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::InString;
                }

                else if(cChar == '#') {
                    m_iState = ETokenizerState::InComment;
                }

                else if(cChar == '(') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::StructOpen,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                }

                else if(cChar == '[') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::ArrayOpen,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                }

                else if(cChar == ']') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::ArrayClose,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                }

                else if(cChar == '{') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::BlockOpen,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                }

                else if(cChar == '}') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::BlockClose,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                }

                // Qualifiers
                else if(cChar == '@') {
                    m_iRefType = EQualifier::FileTextRef;
                }

                else if(cChar == '&') {
                    m_iRefType = EQualifier::FileBinRef;
                }

                else if(cChar == ':') {
                    m_iRefType = EQualifier::ConstDecl;
                }

                else if(cChar == '$') {
                    m_iRefType = EQualifier::ConstRef;
                }
                break;

            case ETokenizerState::InComment:
                if(cChar == '\n' || cChar == '#') {
                    m_iState = ETokenizerState::Start;
                }
                break;

            case ETokenizerState::InNumber:
                if (isdigit(cChar) || cChar == '.') {
                    m_iLexemLength++;
                } else {
                    m_aTokens.PushBack({
                        EToken::Number,
                        CSubString(m_sSource, m_iLexemStart, m_iLexemLength),
                        m_iLine, m_iCol,
                        m_iRefType
                    });

                    m_iRefType = EQualifier::None;

                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::Start;
                    ProcessLastChar(cChar);
                }
                break;

            case ETokenizerState::InIdentifier:
                if(isalnum(cChar) || cChar == '_' || cChar == '.') {
                    m_iLexemLength++;
                } else {
                    char sBuff[CBPP_CML_LEXEM_BUFFER_SIZE];
                    CSubString sLexeme(m_sSource, m_iLexemStart, m_iLexemLength);
                    sLexeme.Bufferize(sBuff, sizeof(sBuff));

                    EToken iType = IsKeyword(sBuff) ? EToken::Keyword : EToken::Identifier;
                    
                    m_aTokens.PushBack({
                        iType,
                        sLexeme,
                        m_iLine, m_iCol,
                        m_iRefType
                    });

                    m_iRefType = EQualifier::None;

                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::Start;
                    ProcessLastChar(cChar);
                }
                break;
                
            case ETokenizerState::InString:
                if(cChar == '"') {
                    m_aTokens.PushBack({
                        EToken::String,
                        CSubString(m_sSource, m_iLexemStart, m_iLexemLength),
                        m_iLine, m_iCol,
                        m_iRefType
                    });

                    m_iRefType = EQualifier::None;

                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::Start;
                    //ProcessLastChar(cChar);
                } else { 
                    m_iLexemLength++;
                }
                break;
        }

        m_iCounter++;
    }

    void CTokenizer::Finish() {
        ProcessCharacter('#');
        m_aTokens.Shrink();
    }

    CArray<Token>& CTokenizer::GetTokens() { return m_aTokens; }

    void CTokenizer::ProcessString() {
        char* C = (char*)(const char*)m_sSource; // >~<
        while(*C != '\0') {
            ProcessCharacter(*C);
            C++;
        }
        Finish();
    }

    void CTokenizer::Print() {
        static char sBuffer[CBPP_CML_MAX_LEXEM_LENGTH+1];
        for(size_t i = 0; i < m_aTokens.Length(); i++) {
            Token& Current = m_aTokens[i];

            Current.sLexeme.Bufferize(sBuffer, sizeof(sBuffer));
            printf("%s = '%s'\n", GetTokenName(Current.iType), sBuffer);
        }
    }
}
