#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbpp/cbdef.h"
#include "cbpp/network.h"

void InitGLFW() {
    CBPP_ASSERT(glfwInit() == 0, "Failed to initialize GLFW");
}

CbNetMessage(cbpp_test) {
    return true;
}

int main(int argc, char** argv) {
    InitGLFW();

    return EXIT_SUCCESS;
}
