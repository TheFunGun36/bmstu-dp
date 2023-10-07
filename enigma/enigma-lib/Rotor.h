#pragma once
#include "Permutator.h"

// Ротор. Биекция алфавита на самого себя.
class Rotor {
public:
    Rotor(const Permutator& permutator);
    Rotor(Permutator&& permutator);
    Rotor(Rotor&& other) noexcept;

    Char encode(Char symbol) const;
    Char decode(Char symbol) const;

    bool rotate();
    int rotation();
    void set_rotation(int value);

    void set_permutator(Permutator&& permutator);
    void set_encoding(Permutator::Alphabet&& encoding);

private:
    int m_rotation;
    Permutator m_permutator;
};
