#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"

#include <fstream>

bool test(const char *x, const char *y, const u8 num){

    if( !strcmp(x,y) ){
        std::cout << "test: " << (int)num << " passed\n";
        std::cout << "want: " << x <<'\n';
        std::cout << "got:  " << y <<"\n\n";
        return true;

    } else {
        std::cout << "test: " << (int)num << " failed\n";
        std::cout << "want: " << x <<'\n';
        std::cout << "got:  " << y <<"\n\n";
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

    // test 1: simple abc
    const char string_abc[] = "abc"; // a9993e364706816aba3e25717850c26c9cd0d89d
    test_hashing("a9993e364706816aba3e25717850c26c9cd0d89d", string_abc, strlen(string_abc), 1);


    // test 2: no data
    const char string_empty[] = ""; // da39a3ee5e6b4b0d3255bfef95601890afd80709
    test_hashing("da39a3ee5e6b4b0d3255bfef95601890afd80709", string_empty, strlen(string_empty), 2);


    // test 3: 64 bytes
    const char string_random[] = "stuff123456stuff123456123456stuff123456123456stuff1234456333334q";
    test_hashing("5c55f1d6a46e368fbdd830843fa4b77c0d83186d", string_random, strlen(string_random), 3);

    // test 4: manual incremental updates
    char const string1[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghij";
    char const string2[] = "klmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";

    //char const string1[] = "abc";
    //char const string2[] = "123";

    const char expect[] = "a49b2446a02c645bf419f995b67091253a04a259";
    char result[21];
    char hexresult[41];

    SHA1 test4hash;
    test4hash.update(string1, strlen(string1));
    test4hash.update(string2, strlen(string2));
    test4hash.final();
    test4hash.output(result);

    digest_to_hex_string(result, hexresult);

    test( expect, hexresult, 4 );


    //* test 5: million a
    const char a_digest[] = "34aa973cd4c4daa4f61eeb2bdbad27316534016f";
    char a_str[1000001];
    char a_output[21];
    char a_hex[41];

    for(u32 i=0; i<1000000; i++) a_str[i] = 'a';
    a_str[1000000] ='\0';

    sha1(a_output, a_str, strlen(a_str));

    digest_to_hex_string(a_output, a_hex);

    test( a_digest, a_hex, 5 );

    // test 6: useing std::String
    std::string myslirn = "hello;";
    const std::string answer = "5c1acc0c3d8917623f9acf1607c11fdaf8f4321e";

    test_hashing(answer.c_str(), myslirn.c_str(), myslirn.size(), 6);

    //* test 7: file
    const std::string test7answer = "d2a55cc67f3f2f33e1bb195682ae2ea8443f65f5";
    std::ifstream file("/tmp/img.jpg");

    std::string tes7( (std::istreambuf_iterator<char>(file)),
                      (std::istreambuf_iterator<char>()) );

    test_hashing(test7answer.c_str(), tes7.c_str(), tes7.size(), 7);
    /**/

    // test 8: file
    const std::string test8answer = "d2a55cc67f3f2f33e1bb195682ae2ea8443f65f5";
    char output8[21];
    char output8_hex[41];

    std::ifstream file8("/tmp/img.jpg", std::ios::binary);

    file8.seekg(0,std::ios::end);
    u64 length = file8.tellg();
    file8.seekg(0,std::ios::beg);

    SHA1 test8;

    u16 bfs = 512*8;
    for(char buff[bfs]; length > 0;  ) {
        u64 amount = (length > bfs) ? bfs : length ;

        file8.read(buff, amount);

        test8.update(buff, amount );

        length-=amount;

        memset(buff, 0, 256);
    }


    test8.final();
    test8.output(output8);

    digest_to_hex_string(output8, output8_hex);

    test(test8answer.c_str(), output8_hex, 8);


    //showBits( rotl(0x7fff,1) );

   return 0;
}
