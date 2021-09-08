#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#include <utility>
#include <vector>
#include <sstream>
#include <cstring>
#include "bit_tools.h"

class SHA1 {

    u32 h0 = 0x67452301;
    u32 h1 = 0xEFCDAB89;
    u32 h2 = 0x98BADCFE;
    u32 h3 = 0x10325476;
    u32 h4 = 0xC3D2E1F0;

    u64 ml = 0; // message length in bits
    u64 byte_left = 0; // byte left in chunck

    typedef union {
        u8 bytes[64];
        u32 words[16];
    } chunk_t;

    chunk_t buffer = { .bytes = {} }; // declare with bytes being empty;

    inline void endianWord(u8 i); // set words in big ENDIAN if needed
    void putBitCountAtTheEnd(); // put ml at the end of the buffer
    void process(); // hash 512 bits in the buffer and then clear it.

public:
    void update(const char * str, u64 length); // parse str to process
    void final();
    void print() const; // print string out
    void output(char * out) const; // return the hash pieces

    SHA1() = default;

};

void sha1(char *out, const char *str, u64 length);

#endif //MY_SHA1_SHA1_H
