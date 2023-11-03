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

void SHA1::endianWord(u8 i){
#if BYTE_ORDER == LITTLE_ENDIAN
    buffer.words[i] =  (rotl(buffer.words[i],24 ) & 0xFF00FF00) | (rotl(buffer.words[i],8) & 0x00FF00FF);
#elif BYTE_ORDER == BIG_ENDIAN
    ;
#endif
}

void SHA1::putBitCountAtTheEnd(){
    // this may be broken in BIG_ENDIAN. maybe flipping the 14 & 15 will fix it in that case.
    buffer.words[14] = ml>>32 ;
    buffer.words[15] = ml ;
}

inline u32 Ch(u32 const& x, u32 const& y, u32 const& z){
    return ( x & (y^z) ) ^ z ;
    return (x & y) ^ (~x & z);
}

inline u32 Parity(u32 const& x, u32 const& y, u32 const& z){
    return x ^ y ^ z;
}

inline u32 Maj(u32 const& x, u32 const& y, u32 const& z){
    return ( (x|y) & z ) | (x&y) ;
    return (x & y) ^ (x & z) ^ (y & z);
}

inline u32 * getWord(u32 *w, const u8 i){

    const u8 s = i & 0xf; // mask for values above 16

    w[s] = w[ (s+13) & 0xf ] ^ w[ (s+8) & 0xf ] ^ w[ (s+2) & 0xf ] ^ w[s];
    w[s] = rotl( w[s], 1);

    return &w[s];
}

inline void r0(u32 *a, u32 *b, u32 *c, u32 *d, u32 *e, const u32 *w, u8 const& t){

    static const u32 k = 0x5A827999;

    u32 temp;
    u32 f0, f1;

    f0 = Ch(*b,*c,*d);
    temp = rotl(*a, 5) + f0 + *e + k + w[t];

    *e = *c;

    *b = rotl(*b, 30);
    *c = rotl(*a, 30);

    f1 = Ch(*a,*b,*e); // using e since it became c
    *a = rotl(temp, 5) + f1 + *d + k + w[t+1];

    *d = *b;
    *b = temp;
}

inline void r1(u32 *a, u32 *b, u32 *c, u32 *d, u32 *e, u32 *w, u8 const& t){

    static const u32 k = 0x5A827999;

    u32 temp;
    u32 f0, f1; // ft and ft+1

    f0 = Ch(*b,*c,*d);
    temp = rotl(*a, 5) + f0 + *e + k + *getWord(w, t);

    *e = *c;

    *b = rotl(*b, 30);
    *c = rotl(*a, 30);

    f1 = Ch(*a,*b,*e);
    *a = rotl(temp, 5) + f1 + *d + k + *getWord(w, t+1);

    *d = *b;
    *b = temp;
    //updateVars(a, b, c, d, e, temp);

    /*
    static int i = t+1;
    std::cout << std::dec << "t" << i << std::hex <<
        " a: " << *a << ", b: " << *b << ", c: " << *c << ", d: " << *d << ", e: " << *e << '\n';
    i+=2;
    /**/
}

inline void r2(u32 *a, u32 *b, u32 *c, u32 *d, u32 *e, u32 *w, u8 const& t){

    static const u32 k = 0x6ED9EBA1;

    u32 temp;
    u32 f0, f1;

    f0 = Parity(*b,*c,*d);
    temp = rotl(*a, 5) + f0 + *e + k + *getWord(w, t);

    *e = *c;

    *b = rotl(*b, 30);
    *c = rotl(*a, 30);

    f1 = Parity(*a,*b,*e);
    *a = rotl(temp, 5) + f1 + *d + k + *getWord(w, t+1);

    *d = *b;
    *b = temp;
}

inline void r3(u32 *a, u32 *b, u32 *c, u32 *d, u32 *e, u32 *w, u8 const& t){

    static const u32 k = 0x8F1BBCDC;

    u32 temp;
    u32 f0, f1;

    f0 = Maj(*b,*c,*d);
    temp = rotl(*a, 5) + f0 + *e + k + *getWord(w, t);

    *e = *c;

    *b = rotl(*b, 30);
    *c = rotl(*a, 30);

    f1 = Maj(*a,*b,*e);
    *a = rotl(temp, 5) + f1 + *d + k + *getWord(w, t+1);

    *d = *b;
    *b = temp;
}

inline void r4(u32 *a, u32 *b, u32 *c, u32 *d, u32 *e, u32 *w, u8 const& t){

    static const u32 k = 0xCA62C1D6;

    u32 temp;
    u32 f0, f1;

    f0 = Parity(*b,*c,*d);
    temp = rotl(*a, 5) + f0 + *e + k + *getWord(w, t);

    *e = *c;

    *b = rotl(*b, 30);
    *c = rotl(*a, 30);

    f1 = Parity(*a,*b,*e);
    *a = rotl(temp, 5) + f1 + *d + k + *getWord(w, t+1);

    *d = *b;
    *b = temp;
}

