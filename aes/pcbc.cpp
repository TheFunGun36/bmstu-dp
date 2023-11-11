#include "pcbc.h"
#include <memory>

Pcbc::Pcbc(const Char* key)
    : m_aes(key) {
}

Pcbc::Pcbc(const Char* key, const Char* iv)
    : Pcbc(key) {
    set_initial_vector(iv);
}

void Pcbc::set_initial_vector(const Char* iv) {
    memcpy(m_iv, iv, block_size);
    reset();
}

void Pcbc::reset() {
    memcpy(m_prev_block, m_iv, block_size);
}

static void block_xor(Char* a, const Char* b) {
    long long* aa = (long long*)a;
    const long long* bb = (const long long*)b;
    for (int i = 0; i < block_size / sizeof(long long); i++)
        aa[i] ^= bb[i];
}

static void block_copy(Char* dst, const Char* src) {
    memcpy(dst, src, block_size);
}

static void block_xor_to(Char* dst, const Char* a, const Char* b) {
    block_copy(dst, a);
    block_xor(dst, b);
}

void Pcbc::encrypt(Char* block) {
    // both buffer and block contain plaintext
    Char buffer[block_size];
    block_copy(buffer, block);

    // encrypting block
    block_xor(block, m_prev_block);
    m_aes.encrypt(block);

    // store new xored vector
    block_xor_to(m_prev_block, block, buffer);
}

void Pcbc::decrypt(Char* block) {
    // both buffer and block contain plaintext
    Char buffer[block_size];
    block_copy(buffer, block);

    // decrypting block
    m_aes.decrypt(block);
    block_xor(block, m_prev_block);

    // store new xored vector
    block_xor_to(m_prev_block, block, buffer);
}
