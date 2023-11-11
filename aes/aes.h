#pragma once
constexpr size_t block_size = 16;
constexpr size_t rounds = 10;

using Char = unsigned char;

class Aes {
public:
    void block_encrypt(Char* block);
    void block_decrypt(Char* block);
    void set_key(const Char* key);

private:
    static void shift(Char* arr, size_t size, size_t times = 1);
    void sub_bytes(Char* block, bool inv = false);
    void shift_rows(Char* block, bool inv = false);
    void mix_column(Char* r, bool inv = false);
    void mix_columns(Char* block, bool inv = false);
    void next_key_exp(Char* block);
    void add_round_key(Char* block, size_t key_index);
    void expand_key();

    Char next_rc();

    int m_block_index = 1;
    Char m_rc = 1;
    Char m_key[block_size * (rounds + 1)];
};
