#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"

#define length(x) (sizeof(x)/sizeof(x[0])-1)

int main() {

    char var_1[] = "abc"; // a9993e364706816aba3e25717850c26c9cd0d89d
    char *var_2; // da39a3ee5e6b4b0d3255bfef95601890afd80709
    char var_3[] = "stuff123456stuff123456123456stuff123456123456stuff1234456333334";
    char var_4[] = "abcdbcdecdefdefgefghfghighihijkijkljklmklmnlmnonopnop123321222111";

    sha1 udmm(var_1, length(var_1));
    //sha1 udmm2(var_2, 0);

    //sha1 udmm3(var_3, length(var_3));
    //sha1 udmm4(var_4, length(var_4));

    u32 ll= 1000001;
    char ff_str[ll];

    for(u32 i=0; i<ll; i++) ff_str[i] = 'a';

    //sha1 ff(ff_str, length(ff_str));

    /* spliting u64 into 2 u32 arrays
    u64 x = 12345678912345;
    u32 y[2];

    y[0]= x>>32;
    y[1] = x;

    bitShow( x );
    bitShow( y[0] );
    bitShow( y[1] );
    showBits( ((u64)y[0]<<32)+y[1] );
    /**/

    //showBits( rotl(0x7fff,1) );

    return 0;
}
