#include <random>
#include <vector>
#include <fstream>
#include "sbox.h"
#include "aes.h"
#include "pcbc.h"

constexpr const char* filename_in = "input";
constexpr const char* filename_out = "output";
constexpr const char* filename_out2 = "output2";
#define min(a, b) (((a) < (b)) ? (a) : (b))

Char* generate_byte_array(size_t size) {
    static std::mt19937 g(27834);
    std::uniform_int_distribution<int> dist(0, 255);

    Char* result = new Char[size];

    for (int i = 0; i < size; i++)
        result[i] = dist(g);

    return result;
}

void copy_first(size_t bytes) {
    using std::ios;
    std::ifstream in(filename_in, ios::binary);
    std::fstream out(filename_out, ios::in | ios::out | ios::binary);
    char* buff = new char[bytes];
    in.read(buff, bytes);
    out.write(buff, bytes);
    delete[]buff;
}

bool encrypt(EncryptionEngine &engine, bool include_file_size = true) {
    using std::ios;
    std::ifstream in(filename_in, ios::binary);
    std::ofstream out(filename_out, ios::binary);

    if (!in.is_open() || !out.is_open())
        return false;

    in.seekg(0, ios::end);
    size_t file_size = in.tellg();
    size_t left_to_read = file_size;
    in.seekg(0);

    // processing data blocks
    Char buffer[block_size];

    size_t readed = 0;
    if (!include_file_size) {
        readed = min(file_size, block_size);
    }
    else {
        // Using source file size, not the result one
        *(size_t*)buffer = file_size;
        readed = min(file_size, block_size - sizeof(size_t));
    }

    in.read((char*)buffer + sizeof(size_t), readed);
    left_to_read -= readed;

    engine.encrypt(buffer);
    out.write((char*)buffer, block_size);

    while (left_to_read > block_size) {
        in.read((char*)buffer, block_size);
        engine.encrypt(buffer);
        out.write((char*)buffer, block_size);
        left_to_read -= block_size;
    }

    if (left_to_read > 0) {
        memset(buffer, 0, block_size);  //safety!
        in.read((char*)buffer, left_to_read);
        engine.encrypt(buffer);
        out.write((char*)buffer, block_size);
    }
}

void decrypt(EncryptionEngine &engine, bool include_file_size = true) {
    using std::ios;
    std::ifstream in(filename_out, ios::binary);
    std::ofstream out(filename_out2, ios::binary);

    in.seekg(0, ios::end);
    size_t file_size = in.tellg();
    size_t blocks_count = file_size / block_size;
    size_t blocks_left = blocks_count - 1;
    in.seekg(0);

    // processing data blocks
    Char buffer[block_size];

    in.read((char*)buffer, block_size);
    engine.decrypt(buffer);

    if (include_file_size) {
        file_size = *(size_t*)buffer;
        const size_t to_write = min(block_size - sizeof(size_t), file_size);
        out.write((char*)buffer + sizeof(size_t), to_write);
    }
    else {
        out.write((char*)buffer, block_size);
    }

    while (blocks_left > 1) {
        in.read((char*)buffer, block_size);
        engine.decrypt(buffer);
        out.write((char*)buffer, block_size);
        blocks_left--;
    }

    if (blocks_left > 0) {
        memset(buffer, 0, block_size);  //safety!
        in.read((char*)buffer, block_size);
        engine.decrypt(buffer);

        size_t left_to_read = block_size;
        if (include_file_size)
            left_to_read = file_size + sizeof(size_t) - (blocks_count - 1) * block_size;
        out.write((char*)buffer, left_to_read);
    }
}

int main(int argc, Char* argv[]) {
    // cb268328d0b91fc62d108fa0508fb23d
    Char* iv = generate_byte_array(block_size);

    // d45e5a0fe4bfbfd7b1359ce7dd2d63be
    Char* key = generate_byte_array(block_size);

    //Aes engine(key);
    Pcbc engine(key, iv);
    encrypt(engine);
    engine.reset();
    decrypt(engine);

    //copy_first(0x35);

    return 0;
}
