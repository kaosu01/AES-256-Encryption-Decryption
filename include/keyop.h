#ifndef KEYOP_H
#define KEYOP_H

std::vector<uint8_t> getrandkey();
std::vector<std::vector<uint8_t>> getroundkeys(const std::vector<uint8_t>& key,
                                                const std::vector<std::vector<uint8_t>>& sbox);

std::vector<uint8_t> rotateword(std::vector<uint8_t> word);
std::vector<uint8_t> sboxsub(std::vector<uint8_t> word, const std::vector<std::vector<uint8_t>>& sbox);
std::vector<uint8_t> xorwords(const std::vector<uint8_t>& w1, const std::vector<uint8_t>& w2);
uint8_t rconword(uint8_t word, uint8_t i);

#endif