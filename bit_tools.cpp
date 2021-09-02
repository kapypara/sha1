#include "bit_tools.h"

u32 rotr(const u32 word, const u16 amount){
    const u16 bit_count = countBits(word);

    if(amount & (bit_count-1)) // like `amount %= bit_count` but faster, I think it works because 32 is a base2
    {
        return word >> amount | word << (bit_count - amount);
    }

    return word;
}

u32 rotl(const u32 word, const u16 amount)  {
    const u16 bit_count = countBits(word);

    if(amount & (bit_count-1)){
         return word << amount | word >> (bit_count - amount);
    }

    return word;
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
