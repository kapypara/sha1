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

inline void updateVars(u32 *a, u32 *b, u32 *c, u32 *d, u32 *e, const u32 *t){

    *b = rotl(*b, 30);

    *e = *d;
    *d = *c;
    *c = *b;
    *b = *a;
    *a = *t;

    /*
    static int i = 0;
    std::cout << std::dec << "t" << i << std::hex <<
        " a: " << *a << ", b: " << *b << ", c: " << *c << ", d: " << *d << ", e: " << *e << '\n';
    i+=1;
    /**/
}

u32 get_temp(u32 *w, const u8 i, const u32 a, const u32 e, const u32 f, const u32 k){

    const u8 s = i & 0xf; // mask for values above 16

    w[s] = w[ (s+13) & 0xf ] ^ w[ (s+8) & 0xf ] ^ w[ (s+2) & 0xf ] ^ w[s];
    w[s] = rotl( w[s], 1);

    return rotl(a,5) + f + e + k + w[s];
}

void sha1::run() {

    u32 * word = message.words;

    message.bytes[3] = 0x80 ;
    message.bytes[63] = 24 ;

    //bitShow( callWord(0) );


    /*
    for(u8 t=0;t<16; t++){
        std::cout << (int)t << ": " << getWord(t) << '\n';
        bitShow(callWord(t) );
    }
    std::cout << "words are done!\n";
    /**/


    u32 a = h0;
    u32 b = h1;
    u32 c = h2;
    u32 d = h3;
    u32 e = h4;

    u32 f, k, temp;

    u8 t = 0;

    // make sure words a in big ENDIAN
    for(u8 i=0; i<16; i++) word[i] = callWord(i);

    k = 0x5A827999;
    for(; t < 16; t++){

        f = Ch(b,c,d);
        temp = rotl(a,5) + f + e + k + word[t];
        updateVars(&a, &b, &c, &d, &e, &temp);
    }
    for(; t < 20; t++){

        f = Ch(b,c,d);
        temp = get_temp(word, t, a, e, f, k);
        updateVars(&a, &b, &c, &d, &e, &temp);
    }

    k = 0x6ED9EBA1;
    for(; t < 40; t++){

        f = Parity(b,c,d);
        temp = get_temp(word, t, a, e, f, k);
        updateVars(&a, &b, &c, &d, &e, &temp);
    }

    k = 0x8F1BBCDC;
    for(; t < 60; t++){

        f = Maj(b,c,d);
        temp = get_temp(word, t, a, e, f, k);
        updateVars(&a, &b, &c, &d, &e, &temp);
    }

    k = 0xCA62C1D6;
    for(; t < 80; t++){

        f = Parity(b,c,d);
        temp = get_temp(word, t, a, e, f, k);
        updateVars(&a, &b, &c, &d, &e, &temp);
    }

    h0 += a;
    h1 += b;
    h2 += c;
    h3 += d;
    h4 += e;
}
