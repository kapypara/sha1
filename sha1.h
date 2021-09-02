#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#include <utility>
#include <vector>
#include <sstream>
#include <cstring>
#include "bit_tools.h"

#define upto64(x) ( (x>64) ? 64 : x )

class sha1 {

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

    u32 callWord(u32 word);
    void putBitCountAtTheEnd(); // make sure words are in big ENDIAN

public:
    void process(u8 chunk[64]);
    void print() const;


    sha1(const char * str, u64 length){

        u64 byte_left, byte_passed;

        byte_left = length;

        do {
            byte_passed = ml>>3; // just for readability, only used in memcpy()

            memcpy(&buffer.bytes, (str + byte_passed), upto64(byte_left) );

            if( byte_left > 64 ) {

                byte_left -= 64;
                ml += 512;

                for(u8 i=0; i<16; i++) buffer.words[i] = callWord(buffer.words[i]);

            } else if( byte_left == 64) {

                ml += 512;
                byte_left = 0;

                for(u8 i=0; i<16; i++) buffer.words[i] = callWord(buffer.words[i]);
                process(buffer.bytes);

                buffer.words[0] = 0x80000000; // 1 at the beginning of the first word
                for(u8 i=1; i<14; i++) buffer.words[i] = 0;
                putBitCountAtTheEnd();

            } else if (byte_left >55){

                ml += (byte_left<<3);

                buffer.bytes[byte_left] = 0x80 ;
                for(u8 i = byte_left+1; i< 64; i++) buffer.bytes[i] = 0;

                for(u8 i=0; i<16; i++) buffer.words[i] = callWord(buffer.words[i]);
                process(buffer.bytes);

                for(u8 i=0; i<14; i++) buffer.words[i] = 0;
                putBitCountAtTheEnd();

                byte_left = 0;

            } else {

                ml += (byte_left<<3); // shifting by 3 instead of multiplying by 8

                buffer.bytes[byte_left] = 0x80 ;
                for(u8 i = byte_left+1; i< 56; i++) buffer.bytes[i] = 0;

                for(u8 i=0; i<14; i++) buffer.words[i] = callWord(buffer.words[i]);
                putBitCountAtTheEnd();

                byte_left = 0;

            }

            process(buffer.bytes);

        } while(byte_left>0);

        print();
    }

};

#endif //MY_SHA1_SHA1_H
