#pragma once
#include "encryption_engine.h"
constexpr size_t block_size = 16;
constexpr size_t rounds = 10;

class Aes : public EncryptionEngine {
public:
    Aes(const Char* key);

    void encrypt(Char* block) override;
    void decrypt(Char* block) override;
    void reset() override;

private:
    void sub_bytes(Char* block, bool inv = false);
    void shift_rows(Char* block, bool inv = false);
    void mix_column(Char* r, bool inv = false);
    void mix_columns(Char* block, bool inv = false);
    void add_round_key(Char* block, size_t key_index);
    void expand_key();

    Char next_rc(Char rc);

    Char m_key[block_size * (rounds + 1)];
};
