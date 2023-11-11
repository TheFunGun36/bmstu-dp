#include "Rotor.h"
#include <cassert>
#include "SmartPointers.h"

Rotor::Rotor(const Permutator& permutator)
    : m_permutator(permutator)
    , m_rotation(0) {
}

Rotor::Rotor(Permutator&& permutator)
    : m_permutator(move(permutator))
    , m_rotation(0) {
}

Rotor::Rotor(Rotor&& other) noexcept
    : m_permutator(move(other.m_permutator))
    , m_rotation(other.m_rotation) {
}

Char Rotor::encode(Char symbol) const {
    Char shifted = m_permutator.alphabetical_cyclic_shift(symbol, m_rotation);
    return m_permutator.encode(shifted);
}

Char Rotor::decode(Char symbol) const {
    Char decoded = m_permutator.decode(symbol);
    return m_permutator.alphabetical_cyclic_shift(decoded, -m_rotation);
}

bool Rotor::rotate() {
    m_rotation++;
    if (m_rotation >= m_permutator.encoding_length())
        m_rotation = 0;
    return m_rotation == 0;
}

int Rotor::rotation() {
    return m_rotation;
}

void Rotor::set_rotation(int value) {
    m_rotation = value;
}

void Rotor::set_permutator(Permutator&& permutator) {
    m_permutator = move(permutator);
}

void Rotor::set_encoding(Permutator::Alphabet&& encoding) {
    m_permutator.set_encoding(move(encoding));
}
