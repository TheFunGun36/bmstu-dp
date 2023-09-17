#pragma once
#include "Permutator.h"
#include "Alphabet.h"

// Ротор. Биекция алфавита на самого себя.
class Rotor : public Permutator {
public:
    Rotor(const Alphabet& encoding);

    void rotate(int shift);
    int rotation();
    void set_rotation(int value);

private:
    int cyclic_shift(int letter_index, bool inverse = false);
    virtual Char permute_to_letter(int letter_index, bool inverse = false) override;

    int m_rotation;
};
