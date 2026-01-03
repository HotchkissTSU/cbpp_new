#include "cbpp/cml/cml.h"

#include <ctype.h>

#include "cbpp/string_utils.h"

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

    void CTokenizer::ProcessCharacter(char cChar) {
        switch(m_iState) {
            case ETokenizerState::Start:
                if (isspace(cChar)) {
                    if(cChar == '\n') { 
                        m_iLine++; 
                        m_iCol = 1;
                    } else { 
                        m_iCol++;
                    }
                }
                
                else if(isdigit(cChar) || cChar == '.') {
                    m_iLexemStart = m_iCounter;
                    m_iLexemLength = 1;

                    m_iState = ETokenizerState::InNumber;
                    m_iCol++;
                }

                else if(isalpha(cChar) || cChar == '_') {
                    m_iLexemStart = m_iCounter;
                    m_iLexemLength = 1;

                    m_iState = ETokenizerState::InIdentifier;
                    m_iCol++;
                }

                else if(cChar == '"') {
                    m_iLexemStart = m_iCounter + 1;
                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::InString;
                    m_iCol++;
                }

                else if(cChar == '#') {
                    m_iState = ETokenizerState::InComment;
                    m_iCol++;
                }
                
                else if(cChar == '{') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::BlockOpen,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == '}') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::BlockClose,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == '[') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::ArrayOpen,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == ']') {
                    m_iLexemStart = m_iCounter;

                    m_aTokens.PushBack({
                        EToken::ArrayClose,
                        CSubString(m_sSource, m_iLexemStart, 1),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
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
                    m_iCol++;
                } else {

                    m_aTokens.PushBack({
                        EToken::Number,
                        CSubString(m_sSource, m_iLexemStart, m_iLexemLength),
                        m_iLine, m_iCol
                    });

                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::Start;
                }
                break;

            case ETokenizerState::InIdentifier:
                if(isalnum(cChar) || cChar == '_') {
                    m_iLexemLength++;
                    m_iCol++;
                } else {
                    char sBuff[128];
                    CSubString sLexeme(m_sSource, m_iLexemStart, m_iLexemLength);
                    sLexeme.Bufferize(sBuff, sizeof(sBuff));

                    EToken iType = IsKeyword(sBuff) ? EToken::Keyword : EToken::Identifier;

                    m_aTokens.PushBack({
                        iType,
                        sLexeme,
                        m_iLine, m_iCol
                    });

                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::Start;
                }
                break;
                
            case ETokenizerState::InString:
                if(cChar == '"') {
                    m_aTokens.PushBack({
                        EToken::String,
                        CSubString(m_sSource, m_iLexemStart, m_iLexemLength),
                        m_iLine, m_iCol
                    });

                    m_iLexemLength = 0;
                    m_iState = ETokenizerState::Start;
                } else { 
                    m_iLexemLength++;
                }
                m_iCol++;
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
        char* C = (char*)(const char*)m_sSource;
        while(*C != '\0') {
            ProcessCharacter(*C);
            C++;
        }
        Finish();
    }

    void CTokenizer::Print() {
        static char sBuffer[128];
        for(size_t i = 0; i < m_aTokens.Length(); i++) {
            Token& Current = m_aTokens[i];

            Current.sLexeme.Bufferize(sBuffer, sizeof(sBuffer));
            printf("%s = '%s'\n", GetTokenName(Current.iType), sBuffer);
        }
    }
}
