#pragma once
#include "Permutator.h"

// Коммутационная панель
class Reflector {
public:
    Reflector(const Permutator& permutator);
    Reflector(Permutator&& permutator) noexcept;
    Char put_through(Char symbol) const;

    void set_permutator(Permutator&& permutator);
    void set_encoding(Permutator::Alphabet&& encoding);

private:
    Permutator m_permutator;
};
