#include "sha1.h"

void sha1(char *out, const char *str, const u64 length){
    SHA1 hash;

    hash.update(str, length);
    hash.final();
    //hash.print();

    hash.output(out);
}

inline void wordToChar(char *str, u8 offset, u32 word){

    sprintf(str+offset,  "%c%c%c%c", (u8)(word>>24), (u8)(word>>16), (u8)(word>>8), (u8)(word)  );
}

void SHA1::output(char *out) const{

    wordToChar(out, 0,  h0);
    wordToChar(out, 4,  h1);
    wordToChar(out, 8,  h2);
    wordToChar(out, 12, h3);
    wordToChar(out, 16, h4);

    out[20] = '\0';

}

void SHA1::print() const {

    //std::cout << "byte count: " << (ml>>3) << '\n';
    //std::cout << std::hex << h0 << h1 << h2 << h3 << h4 << '\n';
    std::printf("%08x%08x%08x%08x%08x\n",h0,h1,h2,h3,h4);
}

u32 SHA1::endianWord(u32 word){
#if BYTE_ORDER == LITTLE_ENDIAN
    return (rotl(word,24 ) & 0xFF00FF00) | (rotl(word,8) & 0x00FF00FF);
#elif BYTE_ORDER == BIG_ENDIAN
    return word;
#else
#error "Endianness not defined!"
#endif
}

void SHA1::putBitCountAtTheEnd(){
    // this may be broken in BIG_ENDIAN. maybe flipping the 14 & 15 will fix it in that case.
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

void SHA1::process() {


    u32 * word = buffer.words;

    // make sure words are in big ENDIAN
    //for(u8 i=0; i<16; i++) word[i] = endianWord(word[i]);

    //showBits( endianWord(0) );


    /*
    std::cout << "[ SHA1::process ] listing words!, data in hex\n";
    for (u8 t = 0; t < 16; t++) {
        std::cout << std::dec << 't' << (int) t << ": " << std::hex << word[t] << std::dec << '\n';
        showBits(word[t]);
    }
    std::cout << "[ SHA1::process ] done listing words!\n";
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

    for(u8 i=0; i<16; i++) buffer.words[i] = 0;
}

void SHA1::update(const char * str, u64 length){

    ml += (length<<3);
    u64 byte_added;
    u64 byte_passed = 0;

    while ( (length+byte_left) > 63 ) {

        byte_added = 64-byte_left;

        memcpy(&buffer.bytes[byte_left] , &str[byte_passed], byte_added );

        length -= byte_added;
        byte_passed += byte_added;

        byte_left=0;

        for(u8 i=0; i<16; i++) buffer.words[i] = endianWord(buffer.words[i]);
        process();
    }

    memcpy(&buffer.bytes[byte_left] , &str[byte_passed], length);

    byte_left += length;

    //print();
}

void SHA1::final() {

    if (byte_left > 55) {

        buffer.bytes[byte_left] = 0x80 ;
        for(u8 i = byte_left + 1; i < 64; i++) buffer.bytes[i] = 0;

        for(u8 i=0; i<16; i++) buffer.words[i] = endianWord(buffer.words[i]);
        process();


        for(u8 i=0; i<14; i++) buffer.words[i] = 0;
        putBitCountAtTheEnd();

    } else {

        buffer.bytes[byte_left] = 0x80 ;
        for(u8 i = byte_left + 1; i < 56; i++) buffer.bytes[i] = 0;

        for(u8 i=0; i<14; i++) buffer.words[i] = endianWord(buffer.words[i]);
        putBitCountAtTheEnd();

    }

    process();
}
