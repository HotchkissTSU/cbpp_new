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

    /*cbpp::CBinTable<cbpp::CSubString, int> dTest;

    cbpp::CString s1("word1 word2 word3");

    dTest.Insert(cbpp::CSubString(s1, 0, 5), 1);*/

    cbpp::IFile* File = cbpp::OpenFile("assets/locale/test.cml", "rb");
    if(!File->IsOpen()) {
        return 1;
    }

    size_t iFileLength = File->Length();
    char* sFileText = cbpp::Malloc<char>(iFileLength+1);
    sFileText[iFileLength] = '\0';

    File->ReadAll(sFileText);

    CbBenchmark();
    
    /*cbpp::cml::CParser Parser;
    if(Parser.ParseString(sFileText)) {
        PrintObject(Parser.Root());
    } else {
        char sBuff[256];
        Parser.GetErrorLog(sBuff, sizeof(sBuff));
        printf("%s\n", sBuff);
    }*/

    cbpp::CImage imageTest(128, 128);
    imageTest.Fill(cbpp::Color(255,255,0,255));

    imageTest.SaveAs("test.bmp", cbpp::EImageType::BMP);
    imageTest.SaveAs("test.png", cbpp::EImageType::PNG, 9);
    imageTest.SaveAs("test.jpeg", cbpp::EImageType::JPEG, 100);
    imageTest.SaveAs("test.tga", cbpp::EImageType::TGA);

    return EXIT_SUCCESS;
}
