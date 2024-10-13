#ifndef ENCRYPT_H
#define ENCRYPT_H

std::vector<uint8_t> encrypt(const std::vector<std::vector<uint8_t>>& roundkeys,
                                const std::vector<uint8_t>& block,
                                    const std::vector<std::vector<uint8_t>>& sbox);

std::vector<uint8_t> addroundkey(const std::vector<uint8_t>& roundkey,
                                    const std::vector<uint8_t>& block);

std::vector<uint8_t> shiftrows(const std::vector<uint8_t>& block);

std::vector<uint8_t> mixcolumns(const std::vector<uint8_t>& block);

uint8_t galoismult(const uint8_t& colvalue, const uint8_t& fieldvalue);

#endif