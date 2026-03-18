#include <stdio.h>
#include <stddef.h>

#include "cbep/cbep.h"

#include "cbpp/Error.h"
#include "cbpp/Dynlib.h"
#include "cbpp/Module.h"

cbsv_state_t g_State;

const char* g_sDefaultGamefile = "default";

int main(int argc, char** argv) {
    ParseArgs(argc, argv);

    if(g_State.sGamefilePath == NULL) {
        cbpp::WriteLogf(cbpp::ELogLevel::Info, "[CBEP] No gamefile is specified, assuming default ('%s')", g_sDefaultGamefile);
        g_State.sGamefilePath = (char*)g_sDefaultGamefile;
    }

    char sBuffer[128];
    snprintf(sBuffer, sizeof(sBuffer), "bin/%s.so", g_State.sGamefilePath);

    cbpp::CDynamicLibrary hGame( sBuffer );

    void* fpMainTest = hGame.GetSymbol("cbpp_main");

    if(fpMainTest == NULL) {
        cbpp::Throwf("The module '%s' has no 'cbpp_main' callback specified", sBuffer);
    }

    auto fpMainCallback = (cbpp::ModuleInfo::fpmain_t)( fpMainTest );

    int iMainCode = fpMainCallback(argc, argv);

    cbpp::WriteLogf(cbpp::ELogLevel::Info, "Module '%s' has finished with exit code %d", sBuffer, iMainCode);

    return 0;
}
