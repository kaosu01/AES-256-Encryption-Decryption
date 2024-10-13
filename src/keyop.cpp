#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include "keyop.h"


// Source Mentioned In getroundkeys Function
std::vector<uint8_t> rcon = {
    {0x01, 0x02, 0x04, 0x08, 0x10,
        0x20, 0x40, 0x80, 0x1b, 0x36}
};

std::vector<uint8_t> getrandkey()
{
    // Initialize Vector to Store 32 Bytes (256 Bits)
    std::vector<uint8_t> key(32);
    std::random_device rd;

    for(auto& byte : key)
        byte = rd();

    return key;
}

std::vector<std::vector<uint8_t>> getroundkeys(const std::vector<uint8_t>& key, const std::vector<std::vector<uint8_t>>& sbox)
{
    // Source: https://en.wikipedia.org/wiki/AES_key_schedule

    int wordssize = 60;         // # of Words
    int numroundkeys = 15;      // # of Round Keys
    int keywordslength = 8;     // # of Words Generated w/ Exact Random Key
    int wordbytecount = 4;      // # of Bytes In A Word
    int keybytecount = 16;      // # of Bytes In A Round Key

    // 2-D Vector Used to Store Round Keys
    std::vector<std::vector<uint8_t>> roundkeys(numroundkeys, std::vector<uint8_t>(keybytecount));

    // Vector to Store Each Individual Word For Easier Computations
    std::vector<std::vector<uint8_t>> words(wordssize, std::vector<uint8_t>(wordbytecount));

    for(int round = 0; round < words.size(); round++)
    {
        // For the 1st 8 Words, Simply Insert Them
        /*
            words[0] --> key[0..3], words[1] --> key[4..7], etc.
        */
        if(round < keywordslength)
        {
            for(int i = 0; i < wordbytecount; i++)
            {
                words[round][i] = key[round * wordbytecount + i];
            }
        }

        // Perform Rotation, Substitution, and XOR Operations
        else if(round >= keywordslength && round % keywordslength == 0)
        {
            words[round] = sboxsub(rotateword(words[round - 1]), sbox);
            words[round][0] = rconword(words[round][0], (round / keywordslength) - 1);
            words[round] = xorwords(words[round - keywordslength], words[round]);
        }

        // Perform Only Substitution and XOR Operations
        else if(round >= keywordslength && round % keywordslength == 4)
        {
            words[round] = sboxsub(words[round - 1], sbox);
            words[round] = xorwords(words[round - keywordslength], words[round]);
        }

        // Perform Only A Single XOR Operation
        else
        {
            words[round] = xorwords(words[round - keywordslength], words[round - 1]);
        }
    }

    // Combine 4 Words At A Time to Make A Round Key
    // Iterate Through to Get 15 Round Keys
    for(int i = 0; i < numroundkeys; i++)
        // Iterate Through to Get the Next 4 Words
        for(int j = 0; j < 4; j++)
            // Iterate Through to Get Each Byte From Those Words
            for(int k = 0; k < 4; k++)
                roundkeys[i][(j * 4) + k] = words[(i * 4) + j][k];

    return roundkeys;
}

// Left Shift the Bits, Carry Over Shifted Bits to the Right
std::vector<uint8_t> rotateword(std::vector<uint8_t> word)
{
    // Left Shift the Bits (Left Shift Vector), Carry Over To Right
    uint8_t temp = word[0];
    for(int i = 0; i < word.size(); i++)
        word[i] = word[i + 1];
    word[word.size() - 1] = temp;

    return word;
}

std::vector<uint8_t> sboxsub(std::vector<uint8_t> word, const std::vector<std::vector<uint8_t>>& sbox)
{
    for(int i = 0; i < word.size(); i++)
    {
        uint8_t row = (word[i] >> 4) & 0x0f;
        uint8_t column = word[i] & 0x0f;

        word[i] = sbox[row][column];
    }

    return word;
}

std::vector<uint8_t> xorwords(const std::vector<uint8_t>& w1, const std::vector<uint8_t>& w2)
{
    std::vector<uint8_t> ans(4);
    for(int i = 0; i < w1.size(); i++)
        ans[i] = w1[i] ^ w2[i];

    return ans;
}

uint8_t rconword(uint8_t byte, uint8_t i)
{
    return byte ^ rcon[i];
}