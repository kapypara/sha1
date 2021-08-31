#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"


int main() {

    std::string var_1 = "abc";
    //bitShow(var_1[0]);
    sha1 udmm(var_1);
    udmm.run();
    udmm.print();

    //bitShow( rotr(0x7fff,3) );
    //bitShow( rotl(0x7fff,1) );

    return 0;
}
