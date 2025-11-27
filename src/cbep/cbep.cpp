#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbpp/cbdef.h"
#include "cbpp_api/Entity.h"
#include "cbpp/filesystem.h"

#include "cbpp_api/Table.h"

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

    cbpp::CTable<int, float> dTest;
    
    for(int i = 200; i > 0; i--) {
        dTest[i] = i*10.0f;
    }
    
    forever {
        int iIndex;
        scanf("%d", &iIndex); //putc('\n', stdout);

        printf("Table[%d] = %f\n", iIndex, *(dTest.At(iIndex)));
    }

    return EXIT_SUCCESS;
}
