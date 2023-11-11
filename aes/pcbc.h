#pragma once
#include "aes.h"

class Pcbc : public EncryptionEngine {
public:
    Pcbc(const Char* key);
    Pcbc(const Char* key, const Char* iv);
    void set_initial_vector(const Char* iv);

    void reset();
    void encrypt(Char* block);
    void decrypt(Char* block);

private:

    Char m_iv[block_size];
    Char m_prev_block[block_size];
    Aes m_aes;
};
