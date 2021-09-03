#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#include <utility>
#include <vector>
#include <sstream>
#include <cstring>
#include "bit_tools.h"

#define upto64(x) ( (x>64) ? 64 : x )

class SHA1 {

    u32 h0 = 0x67452301;
    u32 h1 = 0xEFCDAB89;
    u32 h2 = 0x98BADCFE;
    u32 h3 = 0x10325476;
    u32 h4 = 0xC3D2E1F0;

    u64 ml = 0; // message length in bits

    typedef union {
        u8 bytes[64];
        u32 words[16];
    } chunk_t;

    chunk_t buffer = { .bytes = {} }; // declare with bytes being empty;

    u32 callWord(u32 word); // make sure words are in big ENDIAN
    void putBitCountAtTheEnd(); // put ml at the end of the buffer

public:
    void process(u8 chunk[64]); // hash 512 bits, main function of this class
    void update(const char * str, u64 length); // parse str to process
    void print() const; // print string out
    void output(char * out) const; // return the hash pieces

    SHA1() = default;

    SHA1(const char * str, u64 len){
        update(str, len);
    };


};

char * sha1(char *out, const char *str, u64 length);

#endif //MY_SHA1_SHA1_H
