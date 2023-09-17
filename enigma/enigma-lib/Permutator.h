#pragma once
#include "Alphabet.h"

class Permutator {
public:
    Permutator(const Alphabet& encoding);

    virtual Char encode(Char symbol);
    virtual Char decode(Char symbol);

protected:
    static int normalize_letter(Char &symbol);
    static int normalize_letter_index(int letter_index);
    static bool is_letter_maincase(Char symbol);
    static bool is_letter_othercase(Char symbol);

    virtual Char permute_to_letter(int letter_index, bool inverse = false);

    Alphabet m_encoding;
};
