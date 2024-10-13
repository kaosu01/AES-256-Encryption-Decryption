#ifndef BLOCKS_H
#define BLOCKS_H

std::vector<std::vector<uint8_t>> getblocks(const std::string& message);
std::vector<std::vector<uint8_t>> getblocks(const std::vector<uint8_t>& ciphertext);
uint8_t strtobytes(const char& character);
std::string removepadding(const std::vector<uint8_t> hexmessage);

#endif