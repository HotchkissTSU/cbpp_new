#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbep/state.h"

#include "cbpp/cbdef.h"
#include "cbpp_api/String.h"

void InitGLFW() {
    CBPP_ASSERT(glfwInit() == 0, "Failed to initialize GLFW");
}

int main(int argc, char** argv) {
    InitGLFW();

    using namespace cbpp;

    CString sTest("fuck");
    CString sTest2(" niggers");

    CString sSum = sTest + sTest2;

    printf("%s\n", sSum.String());

    return EXIT_SUCCESS;
}
