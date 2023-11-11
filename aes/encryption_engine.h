#pragma once
using Char = unsigned char;

class EncryptionEngine {
public:
    virtual void reset() = 0;
    virtual void encrypt(Char* block) = 0;
    virtual void decrypt(Char* block) = 0;
};