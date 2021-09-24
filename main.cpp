#include <iostream>
#include <cstring>
#include "bit_tools.h"
#include "sha1.h"

#include <fstream>
#include <chrono>

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

    test_hashing(a_digest, a_str, strlen(a_str), 5);

    // test 6: useing std::String
    std::string myslirn = "hello;";
    const std::string answer = "5c1acc0c3d8917623f9acf1607c11fdaf8f4321e";

    test_hashing(answer.c_str(), myslirn.c_str(), myslirn.size(), 6);


    //* test 7: file
    auto const test7beging = std::chrono::system_clock::now();
    const std::string test7answer = "7e827ac16eb9a161306468bdd1c86d43a10c9aa2";
    std::ifstream file("/tmp/img.jpg", std::ios::binary);

    if( file ) {

#if 0
        std::string tes7((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
#else
        // different method
        file.seekg(0, std::ios::end);
        u64 fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> tes7(fileSize);
        file.read(tes7.data(), fileSize);
#endif

        test_hashing(test7answer.c_str(), tes7.data(), tes7.size(), 7);
    }
    auto const test7end = std::chrono::system_clock::now();
    std::cout << "test 7 duration: " << std::chrono::duration_cast<std::chrono::microseconds>(test7end-test7beging).count() << " µs \n\n";
    /**/

    // test 8: file loaded into a prefixed array
    auto const test8beging = std::chrono::system_clock::now();
    const std::string test8answer = "7e827ac16eb9a161306468bdd1c86d43a10c9aa2";
    char output8[21];
    char output8_hex[41];

    std::ifstream file8("/tmp/img.jpg", std::ios::binary);

    if( file8 ) {

        /*
        file8.seekg(0, std::ios::end);
        u64 length = file8.tellg();
        file8.seekg(0, std::ios::beg);
        */

        SHA1 test8;

        u16 bfs = 512*8;
        char buff[bfs];
        do {
            file8.read(buff, bfs);

            test8.update(buff, file8.gcount() );

        } while (file8.gcount() == bfs);

        test8.final();
        test8.output(output8);

        digest_to_hex_string(output8, output8_hex);

        test(test8answer.c_str(), output8_hex, 8);
    }
    auto const test8end = std::chrono::system_clock::now();
    std::cout << "test 8 duration: " << std::chrono::duration_cast<std::chrono::microseconds>(test8end-test8beging).count() << " µs \n\n";

    // test 9: 60 bytes
    const char string_random9[] = "stuff123456stuff123456123456stuff123456123456stuff1234456333";
    test_hashing("07ed6dcdbd6035629f2aa3cafb4c0cdc5b39ab27", string_random, strlen(string_random9), 9);

    // test 10
    const char expect10[] = "7c896e1135f30e40830a01f67f4ada4f2384cdef";
    char test10digest[21];
    char test10hex_hash[41];

    /* following string broken into separate updates
     * sudocompsize./################################################ffmpeg-i./deskshare.webm-movflagsfaststart-pix_fmtyuv420p-vf"scale=trunc(iw/2)*2";:trunc(ih/2)*2"-vf"setpts=0.05*PTS"out2.mp4----systemprofileingsudoperfrecord-g-asleep10----balncebtrfsfast?sudobtrfsbalancestart-musage=35-dusage=35/sudobtrfsbalancestart-dusage=50-dlimit=4-musage=50-mlimit=16/sudobtrfsbalancestart-dusage=90-musage=90/----wrtingtofileatcertinaddreaseprintf'\x48\xC7\xC0\x19\x01\x00\x00\xC3'|ddof=sublime_merge.exebs=1seek=145194conv=notrunc----findoldestfileswithfindfind-typef-printf'%T+%p\n'|sort|head-n1###libvirtfixnetworking###usevirshnet-*toolswhenigotitworkingpackges[iptables-nftbridge-utilsdnsmasq]wereinstallednofirewall(I'mnotsuretho)withtheuseofthisrule:<network><name>mess</name><uuid>4bf02277-db72-4dde-878a-8c6642c6b94a</uuid><forwardmode='nat'><nat><portstart='1024'end='65535'/></nat></forward><bridgename='brog0'stp='on'delay='0'/><macaddress='52:54:00:bc:82:36'/><ipaddress='192.168.122.1'netmask='255.255.255.0'><dhcp><rangestart='192.168.122.2'end='192.168.122.254'/></dhcp></ip>
     * should give 7c896e1135f30e40830a01f67f4ada4f2384cdef
     */
    const char test10str1[] = R"(sudocompsize)";
    const char test10str2[] = R"(./################################################ffmpeg-i./deskshare.webm-movflagsfaststart-pix_fmtyuv420p-vf"scale=trunc(iw/2)*2";)";
    const char test10str3[] = R"(:trunc(ih/2)*2"-vf"set)";
    const char test10str4[] = R"(pt)";
    const char test10str5[] = R"(s=0.0)";
    const char test10str6[] = R"(5*PTS"out2.mp4----systemprofileingsudoperfrecord-g-asleep10----balncebtrfsfast?sudobtrfsbalancestart-musage=35-dusage=35/sudobtrfsbalancestart-dusage=50-dlimit=4-musage=50-mlimit=16/sudobtrfsbalancestart-dusage=90-musage=90/----wrtingtofileatcertinaddreaseprintfexebs=1seek=145194conv=notrunc----findoldestfileswithfindfind-typef-printf'%T+%p\n'|sort|head-n1###libvirtfixnetworking###usevirshnet-*toolswhenigotitworkingpackges[iptables-nftbridge-utilsdnsmasq]wereinstallednofirewall(I'mnotsuretho)withtheuseofthisrule:<network><name>mess</name><uuid>4bf02277-db72-4dde-878a-8c6642c6b94a</uuid><forwardmode='nat'><nat><portstart='1024'end='65535'/></nat></forward><bridgename='brog0'stp='on'delay='0'/><macaddress='52:54:00:bc:82:36'/><ipaddress='192.168.122.1'netmask='255.255.255.0'><dhcp><rangestart='192.168.122.2'end='192.168.122.254'/></dhcp></ip>)";
    SHA1 test10;

    test10.update(test10str1, 0);
    test10.update(test10str1, strlen(test10str1));
    test10.update(test10str2, strlen(test10str2));
    test10.update(test10str3, strlen(test10str3));

    test10.update(NULL,0);
    test10.update(test10str4, strlen(test10str4));

    test10.update(NULL,0);
    test10.update(NULL,0);
    test10.update(test10str5, strlen(test10str5));
    test10.update(test10str5, 0);
    test10.update(NULL,0);

    test10.update(test10str6, strlen(test10str6));

    test10.update(NULL,0);
    test10.update(NULL,0);
    test10.final();
    test10.output(test10digest);

    digest_to_hex_string(test10digest, test10hex_hash);

    test(expect10, test10hex_hash, 10);

    //showBits( rotl(0x7fff,1) );

   return 0;
}
