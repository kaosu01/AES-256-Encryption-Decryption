#include <vector>
#include "decrypt.h"
#include "encrypt.h"
#include "keyop.h"

const std::vector<std::vector<uint8_t>> invgfield = {
    { 14, 11, 13, 9 }, 
    { 9, 14, 11, 13 },
    { 13, 9, 14, 11 },
    { 11, 13, 9, 14 }
};

std::vector<uint8_t> decrypt(const std::vector<std::vector<uint8_t>>& roundkeys,
                                const std::vector<uint8_t>& block,
                                    const std::vector<std::vector<uint8_t>>& invsbox)
{
    std::vector<uint8_t> decryptedblock;

    for(int round = 14; round >= 0; round--)
    {
        if(round == 14)
            decryptedblock = addroundkey(roundkeys[round], block);
        else if(round > 0 && round < 14)
        {
            decryptedblock = invshiftrows(decryptedblock);
            decryptedblock = invsboxsub(decryptedblock, invsbox);
            decryptedblock = addroundkey(roundkeys[round], decryptedblock);
            decryptedblock = invmixcolumns(decryptedblock);
        }
        else if(round == 0)
        {
            decryptedblock = invshiftrows(decryptedblock);
            decryptedblock = invsboxsub(decryptedblock, invsbox);
            decryptedblock = addroundkey(roundkeys[round], decryptedblock);
        }
    }
    return decryptedblock;
}

std::vector<uint8_t> invshiftrows(const std::vector<uint8_t>& block)
{
    std::vector<uint8_t> shifted(16);
    // Break Block into 4x4 Matrix
    for(int i = 0; i < 4; i++)
    {
        std::vector<uint8_t> row(block.begin() + (i * 4), block.begin() + (i * 4) + 4);

        // Compute j Rotations Every jth Row (1 Right Rotation = 7 Left Rotations, etc.)
        for(int j = 8; j > i % 8; j--)
            row = rotateword(row);

        // Insert row Into Main Vector
        for(int j = 0; j < 4; j++)
            shifted[(i * 4) + j] = row[j];
    }
    
    return shifted;
}
std::vector<uint8_t> invsboxsub(const std::vector<uint8_t>& block,
                                    const std::vector<std::vector<uint8_t>> invsbox)
{
    return sboxsub(block, invsbox);
}

std::vector<uint8_t> invmixcolumns(const std::vector<uint8_t>& block)
{
    std::vector<uint8_t> newblock(16);
    for(int i = 0; i < 4; i++)
    {
        newblock[(0 * 4) + i] = invgaloismult(block[(0 * 4) + i], invgfield[0][0]) ^
                                invgaloismult(block[(1 * 4) + i], invgfield[0][1]) ^
                                invgaloismult(block[(2 * 4) + i], invgfield[0][2]) ^
                                invgaloismult(block[(3 * 4) + i], invgfield[0][3]);

        newblock[(1 * 4) + i] = invgaloismult(block[(0 * 4) + i], invgfield[1][0]) ^
                                invgaloismult(block[(1 * 4) + i], invgfield[1][1]) ^
                                invgaloismult(block[(2 * 4) + i], invgfield[1][2]) ^
                                invgaloismult(block[(3 * 4) + i], invgfield[1][3]);

        newblock[(2 * 4) + i] = invgaloismult(block[(0 * 4) + i], invgfield[2][0]) ^
                                invgaloismult(block[(1 * 4) + i], invgfield[2][1]) ^
                                invgaloismult(block[(2 * 4) + i], invgfield[2][2]) ^
                                invgaloismult(block[(3 * 4) + i], invgfield[2][3]);

        newblock[(3 * 4) + i] = invgaloismult(block[(0 * 4) + i], invgfield[3][0]) ^
                                invgaloismult(block[(1 * 4) + i], invgfield[3][1]) ^
                                invgaloismult(block[(2 * 4) + i], invgfield[3][2]) ^
                                invgaloismult(block[(3 * 4) + i], invgfield[3][3]);
    }

    return newblock;
}

uint8_t invgaloismult(uint8_t colvalue, uint8_t matrixvalue)
{
    uint8_t ans = 0;

    for (int counter = 0; counter < 8; counter++) {
        if ((matrixvalue & 1) != 0) {
            ans ^= colvalue;
        }

        bool hi_bit_set = (colvalue & 0x80) != 0;
        colvalue <<= 1;
        if (hi_bit_set) {
            colvalue ^= 0x1B; /* x^8 + x^4 + x^3 + x + 1 */
        }
        matrixvalue >>= 1;
    }

    return ans;
}