#include "aes.h"
#include "sbox.h"
#include <memory>

void Aes::block_encrypt(Char* block) {
    add_round_key(block, 0);

    for (int i = 1; i < rounds; i++) {
        sub_bytes(block);
        shift_rows(block);
        mix_columns(block);
        add_round_key(block, i);
    }
    
    sub_bytes(block);
    shift_rows(block);
    add_round_key(block, rounds);
}

void Aes::block_decrypt(Char* block) {
    add_round_key(block, rounds);

    for (int i = rounds - 1; i > 0; i--) {
        shift_rows(block, true);
        sub_bytes(block, true);
        add_round_key(block, i);
        mix_columns(block, true);
    }
    
    shift_rows(block, true);
    sub_bytes(block, true);
    add_round_key(block, 0);
}

void Aes::set_key(const Char* key) {
    memcpy(m_key, key, block_size);
    m_block_index = 1;
    m_rc = 1;
    expand_key();
}

void Aes::sub_bytes(Char* block, bool inv) {
    for (int i = 0; i < block_size; i++)
        block[i] = inv ? inv_sbox[block[i]] : sbox[block[i]];
}

void Aes::shift(Char* arr, size_t size, size_t times) {
    for (int t = 0; t < times; t++) {
        Char buf = arr[0];
        for (int i = 1; i < size; i++)
            arr[i - 1] = arr[i];
        arr[size - 1] = buf;
    }
}

void Aes::shift_rows(Char* block, bool inv) {
    for (int i = 1; i <= 3; i++)
        shift(block + i * 4, 4, inv ? 4 - i : i);
}

static Char mul02(Char value) {
    Char res = value << 1;
    if (value >= 0x80)
        res ^= 0x1b;
    return res % 0x100;
}

static Char mul09(Char value) {
    //return mul03(value) ^ mul03(value) ^ mul03(value) - works wrong, I don't know why
    return mul02(mul02(mul02(value))) ^ value;
}

static Char mul0b(Char value) {
    //return mul09(value) ^ mul02(value)
    return mul02(mul02(mul02(value))) ^ mul02(value) ^ value;
}

static Char mul0d(Char value) {
    //return mul0b(value) ^ mul02(value)
    return mul02(mul02(mul02(value))) ^ mul02(mul02(value)) ^ value;
}

static Char mul0e(Char value) {
    //return mul0d(value) ^ value
    return mul02(mul02(mul02(value))) ^ mul02(mul02(value)) ^ mul02(value);
}

void Aes::mix_column(Char* r, bool inv) {
    //const Char bytes[] = {0x02, 0x03, 0x01, 0x01};

    Char a[4];
    memcpy(a, r, 4);

    if (!inv) {
        Char b[4];

        for (Char c = 0; c < 4; c++) {
            b[c] = r[c] << 1;
            Char h = r[c] >> 7;
            b[c] ^= h * 0x1B;
        }

        r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
        r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
        r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
        r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
    }
    else {
        r[0] = mul0e(a[0]) ^ mul0b(a[1]) ^ mul0d(a[2]) ^ mul09(a[3]);
        r[1] = mul09(a[0]) ^ mul0e(a[1]) ^ mul0b(a[2]) ^ mul0d(a[3]);
        r[2] = mul0d(a[0]) ^ mul09(a[1]) ^ mul0e(a[2]) ^ mul0b(a[3]);
        r[3] = mul0b(a[0]) ^ mul0d(a[1]) ^ mul09(a[2]) ^ mul0e(a[3]);
    }
}

void Aes::mix_columns(Char* block, bool inv) {
    for (int i = 0; i < 4; i++) {
        Char col[4];
        for (int j = 0; j < 4; j++)
            col[j] = block[j * 4 + i];
        mix_column(col, inv);
        for (int j = 0; j < 4; j++)
            block[j * 4 + i] = col[j];
    }
}

void Aes::next_key_exp(Char* block) {
    Char new_key[block_size];

    for (int col = 0; col < 4; col++) {
        if (col % 4 == 0) {
            // циклический сдвиг предыдущего слова
            for (int i = 0; i < 4; i++)
                new_key[i * 4] = m_key[3 + ((i + 1) % 4) * 4];

            // SBox его
            for (int i = 0; i < 4; i++)
                new_key[i * 4] = sbox[new_key[i * 4]];

            // xor с первым столбцом предыдущего ключа
            for (int i = 0; i < 4; i++)
                new_key[i * 4] ^= m_key[i * 4];

            // xor с rcon
            new_key[0] ^= m_rc;
        }
        else {

            for (int i = 0; i < 4; i++)
                new_key[col + i * 4] = m_key[col + i * 4] ^ new_key[col - 1 + i * 4];
        }
    }

    memcpy(m_key, new_key, block_size);
    m_block_index++;
    m_rc = next_rc();
}

void Aes::add_round_key(Char* block, size_t key_index) {
    Char *base = m_key + key_index * block_size;
    for (int i = 0; i < block_size; i++)
        block[i] ^= base[i];
}

void Aes::expand_key() {
    for (int round = 1; round < rounds; round++) {
        for (int col = 0; col < 4; col++) {
            const int blk = round * block_size;

            if (col % 4 == 0) {
                const int cur = round * block_size;
                const int prev = (round - 1) * block_size + 3;
                const int pprev = (round - 1) * block_size;
                // циклический сдвиг предыдущего слова
                for (int i = 0; i < 4; i++)
                    m_key[cur + i * 4] = m_key[prev + ((i + 1) % 4) * 4];

                // SBox его
                for (int i = 0; i < 4; i++)
                    m_key[cur + i * 4] = sbox[m_key[cur + i * 4]];

                // xor с первым столбцом предыдущего ключа
                for (int i = 0; i < 4; i++)
                    m_key[cur + i * 4] ^= m_key[pprev + i * 4];

                // xor с rcon
                m_key[cur] ^= m_rc;
            }
            else {
                const int cur = round * block_size + col;
                const int prev = round * block_size + col - 1;
                const int pprev = (round - 1) * block_size + col;
                for (int i = 0; i < 4; i++)
                    m_key[cur + i * 4] = m_key[pprev + i * 4] ^ m_key[prev + i * 4];
            }
        }
    }
}

Char Aes::next_rc() {
    Char result = m_rc << 1;
    if (m_rc >= 0x80)
        result ^= 0x11b;
    return result;
}
