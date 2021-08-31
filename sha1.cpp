#include "sha1.h"


void sha1::print() const {

    std::cout << std::hex << h0 << h1 << h2 << h3 << h4 << '\n';
}

inline u32 Ch(u32 x, u32 y, u32 z){
    return (x & y) ^ (~x & z);
}

inline u32 Parity(u32 x, u32 y, u32 z){
    return x ^ y ^ z;
}

inline u32 Maj(u32 x, u32 y, u32 z){
    return (x & y) ^ (x & z) ^ (y & z);
}

void sha1::run() {

    u32 * word = message.words;

    message.bytes[3] = 0x80 ;
    message.bytes[63] = 24 ;

    //bitShow( getWord(0) );


    for(u8 i=0;i<16; i++){
        std::cout << (int)i << ": " << getWord(i) << '\n';
        bitShow(getWord(i) );
    }
    std::cout << "words are done!\n";
    /**/

    u32 a = h0;
    u32 b = h1;
    u32 c = h2;
    u32 d = h3;
    u32 e = h4;

    u32 f,k, temp, Ws;

    //u8 i = 0;
    for(u8 i=0; i<80; i++){
        if (i < 20){
            f = Ch(b,c,d);
            k = 0x5A827999;
        } else if (i < 40){
            f = Parity(b,c,d);
            k = 0x6ED9EBA1;
        } else if (i < 60){
            f = Maj(b,c,d);
            k = 0x8F1BBCDC;
        } else if (i >= 60){
            f = Parity(b,c,d);
            k = 0xCA62C1D6;
        }


        if(i < 16){
            word[i] = getWord(i);
            temp = rotl(a,5) + f + e + k + word[i];
        } else {
            u8 s = i & 0xf; // mask for values above 16

            word[s] = word[ (s+13) & 0xf ] ^ word[ (s+8) & 0xf ] ^ word[ (s+2) & 0xf ] ^ word[s];
            word[s] = rotl( word[s], 1);
            temp = rotl(a,5) + f + e + k + word[s];
        }

        if(i==19){
            bitShow( temp );
            bitShow( 0xfd9e1d7d );
        }

        e = d;
        d = c;
        c = rotl(b,30);
        b = a;
        a = temp;


        std::cout << std::dec << "t" << (int)i << std::hex <<
            " a: " << a << ", b: " << b << ", c: " << c << ", d: " << d << ", e: " << e << '\n';

    }




    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
}
