#include <cmath>
#include <vector>
#include <string>
#include "blocks.h"
#include <iostream>
#include <iomanip>

std::vector<std::vector<uint8_t>> getblocks(const std::string& message)
{
    // Each Block Will Contain 16 Bytes (Combination of 4 Words)
    int blockbytes = 16;

    // Initialize A 2-D Vector, The Row Being The # of Blocks, The Column Being Each Byte In The Block
    std::vector<std::vector<uint8_t>> blocks(ceil(static_cast<double>(message.size()) / blockbytes), std::vector<uint8_t>(16));

    // Do CMS Padding If Necessary
    int paddingsize = blockbytes - (message.size() % blockbytes);
    if(paddingsize == blockbytes)
        paddingsize = 0;
    std::string paddedstring = message + std::string(paddingsize, static_cast<char>(paddingsize));

    // Convert the String Into Byte Form
    for(int i = 0; i < blocks.size(); i++)
    {
        std::string currentblock = paddedstring.substr(i * blockbytes, blockbytes);
        for(int j = 0; j < currentblock.size(); j++)
            blocks[i][j] = strtobytes(currentblock[j]);
    }

    return blocks;
}

std::vector<std::vector<uint8_t>> getblocks(const std::vector<uint8_t>& ciphertext)
{
    std::vector<std::vector<uint8_t>> blocks(ciphertext.size() / 16, std::vector<uint8_t>(16));

    for(int i = 0; i < ciphertext.size() / 16; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            blocks[i][j] = ciphertext[(i * 16) + j];
        }
    }
    return blocks;
}

uint8_t strtobytes(const char& character)
{
    return static_cast<uint8_t>(character);
}

std::string removepadding(const std::vector<uint8_t> hexmessage)
{
    // Get Padding Size
    uint8_t padding = static_cast<uint8_t>(hexmessage.back());

    if(padding < 16 || padding < hexmessage.size())
        return std::string(hexmessage.begin(), hexmessage.end() - padding);
    else
        return "";

}