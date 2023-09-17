#include "Rotor.h"
#include <cassert>

Rotor::Rotor(const Alphabet& encoding)
    : Permutator(encoding)
    , m_rotation(0) {
    assert(m_encoding.size() == g_alphabet_length);
}

void Rotor::rotate(int shift) {
    m_rotation = normalize_letter_index(m_rotation + shift);
}

int Rotor::rotation() {
    return m_rotation;
}

void Rotor::set_rotation(int value) {
    m_rotation = value;
}

int Rotor::cyclic_shift(int letter_index, bool inverse) {
    letter_index += inverse ? -m_rotation : m_rotation;
    return normalize_letter_index(letter_index);
}

Char Rotor::permute_to_letter(int letter_index, bool inverse) {
    letter_index = cyclic_shift(letter_index, inverse);
    return Permutator::permute_to_letter(letter_index, inverse);
}
