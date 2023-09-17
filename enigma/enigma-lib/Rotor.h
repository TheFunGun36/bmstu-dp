#pragma once
#include <string>
#include "Alphabet.h"

// Ротор. Биекция алфавита на самого себя.
class Rotor {
public:
    using Alphabet = std::string;

    Rotor(const Alphabet& alphabet_encoding);

    Char encode(Char symbol);
    Char decode(Char symbol);
    void rotate(int shift);

private:
    static bool is_letter_maincase(Char symbol);
    static bool is_letter_othercase(Char symbol);
    static int normalize_letter_index(int letter_index);
    int cyclic_shift(int letter_index, bool inverse = false);
    Char shifted_letter(int letter_index, bool inverse = false);

    Alphabet m_alphabet_encoding;
    int m_current_rotation;
};
