#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbpp/cbdef.h"
#include "cbpp_api/Entity.h"
#include "cbpp/filesystem.h"

#include "cbpp_api/Table.h"
#include "cbpp_api/String.h"

#include "cbpp/cml/cml.h"
#include "cbpp_api/Filesystem.h"

void InitGLFW() {
    CbAssert(glfwInit() == 0, "Failed to initialize GLFW");
}

int main(int argc, char** argv) {
    InitGLFW();

    /*cbpp::CBaseEntity* pEntity = cbpp::CreateEntityClass("ent_test");

    cbpp::IProperty* pCurrent = pEntity->GetProperties();
    while(pCurrent) {
        printf("%s\n", pCurrent->Name());
        if(pCurrent->Type() == cbpp::EGenericType::Enum) {
            cbpp::CEnumProperty* pEnumP = dynamic_cast<cbpp::CEnumProperty*>(pCurrent);
            const cbpp::CEnumProperty::pairs_t& aPairs = pEnumP->GetPairs();

            for(size_t i = 0; i < aPairs.Length(); i++) {
                const cbpp::CEnumProperty::Pair Pair = aPairs.At(i);
                printf("\t%s = %u\n", Pair.sName, Pair.iValue);
            }
        }
        pCurrent = pCurrent->Next();
    }*/

    //cbpp::ParsePath("FS:/scripts/map_test/client/fx.ysl");

    //cbpp::MountLocale("test.json");

    /*cbpp::CBinTable<cbpp::CString, cbpp::CString> dTest;

    dTest["test1"] = "amongus";
    dTest["test2"] = "spy amongst us!";

    printf("dTest['%s'] = '%s'\n", "test1", dTest["test1"].String());
    printf("dTest['%s'] = '%s'\n", "test2", dTest["test2"].String());*/

    cbpp::IFile* File = cbpp::OpenFile("test.cml", "rb");
    if(!File->IsOpen()) {
        return 1;
    }

    size_t iFileLength = File->Length();
    char* sFileText = cbpp::Malloc<char>(iFileLength+1);
    sFileText[iFileLength] = '\0';

    File->ReadAll(sFileText);

    cbpp::CString sText(sFileText);

    cbpp::cml::CTokenizer Test(sText);

    Test.ProcessString();
    Test.Print();

    /*char sBuff[128];
    const char* sSource = "the kitsune 3D model does not exist obviously, and all rumors around it are a psyop";

    cbpp::CSubString sTest(sSource, 4, 11);
    sTest.Bufferize(sBuff, 128);

    cbpp::CString sCopyTest = sTest.Copy();

    printf("sBuff = '%s'\nsCopyTest = '%s'\n", sBuff, sCopyTest.String());*/

    return EXIT_SUCCESS;
}
