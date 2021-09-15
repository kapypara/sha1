#include "bit_tools.h"

u32 rotr(const u32 word, const u8 amount){
    return word >> amount | word << (32 - amount);

    /* same as rotl
    const u8 bit_count = 32; //countBits(word);

    if(amount & (bit_count-1)) // mask only allow values up to 31
    {
        return word >> amount | word << (bit_count - amount);
    }

    return word;
    */
}

u32 rotl(const u32 word, const u8 amount)  {
    return word << amount | word >> (32 - amount);

    /* slower but better handling for amount
    const u16 bit_count = countBits(word);

    if(amount & (bit_count-1)){
         return word << amount | word >> (bit_count - amount);
    }

    return word;
    */
}

void showBits(u64 bits){
    const u16 bit_count = countBits(u64) - 1;

    //std::cout << "byte count: " << bit_count << '\n';
    std::cout << "bits:";

    for(u16 i=0; i<=bit_count; i++){
        if(!(i%4)) std::cout << ' ';
        if(i==32) std::cout << "| ";
        std::cout << (bits >> (bit_count - i) & 1 );
    }
    std::cout << '\n';
}
