#include "sha1.h"


void sha1::print() const {

    std::cout << std::hex << h0 << h1 << h2 << h3 << h4 << '\n';
}

void sha1::putBitCountAtTheEnd(){
    buffer.words[14] = ml>>32 ;
    buffer.words[15] = ml ;
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

void sha1::process(u8 chunk[64]) {


    u32 * word = (u32*)chunk;

    // make sure words are in big ENDIAN
    //for(u8 i=0; i<16; i++) word[i] = callWord(word[i]);

    //showBits( callWord(0) );


    //*
    std::cout << "[ sha1::process ] listing words!, data in hex\n";
    for(u8 t=0;t<16; t++){
        std::cout << std::dec << 't' << (int)t << ": " << std::hex << word[t] << std::dec << '\n';
        showBits(word[t]);
    }
    std::cout << "[ sha1::process ] done listing words!\n";
    /**/


    u32 a = h0;
    u32 b = h1;
    u32 c = h2;
    u32 d = h3;
    u32 e = h4;

    u32 f, k, temp;

    u8 t = 0;


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
