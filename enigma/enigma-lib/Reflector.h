#pragma once
#include "Permutator.h"

// Коммутационная панель
class Reflector : public Permutator {
public:
    Reflector(const Alphabet& encoding);
    Reflector(Alphabet&& encoding) noexcept;
    Char decode(Char symbol) override;
    void set_encoding(Alphabet&& encoding) override;
    void set_encoding(const Alphabet& encoding) override;
};
