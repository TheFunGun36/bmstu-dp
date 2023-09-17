#pragma once
#include "Alphabet.h"

class Permutator {
public:
    Permutator(const Alphabet& encoding);
    Permutator(Alphabet&& encoding);
    Permutator(Permutator&& other) noexcept;

    virtual Char encode(Char symbol);
    virtual Char decode(Char symbol);
    virtual void set_encoding(const Alphabet& encoding);
    virtual void set_encoding(Alphabet&& encoding);

protected:
    Permutator() = default;
    virtual Char permute_to_letter(int letter_index, bool inverse = false);

    Alphabet m_encoding;
};
