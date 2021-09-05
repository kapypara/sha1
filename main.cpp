#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"

bool test(const char *x, const char *y, const u8 num){

    if( !strcmp(x,y) ){
        std::cout << "test: " << (int)num << " passed\n";
        std::cout << x <<'\n';
        std::cout << y <<"\n\n";
        return true;

    } else {
        std::cout << "test: " << (int)num << " failed\n";
        std::cout << x <<'\n';
        std::cout << y <<"\n\n";
        return false;

    }
}

inline void digest_to_hex_string(const char *hash, char *str){

    for(u8 i = 0; i < 20; i++) {
        sprintf( str+(i*2), "%02x", hash[i]&0xff);
    }
    str[40] = '\0';
}

void test_hashing(const char *hash, const char *data_to_hash, const u64 data_length, const u8 test_num){

    char digested[21];
    char hex_string[41];

    sha1(digested, data_to_hash, data_length);

    digest_to_hex_string(digested, hex_string);

    test( hash, hex_string, test_num);
}

int main() {

    // test 1
    const char string_abc[] = "abc"; // a9993e364706816aba3e25717850c26c9cd0d89d
    test_hashing("a9993e364706816aba3e25717850c26c9cd0d89d", string_abc, strlen(string_abc), 1);


    // test 2
    const char string_empty[] = ""; // da39a3ee5e6b4b0d3255bfef95601890afd80709
    test_hashing("da39a3ee5e6b4b0d3255bfef95601890afd80709", string_empty, strlen(string_empty), 2);


    // test 3
    const char string_random[] = "stuff123456stuff123456123456stuff123456123456stuff1234456333334";
    test_hashing("906c542ab5d4a13c0df66f505bfbcc3fd95044d0", string_random, strlen(string_random), 3);

    // test 4
    char const string1[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghij";
    char const string2[] = "klmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    char expect[] = "a49b2446a02c645bf419f995b67091253a04a259";
    char result[21];
    char hexresult[41];

    SHA1 hash;
    hash.update(string1, strlen(string1));
    hash.update(string2, strlen(string2));
    hash.output(result);

    digest_to_hex_string(result, hexresult);

    test( expect, hexresult, 4 );


    /* test 5: million a
    const char a_digest[] = "34aa973cd4c4daa4f61eeb2bdbad27316534016f";
    char a_str[1000001];
    char a_output[21];
    char a_hex[41];

    for(u32 i=0; i<1000000; i++) a_str[i] = 'a';
    a_str[1000000] ='\0';

    sha1(a_output, a_str, strlen(a_str));

    digest_to_hex_string(a_output, a_hex);

    test( a_digest, a_hex );
    /**/

    //showBits( rotl(0x7fff,1) );

    return 0;
}
