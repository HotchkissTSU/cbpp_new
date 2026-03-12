#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbpp/Entity.h"
#include "cbpp/Filesystem.h"

#include "cbpp/Table.h"
#include "cbpp/String.h"

#include "cbpp/CML.h"
#include "cbpp/Benchmark.h"
#include "cbpp/Image.h"

void InitGLFW() {
    CbAssert(glfwInit() == 0, "Failed to initialize GLFW");
}

int main(int argc, char** argv) {
    InitGLFW();

    cbpp::IFile* File = cbpp::OpenFile("assets/goofy.h", "rb");
    if(File == NULL) {
        return 1;
    }

    size_t iFileLength = File->Length();
    char* sFileText = cbpp::Malloc<char>(iFileLength+1);
    sFileText[iFileLength] = '\0';

    File->ReadAll(sFileText);
    cbpp::CString sSource(sFileText);

    cbpp::cml::CTokenizer Lexer(sSource);
    Lexer.ProcessString();
    Lexer.Print();
    
    /*cbpp::cml::CParser Parser;
    if(Parser.ParseString(sFileText)) {
        PrintObject(Parser.Root());
    } else {
        char sBuff[256];
        Parser.GetErrorLog(sBuff, sizeof(sBuff));
        printf("%s\n", sBuff);
    }*/

    cbpp::Free(sFileText);

    cbpp::IEntityDatadesc* pTest = cbpp::CreateEntityDatadesc("CBase");

    size_t iDataLen = pTest->Length();

    printf("Num: %d\n", iDataLen);

    for(int i = 0; i < iDataLen; i++) {
        cbpp::IEntityProperty* pCurrent = pTest->At(i);
        printf("Property '%s' of type %s\n", pCurrent->Name(), cbpp::IEntityProperty::GetTypeName( pCurrent->Type() ));

        printf("\tValue: ");

        switch(pCurrent->Type()) {
            case cbpp::IEntityProperty::EType::Integer: {
                cbpp::CNumberEntityProperty* pNumber = (cbpp::CNumberEntityProperty*)pCurrent;
                printf("%d\n", pNumber->GetValue());
                break;
            }

            case cbpp::IEntityProperty::EType::Float: {
                cbpp::CFloatEntityProperty* pFloat = (cbpp::CFloatEntityProperty*)pCurrent;
                printf("%f\n", pFloat->GetValue());
                break;
            }

            case cbpp::IEntityProperty::EType::String: {
                cbpp::CStringEntityProperty* pString = (cbpp::CStringEntityProperty*)pCurrent;
                printf("'%s'\n", pString->GetValue());
                break;
            }

            case cbpp::IEntityProperty::EType::Enum: {
                cbpp::CEnumEntityProperty* pNumber = (cbpp::CEnumEntityProperty*)pCurrent;
                
                for(int k = 0; k < pNumber->PairsAmount(); k++) {
                    printf("\n\t'%s' = %d", pNumber->GetPairs()[k].Name, pNumber->GetPairs()[k].Value);
                }
                putc('\n', stdout);

                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
