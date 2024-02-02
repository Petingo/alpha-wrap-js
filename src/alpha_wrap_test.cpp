#include <iostream>
#include "alpha_wrap.h"

int main() {
    AlphaWrap alphaWrap;

    std::string serializedPoints = "1,2,3\n4,5,6\n7,8,9\n";
    auto result = alphaWrap.wrap((char*)serializedPoints.c_str());

    std::cout << result << std::endl;
    return 0;
}