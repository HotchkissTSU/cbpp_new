#include <stdio.h>

class CTestClass {
    int m_iNumber;

    public:
        CTestClass() = default;

        int GetMyInt() {
            return m_iNumber;
        }
};

int main() {
    return 0;
}

#include "luamac.h"
luamac("test.lua", 52, "hello world")