void SHA1::process() {

    u32 * word = buffer.words;

    /*
    std::cout << "[ SHA1::process ] listing words!, data in hex\n";
    for (u8 t = 0; t < 16; t++) {
        std::cout << std::dec << 't' << (int) t << ": " << std::hex << word[t] << std::dec << '\n';
        showBits(word[t]);
    }
    std::cout << "[ SHA1::process ] done listing words!\n";
    /**/

    u32 var_a = h0;
    u32 var_b = h1;
    u32 var_c = h2;
    u32 var_d = h3;
    u32 var_e = h4;

    u32 *a=&var_a, *b=&var_b, *c=&var_c, *d=&var_d, *e=&var_e;

    r0(a, b, c, d, e, word, 0);
    r0(a, b, c, d, e, word, 2);
    r0(a, b, c, d, e, word, 4);
    r0(a, b, c, d, e, word, 6);
    r0(a, b, c, d, e, word, 8);
    r0(a, b, c, d, e, word, 10);
    r0(a, b, c, d, e, word, 12);
    r0(a, b, c, d, e, word, 14);

    r1(a, b, c, d, e, word, 16);
    r1(a, b, c, d, e, word, 18);

    r2(a, b, c, d, e, word, 20);
    r2(a, b, c, d, e, word, 22);
    r2(a, b, c, d, e, word, 24);
    r2(a, b, c, d, e, word, 26);
    r2(a, b, c, d, e, word, 28);
    r2(a, b, c, d, e, word, 30);
    r2(a, b, c, d, e, word, 32);
    r2(a, b, c, d, e, word, 34);
    r2(a, b, c, d, e, word, 36);
    r2(a, b, c, d, e, word, 38);

    r3(a, b, c, d, e, word, 40);
    r3(a, b, c, d, e, word, 42);
    r3(a, b, c, d, e, word, 44);
    r3(a, b, c, d, e, word, 46);
    r3(a, b, c, d, e, word, 48);
    r3(a, b, c, d, e, word, 50);
    r3(a, b, c, d, e, word, 52);
    r3(a, b, c, d, e, word, 54);
    r3(a, b, c, d, e, word, 56);
    r3(a, b, c, d, e, word, 58);

    r4(a, b, c, d, e, word, 60);
    r4(a, b, c, d, e, word, 62);
    r4(a, b, c, d, e, word, 64);
    r4(a, b, c, d, e, word, 66);
    r4(a, b, c, d, e, word, 68);
    r4(a, b, c, d, e, word, 70);
    r4(a, b, c, d, e, word, 72);
    r4(a, b, c, d, e, word, 74);
    r4(a, b, c, d, e, word, 76);
    r4(a, b, c, d, e, word, 78);

    h0 += *a;
    h1 += *b;
    h2 += *c;
    h3 += *d;
    h4 += *e;

    //for(u8 i=0; i<16; i++) buffer.words[i] = 0;
}

void SHA1::update(const char *str, u64 length){

    ml += (length<<3); // converting bytes into bits, right bit shift by three is the equivalent of multiplying by 8
    u64 byte_added;
    u64 byte_passed = 0;

    if ((length + bytes_in_buffer) > 63) {

        byte_added = 64 - bytes_in_buffer;

        memcpy(&buffer.bytes[bytes_in_buffer], &str[byte_passed], byte_added);

        length -= byte_added;
        byte_passed += byte_added;

        bytes_in_buffer = 0;

        for (u8 i = 0; i < 16; i++) endianWord(i);
        process();

        while (length > 63) {

            memcpy(&buffer.bytes[0], &str[byte_passed], 64);

            length -= 64;
            byte_passed += 64;

            for (u8 i = 0; i < 16; i++) endianWord(i);
            process();
        }
    }

    memcpy(&buffer.bytes[bytes_in_buffer] , &str[byte_passed], length);

    bytes_in_buffer += length;

    //print();
}

void SHA1::final() {

    buffer.bytes[bytes_in_buffer] = 0x80 ;
    memset(&buffer.bytes[bytes_in_buffer + 1], 0, 63 - bytes_in_buffer);

    if (bytes_in_buffer > 55) {

        for(u8 i=0; i<16; i++) endianWord(i);
        process();

        memset(&buffer, 0, 64);

    } else {

        for(u8 i=0; i<14; i++) endianWord(i);

    }

    putBitCountAtTheEnd();
    process();
}
