#ifndef DECRYPT_H
#define DECRYPT_H

std::vector<uint8_t> decrypt(const std::vector<std::vector<uint8_t>>& roundkeys,
                                const std::vector<uint8_t>& block,
                                    const std::vector<std::vector<uint8_t>>& invsbox);

std::vector<uint8_t> invmixcolumns(const std::vector<uint8_t>& block);
std::vector<uint8_t> invshiftrows(const std::vector<uint8_t>& block);
std::vector<uint8_t> invsboxsub(const std::vector<uint8_t>& block,
                                    const std::vector<std::vector<uint8_t>> invsbox);
uint8_t invgaloismult(uint8_t colvalue, uint8_t fieldvalue);

#endif