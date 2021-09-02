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

    void putBitCountAtTheEnd();

public:
    void process(u8 chunk[64]);
    void print() const;


    sha1(const char * str, u64 length){

        u64 byte_left, byte_passed;

        byte_left = length;
        //std::cout << "[ SHA1 constructor ]: byte_left before loop " << std::dec << byte_left << '\n';

        do {
            byte_passed = ml>>3;

            memcpy(&buffer.bytes, (str + byte_passed), 64);
            /*
            std::cout << (char)buffer.bytes[0] << '\n';
            std::cout << (char)buffer.bytes[1] << '\n';
            std::cout << (char)buffer.bytes[2] << '\n';
            /*
            for(u64 i=0; i<length; i++) {
                buffer.bytes[i] = str[i+byte_passed];
            }
             */

            if( byte_left > 64 ) {
                std::cout << "[ consturct ] >64\n";

                byte_left -= 64;
                ml += 512;
                //std::cout << "[ SHA1 constructor ]: ml is now " << std::dec << ml << '\n';

                // make sure words are in big ENDIAN
                for(u8 i=0; i<16; i++) buffer.words[i] = callWord(buffer.words[i]);

            } else if( byte_left == 64) {
                std::cout << "[ consturct ] ==64\n";

                ml += 512;
                byte_left = 0;

                for(u8 i=0; i<16; i++) buffer.words[i] = callWord(buffer.words[i]);
                process(buffer.bytes);

                buffer.words[0] = 0x80000000; // 1 at the beginning of the first word
                for(u8 i=1; i<14; i++) buffer.words[i] = 0;
                putBitCountAtTheEnd();

            } else if (byte_left >55){
                std::cout << "[ consturct ] ==64\n";

                ml += (byte_left<<3);

                std::cout << "[ padding ]: added 0x80 at " << std::dec << byte_left+1 << '\n';
                buffer.bytes[byte_left] = 0x80 ;

                for(u8 i = byte_left+1; i< 64; i++) buffer.bytes[i] = 0;

                for(u8 i=0; i<16; i++) buffer.words[i] = callWord(buffer.words[i]);
                process(buffer.bytes);

                for(u8 i=0; i<14; i++) buffer.words[i] = 0;
                putBitCountAtTheEnd();

                byte_left = 0;

            } else{
                std::cout << "[ consturct ] else\n";

                ml += (byte_left<<3); // shifting by 3 instead of multiplying by 8

                std::cout << "[ padding ]: added 0x80 at " << std::dec << byte_left+1 << '\n';
                buffer.bytes[byte_left] = 0x80 ;

                for(u8 i = byte_left+1; i< 56; i++) buffer.bytes[i] = 0;

                putBitCountAtTheEnd();


                // make sure only the first 14 words are in big ENDIAN
                for(u8 i=0; i<14; i++) buffer.words[i] = callWord(buffer.words[i]);

                byte_left = 0;

            }

            process(buffer.bytes);

            //std::cout << "[ SHA1 constructor ]: byteleft" << byte_left << '\n';
        } while(byte_left>0);

        print();
        //for(int i =0;i<64; i++) std::cout << i << ": " << (int)buffer.bytes[i] << '\n';
        /**/
    }

};

#endif //MY_SHA1_SHA1_H
