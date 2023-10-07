#include "Reflector.h"
#include "SmartPointers.h"
#include <cassert>

Reflector::Reflector(const Permutator& permutator)
    : m_permutator(permutator) {
    m_permutator.make_mirrored();
}

Reflector::Reflector(Permutator&& permutator) noexcept
    : m_permutator(move(permutator)) {
    m_permutator.make_mirrored();
}

Char Reflector::put_through(Char symbol) const {
    return m_permutator.encode(symbol);
}

void Reflector::set_permutator(Permutator&& permutator) {
    m_permutator = move(permutator);
    m_permutator.make_mirrored();
}

void Reflector::set_encoding(Permutator::Alphabet&& encoding) {
    m_permutator.set_encoding(move(encoding));
    m_permutator.make_mirrored();
}
