#ifndef MY_SHA1_BIT_TOOLS_H
#define MY_SHA1_BIT_TOOLS_H

#include <iostream>
#include <climits>

#define u8  uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define i8  int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t

#define countBits(bits_) sizeof(bits_) * CHAR_BIT

#define bit_tools_use_macro 1
#ifdef bit_tools_use_macro

#define rotr(word, amount) ( word >> amount | word << (32 - amount) )
#define rotl(word, amount) ( word << amount | word >> (32 - amount) )

#else
// rotate bits right and left
u32 rotr(u32 word, u8 amount);
u32 rotl(u32 word, u8 amount);

#endif

// show bits for debugging
void showBits(u64 bits);

#endif //MY_SHA1_BIT_TOOLS_H
