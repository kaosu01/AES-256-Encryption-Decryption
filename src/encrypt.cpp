#include <vector>
#include "encrypt.h"
#include "keyop.h"

// References:
// https://en.wikipedia.org/wiki/Rijndael_MixColumns

const std::vector<std::vector<uint8_t>> gfield = {
    { 2, 3, 1, 1 },
    { 1, 2, 3, 1 },
    { 1, 1, 2, 3 },
    { 3, 1, 1, 2 }
};

std::vector<uint8_t> encrypt(const std::vector<std::vector<uint8_t>>& roundkeys,
                                const std::vector<uint8_t>& block,
                                    const std::vector<std::vector<uint8_t>>& sbox)
{
    std::vector<uint8_t> cipherblock;
    for(int round = 0; round < 15; round++)
    {
        // For The Initial Round, ONLY Execute addroundkey
        if(round == 0)
            cipherblock = addroundkey(roundkeys[round], block);
            
        else if(round > 0 && round < 14)
        {
            cipherblock = sboxsub(cipherblock, sbox);
            cipherblock = shiftrows(cipherblock);
            cipherblock = mixcolumns(cipherblock);
            cipherblock = addroundkey(roundkeys[round], cipherblock);
        }
        else if(round == 14)
        {
            cipherblock = sboxsub(cipherblock, sbox);
            cipherblock = shiftrows(cipherblock);
            cipherblock = addroundkey(roundkeys[round], cipherblock);
        }
    }
    return cipherblock;
}

std::vector<uint8_t> addroundkey(const std::vector<uint8_t>& roundkey, const std::vector<uint8_t>& block)
{
    // Initialize A Vector For Operation
    std::vector<uint8_t> keyxor(block.size());
    for(int i = 0; i < keyxor.size(); i++)
    {
        keyxor[i] = block[i] ^ roundkey[i];
    }

    return keyxor;
}

std::vector<uint8_t> shiftrows(const std::vector<uint8_t>& block)
{
    std::vector<uint8_t> shifted(16);
    // Break Block into 4x4 Matrix
    for(int i = 0; i < 16; i += 4)
    {
        std::vector<uint8_t> row(block.begin() + i, block.begin() + i + 4);

        // Compute j Rotations Every jth Row
        for(int j = 0; j < i/4; j++)
            row = rotateword(row);

        // Insert row Into Main Vector
        for(int j = 0; j < 4; j++)
            shifted[i + j] = row[j];
    }
    
    return shifted;
}

std::vector<uint8_t> mixcolumns(const std::vector<uint8_t>& block)
{
    std::vector<uint8_t> newblock(16);
    for(int i = 0; i < 4; i++)
    {
        newblock[(0 * 4) + i] = galoismult(block[(0 * 4) + i], gfield[0][0]) ^
                                galoismult(block[(1 * 4) + i], gfield[0][1]) ^
                                galoismult(block[(2 * 4) + i], gfield[0][2]) ^
                                galoismult(block[(3 * 4) + i], gfield[0][3]);

        newblock[(1 * 4) + i] = galoismult(block[(0 * 4) + i], gfield[1][0]) ^
                                galoismult(block[(1 * 4) + i], gfield[1][1]) ^
                                galoismult(block[(2 * 4) + i], gfield[1][2]) ^
                                galoismult(block[(3 * 4) + i], gfield[1][3]);

        newblock[(2 * 4) + i] = galoismult(block[(0 * 4) + i], gfield[2][0]) ^
                                galoismult(block[(1 * 4) + i], gfield[2][1]) ^
                                galoismult(block[(2 * 4) + i], gfield[2][2]) ^
                                galoismult(block[(3 * 4) + i], gfield[2][3]);

        newblock[(3 * 4) + i] = galoismult(block[(0 * 4) + i], gfield[3][0]) ^
                                galoismult(block[(1 * 4) + i], gfield[3][1]) ^
                                galoismult(block[(2 * 4) + i], gfield[3][2]) ^
                                galoismult(block[(3 * 4) + i], gfield[3][3]);
    }

    return newblock;
}

uint8_t galoismult(const uint8_t& colvalue, const uint8_t& fieldvalue)
{
    uint8_t ans = colvalue;

    if(fieldvalue == 1)
        return ans;
    else if(fieldvalue == 2)
        return (ans & 0x80) ? ((ans << 1) ^ 0x1b) : (ans << 1);
    else
        return ((ans & 0x80) ? ((ans << 1) ^ 0x1b) : (ans << 1)) ^ ans;
}