#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#include <utility>
#include <vector>
#include <sstream>
#include <cstring>
#include "bit_tools.h"

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

    u32 callWord(u32 word){
#if BYTE_ORDER == LITTLE_ENDIAN
        return (rotl(word,24 ) & 0xFF00FF00) | (rotl(word,8) & 0x00FF00FF);
#elif BYTE_ORDER == BIG_ENDIAN
        return buffer.words[word];
#else
#error "Endianness not defined!"
#endif
    }

public:
    void process(u8 chunk[64]);
    void print() const;

    sha1(const char * str, u64 length){

        /*
        for(u8 i=0; i<3; i++) buffer.bytes[i] = str[i];

        buffer.bytes[3] = 0x80 ;

        //for(u8 i = 3+1; i< 62; i++) buffer->bytes[i] = 0;

        buffer.bytes[62] =  0;
        buffer.bytes[63] = 24;


        process(buffer.bytes);
        print();
        /**/
        //*

        u64 byte_passed, byte_left;

        byte_passed = 0;
        byte_left = length;

        do {

            for(u8 i=0; i<byte_left; i++) buffer.bytes[i] = str[i];

            if( byte_left < 64 ){

                byte_passed = byte_left;
                ml += (byte_passed)<<3; // shifting by 3 instead of multiplying by 8

                buffer.bytes[byte_left] = 0x80 ;

                for(u8 i = byte_left+1; i< 62; i++) buffer.bytes[i] = 0;

                buffer.bytes[62] = ml>>32 ;
                buffer.bytes[63] = ml ;
            } else {
                byte_passed = 512;
                ml += byte_passed<<3;
            }


            process(buffer.bytes);

            byte_left -= byte_passed;

        } while(byte_left>0);

        print();
        //for(int i =0;i<64; i++) std::cout << i << ": " << (int)buffer.bytes[i] << '\n';
        /**/
    }

};

#endif //MY_SHA1_SHA1_H
