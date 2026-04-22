#include "cbpp/CML.h"
#include "cbpp/Entity.h"

#include "engine/entity/CHealth.h"

int main(int argc, char** argv) {
    using namespace cbpp;

    char sBuffer[512];

    cml::CObject pRoot = cml::CreateObject(cml::EObjectClass::Object);

    for(size_t iEnt = 0; iEnt < GetEntFactoryMap().Length(); iEnt++) {
        const char* sEntName = GetEntFactoryMap().IndexKey(iEnt);

        puts(sEntName);

        cml::CObject pEntObj = cml::CreateObject(cml::EObjectClass::Object);
        
        IEntityDatadesc* pTable = GetEntFactoryMap().Index(iEnt).fpDatadescCreator();

        const size_t iLength = pTable->Length();
        for(size_t i = 0; i < iLength; i++) {
            IEntityProperty* pData = pTable->At(i);
            const char* sPropName = pData->Name();

            printf("\t%s\n", sPropName);
            snprintf(sBuffer, sizeof(sBuffer), "%s_entry", sPropName);
            
            pEntObj.Push(sPropName, sBuffer);
        }
        putc('\n', stdout);

        Delete(pTable);

        pRoot.Push(sEntName, pEntObj);
    }

    cml::PrintObject(pRoot);

    cml::DeleteObject(pRoot);

    return 0;
}
