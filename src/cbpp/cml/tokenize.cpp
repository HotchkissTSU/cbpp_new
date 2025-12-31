#include "cbpp/cml/cml.h"

#include <ctype.h>

namespace cbpp::cml {
    const char* GetTokenName(EToken iType) {
        switch (iType) {
            case EToken::Assign:
                return "ASSIGN";
            
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
                    m_iState = ETokenizerState::InNumber;
                    m_aCurrentLexeme.PushBack(cChar);
                    m_iCol++;
                }

                else if(isalpha(cChar) || cChar == '_') {
                    m_iState = ETokenizerState::InIdentifier;
                    m_aCurrentLexeme.PushBack(cChar);
                    m_iCol++;
                }

                else if(cChar == '"') {
                    m_iState = ETokenizerState::InString;
                    m_iCol++;
                }

                else if(cChar == '#') {
                    m_iState = ETokenizerState::InComment;
                    m_iCol++;
                }
                
                else if(cChar == '{') {
                    m_aTokens.PushBack({
                        EToken::BlockOpen,
                        CString(),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == '}') {
                    m_aTokens.PushBack({
                        EToken::BlockClose,
                        CString(),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == '[') {
                    m_aTokens.PushBack({
                        EToken::ArrayOpen,
                        CString(),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == ']') {
                    m_aTokens.PushBack({
                        EToken::ArrayClose,
                        CString(),
                        m_iLine, m_iCol
                    });
                    m_iCol++;
                }

                else if(cChar == '=') {
                    m_aTokens.PushBack({
                        EToken::Assign,
                        CString(),
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
                    m_aCurrentLexeme.PushBack(cChar);
                    m_iCol++;
                } else {
                    m_aCurrentLexeme.PushBack('\0');

                    m_aTokens.PushBack({
                        EToken::Number,
                        CString(m_aCurrentLexeme.Data()),
                        m_iLine, m_iCol
                    });

                    m_aCurrentLexeme.Clear();
                    m_iState = ETokenizerState::Start;
                }
                break;

            case ETokenizerState::InIdentifier:
                if(isalnum(cChar) || cChar == '_') {
                    m_aCurrentLexeme.PushBack(cChar);
                    m_iCol++;
                } else {
                    m_aCurrentLexeme.PushBack('\0');
                    EToken iType = IsKeyword(m_aCurrentLexeme.Data()) ? EToken::Keyword : EToken::Identifier;

                    m_aTokens.PushBack({
                        iType,
                        CString(m_aCurrentLexeme.Data()),
                        m_iLine, m_iCol
                    });

                    m_aCurrentLexeme.Clear();
                    m_iState = ETokenizerState::Start;
                }
                break;

            case ETokenizerState::InString:
                if(cChar == '"') {
                    m_aCurrentLexeme.PushBack('\0');

                    m_aTokens.PushBack({
                        EToken::String,
                        CString(m_aCurrentLexeme.Data()),
                        m_iLine, m_iCol
                    });

                    m_aCurrentLexeme.Clear();
                    m_iState = ETokenizerState::Start;
                } else { 
                    if(m_bHasBSlash) {
                        if(cChar == 'n') {
                            m_aCurrentLexeme.PushBack('\n');
                        }else if(cChar == 't') {
                            m_aCurrentLexeme.PushBack('\t');
                        }
                        m_bHasBSlash = false;
                    } else {
                        if(cChar == '\\') {
                            m_bHasBSlash = true;
                        }else if(cChar != '\n'){
                            m_aCurrentLexeme.PushBack(cChar);
                        }
                    }
                }
                m_iCol++;
                break;
        }
    }
    
    void CTokenizer::Finish() {
        ProcessCharacter('#');
        m_aTokens.Shrink();
    }

    void CTokenizer::ProcessString(const char* sString) {
        char* C = (char*)sString;
        while(*C != '\0') {
            ProcessCharacter(*C);
            C++;
        }
        Finish();
    }

    void CTokenizer::Print() {
        for(size_t i = 0; i < m_aTokens.Length(); i++) {
            Token& Current = m_aTokens[i];

            printf("%s = '%s'\n", GetTokenName(Current.iType), Current.sLexeme.String());
        }
    }
}
