#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#include <utility>
#include <vector>
#include <sstream>
#include "bit_tools.h"

class sha1 {

    u32 h0 = 0x67452301;
    u32 h1 = 0xEFCDAB89;
    u32 h2 = 0x98BADCFE;
    u32 h3 = 0x10325476;
    u32 h4 = 0xC3D2E1F0;

    u64 ml = 0; // message length

    union {
        u8 bytes[64];
        u32 words[16];
    } message;

    u32 getWord(u8 i){
#if BYTE_ORDER == LITTLE_ENDIAN
        return (rotl(message.words[i],24 ) & 0xFF00FF00) | (rotl(message.words[i],8) & 0x00FF00FF);
#elif BYTE_ORDER == BIG_ENDIAN
        return message.words[i];
#else
#error "Endianness not defined!"
#endif
    }

public:
    sha1(std::string input){
        for(u8 i =0;i<16; i++) message.words[i] &= 0;
        std::copy(input.begin(), input.end(), message.bytes);
        //for(int i =0;i<64; i++) std::cout << i << ": " << (int)message.bytes[i] << '\n';
    }
    sha1(std::vector<u8> input) {
        for(u8 i =0;i<16; i++) message.words[i] &= 0;
        std::copy(input.begin(), input.end(), message.bytes);
        //for(int i =0;i<64; i++) std::cout << i << ": " << (int)message.bytes[i] << '\n';
    }
    void run();
    void print() const;

};

#endif //MY_SHA1_SHA1_H
