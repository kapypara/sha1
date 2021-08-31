#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#include <utility>
#include <vector>
#include "bit_tools.h"

class SHA1 {

    u32 h0 = 0x67452301;
    u32 h1 = 0xEFCDAB89;
    u32 h2 = 0x98BADCFE;
    u32 h3 = 0x10325476;
    u32 h4 = 0xC3D2E1F0;

    u64 ml = 0; // message length

    std::vector<u32> message, words;

public:
    explicit SHA1(std::vector<u32> input) : message(std::move(input)) {}
    void run();
    void print();

};

#endif //MY_SHA1_SHA1_H
