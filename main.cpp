#include <iostream>
#include "bit_tools.h"
#include "sha1.h"


int main() {

    SHA1 udmm(std::vector<u32>(0));
    udmm.run();
    udmm.print();

    //bitShow(0x7fff);
    //bitShow( rotr(0x7fff,3) );
    //bitShow( rotl(0x7fff,1) );

    return 0;
}
