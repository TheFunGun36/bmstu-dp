#include <random>
#include <vector>
#include <fstream>
#include "gf.h"
#include "sbox.h"
#include "aes.h"

constexpr const char* filename_in = "input";
constexpr const char* filename_out = "output";
constexpr const char* filename_out2 = "output2";
#define min(a, b) ((a < b) ? (a) : (b))

Char* generate_byte_array(size_t size) {
    static std::mt19937 g(27834);
    std::uniform_int_distribution<int> dist(0, 255);

    Char* result = new Char[size];

    for (int i = 0; i < size; i++)
        result[i] = dist(g);

    return result;
}

void block_xor(Char* a, const Char* b) {
    for (int i = 0; i < block_size; i++)
        a[i] ^= b[i];
}

void block_copy(Char* dst, const Char* src) {
    memcpy(dst, src, block_size);
}

void block_xor_to(Char* dst, const Char* a, const Char* b) {
    block_copy(dst, a);
    block_xor(dst, b);
}

void encrypt(const Char* iv, const Char* key) {
    using std::ios;
    std::ifstream in(filename_in, ios::binary);
    in.seekg(0, ios::end);
    size_t file_size = in.tellg();
    in.seekg(0);

    std::ofstream out(filename_out, ios::binary);

    // initialization vector
    Char* prev_block = new Char[block_size];
    block_copy(prev_block, iv);

    Aes aes;
    aes.set_key(key);

    // processing data blocks
    Char* plaintext = new Char[block_size];
    Char* ciphertext = new Char[block_size];

    // for first iteration, add 8 bytes of size to the beginning of the first block
    *(size_t*)plaintext = file_size;

    if (file_size <= 8) {
        in.read((char*)plaintext + sizeof(size_t), file_size);

        block_xor_to(ciphertext, plaintext, prev_block);
        aes.block_encrypt(ciphertext);
        out.write((char*)ciphertext, block_size);
        block_xor_to(prev_block, plaintext, ciphertext);
    }
    else {
        in.read((char*)plaintext + sizeof(size_t), block_size - sizeof(size_t));

        block_xor_to(ciphertext, plaintext, prev_block);
        aes.block_encrypt(ciphertext);
        out.write((char*)ciphertext, block_size);
        block_xor_to(prev_block, plaintext, ciphertext);

        size_t left_to_write = file_size - sizeof(size_t);

        while (left_to_write > block_size) {
            in.read((char*)plaintext, block_size);

            block_xor_to(ciphertext, plaintext, prev_block);
            aes.block_encrypt(ciphertext);
            out.write((char*)ciphertext, block_size);
            block_xor_to(prev_block, plaintext, ciphertext);

            left_to_write -= block_size;
        };

        in.read((char*)plaintext, left_to_write);

        block_xor_to(ciphertext, plaintext, prev_block);
        aes.block_encrypt(ciphertext);
        out.write((char*)ciphertext, block_size);
        block_xor_to(prev_block, plaintext, ciphertext);
    }

    delete[]plaintext;
    delete[]ciphertext;
    delete[]prev_block;
}

void decrypt(const Char* iv, const Char* key) {
    using std::ios;
    std::ifstream in(filename_out, ios::binary);
    in.seekg(0, ios::end);
    size_t blocks_count = in.tellg() / block_size;
    in.seekg(0);
    std::ofstream out(filename_out2, ios::binary);

    // initialization vector
    Char* prev_block = new Char[block_size];
    block_copy(prev_block, iv);

    Aes aes;
    aes.set_key(key);

    // processing data blocks
    Char* ciphertext = new Char[block_size];
    Char* plaintext = new Char[block_size];

    // for first iteration, add 8 bytes of size to the beginning of the first block

    in.read((char*)ciphertext, block_size);

    block_copy(plaintext, ciphertext);
    aes.block_decrypt(plaintext);
    block_xor(plaintext, prev_block);
    size_t filesize = *(size_t*)plaintext;
    out.write((char*)plaintext + sizeof(size_t), min(filesize, block_size - sizeof(size_t)));
    
    if (blocks_count > 1) {
        block_xor_to(prev_block, ciphertext, plaintext);

        for (int i = 1; i < blocks_count - 1; i++) {
            in.read((char*)ciphertext, block_size);

            block_copy(plaintext, ciphertext);
            aes.block_decrypt(plaintext);
            block_xor(plaintext, prev_block);
            out.write((char*)plaintext, block_size);
            block_xor_to(prev_block, ciphertext, plaintext);
        }

        in.read((char*)ciphertext, block_size);

        block_copy(plaintext, ciphertext);
        aes.block_decrypt(plaintext);
        block_xor(plaintext, prev_block);

        
        out.write((char*)plaintext, (filesize + sizeof(size_t)) - block_size * (blocks_count - 1));
    }

    delete[]ciphertext;
    delete[]plaintext;
    delete[]prev_block;
}

int main(int argc, Char* argv[]) {
    // cb268328d0b91fc62d108fa0508fb23d
    Char* iv = generate_byte_array(block_size);

    // d45e5a0fe4bfbfd7b1359ce7dd2d63be
    Char* key = generate_byte_array(block_size);

    encrypt(iv, key);
    decrypt(iv, key);

    return 0;
}
