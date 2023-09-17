#include "Rotor.h"
#include <cassert>
#include "SmartPointers.h"

Rotor::Rotor(const Alphabet& encoding)
    : Permutator(encoding)
    , m_rotation(0) {
}

Rotor::Rotor(Alphabet&& encoding)
    : Permutator(move(encoding))
    , m_rotation(0) {
}

Rotor::Rotor(Rotor&& other) noexcept
    : Permutator(move(other))
    , m_rotation(other.m_rotation) {
}

bool Rotor::rotate() {
    m_rotation++;
    if (m_rotation >= g_alphabet_length)
        m_rotation = 0;
    return m_rotation == 0;
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

/*Char Rotor::permute_to_letter(int letter_index, bool inverse) {
    letter_index = cyclic_shift(letter_index, inverse);
    return Permutator::permute_to_letter(letter_index, inverse);
}*/

Char Rotor::permute_to_letter(int letter_index, bool inverse) {
    assert(letter_index >= 0 && letter_index < g_alphabet_length);
    if (!inverse) {
        letter_index = cyclic_shift(letter_index, inverse);
        return m_encoding[letter_index];
    }

    auto pos = std::find(
        m_encoding.begin(),
        m_encoding.end(),
        letter_index + g_alphabet_begin);

    assert(pos != m_encoding.end());

    letter_index = pos - m_encoding.begin();
    letter_index = cyclic_shift(letter_index, inverse);
    return g_alphabet_begin + letter_index;
}