#include <stdlib.h>

#include "glfw/glfw3.h"

#include "cbpp/cbdef.h"
#include "cbpp/network.h"
#include "cbpp_api/Entity.h"

void InitGLFW() {
    CbAssert(glfwInit() == 0, "Failed to initialize GLFW");
}

int main(int argc, char** argv) {
    InitGLFW();

    cbpp::CBaseEntity* pEntity = cbpp::CreateEntityClass("test_entity");
    pEntity->Think();

    cbpp::net::CIPAddress test("192.168.1.255:22856");
    test.Print();

    return EXIT_SUCCESS;
}
