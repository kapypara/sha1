#include "sha1.h"


void SHA1::print() {

    std::cout << std::hex << h0 << h1 << h2 << h3 << h4 << '\n';
}

void SHA1::run() {
    words.push_back( ('a'<<24) + ('b'<<16) + ('c'<<8) + (1<<7) );
    //bitShow(words[0]);
    words.insert(words.begin()+1,14,0);
    words.push_back(24);
    words.insert(words.end(),64,0);

    for(u8 i=16; i<80; i++){
        //if (i== 65) std::cout << words[i-3] << ' ' <<  words[i-8] << ' ' << words[i-14] << ' ' << words[i-16] << '\n';
        words[i] = (words[i-3] xor words[i-8] xor words[i-14] xor words[i-16]);
        //bitShow(words[i]);
        words[i] = rotl(words[i],1);
        //bitShow(words[i]);
    }
    /*
    for(u8 i=0;i<80; i++){
        std::cout << (int)i << ": " << words[i] << '\n';
    }
    std::cout << "words are done!\n";
    */

    u32 a = h0;
    u32 b = h1;
    u32 c = h2;
    u32 d = h3;
    u32 e = h4;

    u32 f,k, temp;

    //u8 i = 0;
    for(u8 i=0; i<80; i++){
        if (i < 20){
            f = (b & c) ^ (~b & d);
            k = 0x5A827999;
        } else if (i < 40){
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        } else if (i < 60){
            f = (b & c) ^ (b & d) ^ (c & d) ;
            k = 0x8F1BBCDC;
        } else if (i >= 60){
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        temp = rotl(a,5) + f + e + k + words[i];
        e = d;
        d = c;
        c = rotl(b,30);
        b = a;
        a = temp;

        /*
        std::cout << std::dec << "t" << (int)i << std::hex <<
            " a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << ", e: " << e << '\n';
        /**/
    }




    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
}
