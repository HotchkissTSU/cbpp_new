#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbpp/cbdef.h"
#include "cbpp_api/Entity.h"
#include "cbpp/filesystem.h"

#include "cbpp_api/Table.h"
#include "cbpp_api/String.h"

void InitGLFW() {
    CbAssert(glfwInit() == 0, "Failed to initialize GLFW");
}

int main(int argc, char** argv) {
    InitGLFW();

    cbpp::CBaseEntity* pEntity = cbpp::CreateEntityClass("ent_test");

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
    }

    cbpp::ParsePath("FS:/scripts/map_test/client/fx.ysl");

    cbpp::CBinTable<cbpp::CConstString, float> dTest;

    const char* aNames[] = {
        "reklov", //0
        "sineman", //100
        "nonexistent_kitsune", //200
        "trastar",//300
        "amazing_table_model"  //400
    };

    for(int i = 0; i < 5; i++) {
        dTest.Insert(aNames[i], i*100);
    }

    dTest.Print();

    for(int i = 0; i < 5; i++) {
        float* pValue = dTest.At(aNames[i]);
        printf("%s = %f\n", aNames[i], pValue == NULL ? -1.0f : *pValue);
    }

    return EXIT_SUCCESS;
}
