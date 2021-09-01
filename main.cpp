#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"


int main() {

    char var_1[] = { 'a', 'b', 'c' };
    char *var_2;

    //bitShow(var_1[0]);
    sha1 udmm(var_1, sizeof(var_1)/sizeof(var_1[0]));
    sha1 udmm2(var_2, 0);

    /* spliting u64 into 2 u32 arrays
    u64 x = 12345678912345;
    u32 y[2];

    y[0]= x>>32;
    y[1] = x;

    bitShow( x );
    bitShow( y[0] );
    bitShow( y[1] );
    bitShow( ((u64)y[0]<<32)+y[1] );
    /**/

    //bitShow( rotl(0x7fff,1) );

    return 0;
}
