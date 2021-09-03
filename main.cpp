#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"

#define length(x) (sizeof(x)/sizeof(x[0])-1)
bool test(char *x, char *y){

    if( !strcmp(x,y) ){
        std::cout << "test passed\n";
        std::cout << x <<'\n';
        std::cout << y <<"\n\n";
        return true;

    } else {
        std::cout << "test failed\n";
        std::cout << x <<'\n';
        std::cout << y <<"\n\n";
        return false;

    }
}

inline void hex_hash_To_hex_char(const char *hash, char *str){

    for(u8 i = 0; i < 20; i++) {
        sprintf( str+(i*2), "%02x", hash[i]&0xff);
    }
    str[40] = '\0';
}

int main() {

    //char *hash = sha1((char *) "abc", 3);

    //std::printf("%s\n",hash);

    char want[] = "906c542ab5d4a13c0df66f505bfbcc3fd95044d0";

    char tobehashed[] = "stuff123456stuff123456123456stuff123456123456stuff1234456333334";
    char hexhash[21];
    char digested[41];

    sha1(hexhash, tobehashed, strlen(tobehashed));

    hex_hash_To_hex_char(hexhash, digested);

    test( want, digested );

    char const string1[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghij";
    char const string2[] = "klmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    char expect[] = "a49b2446a02c645bf419f995b67091253a04a259";
    char result[21];
    char hexresult[41];

    SHA1 hash;
    hash.update(string1, strlen(string1));
    hash.update(string2, strlen(string2));
    hash.output(result);

    hex_hash_To_hex_char(result, hexresult);

    test( expect, hexresult );

    /*
    char var_1[] = "abc"; // a9993e364706816aba3e25717850c26c9cd0d89d
    char *var_2; // da39a3ee5e6b4b0d3255bfef95601890afd80709
    char var_3[] = "stuff123456stuff123456123456stuff123456123456stuff1234456333334";
    char var_4[] = "abcdbcdecdefdefgefghfghighihijkijkljklmklmnlmnonopnop123321222111";

    SHA1 udmm(var_1, length(var_1));
    //SHA1 udmm2(var_2, 0);

    //SHA1 udmm3(var_3, length(var_3));
    //SHA1 udmm4(var_4, length(var_4));

    u32 ll= 1000001;
    char ff_str[ll];

    for(u32 i=0; i<ll; i++) ff_str[i] = 'a';

    SHA1 ff(ff_str, length(ff_str));

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
