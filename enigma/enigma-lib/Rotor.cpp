#include "Rotor.h"
#include <cassert>

Rotor::Rotor(const Alphabet& alphabet_encoding)
    : m_alphabet_encoding(alphabet_encoding)
    , m_current_rotation(0) {
    assert(m_alphabet_encoding.size() == g_alphabet_length);
}

Char Rotor::encode(Char symbol) {
    if (is_letter_maincase(symbol))
        return shifted_letter(symbol - g_alphabet_begin);
    else if (is_letter_othercase(symbol))
        return shifted_letter(symbol - g_alphabet_othercase_begin);
    else
        return symbol;
}

Char Rotor::decode(Char symbol) {
    if (is_letter_maincase(symbol))
        return shifted_letter(symbol - g_alphabet_begin, true);
    else if (is_letter_othercase(symbol))
        return shifted_letter(symbol - g_alphabet_othercase_begin, true);
    else
        return symbol;
}

void Rotor::rotate(int shift) {
    m_current_rotation = normalize_letter_index(m_current_rotation + shift);
}

bool Rotor::is_letter_maincase(Char symbol) {
    return symbol >= g_alphabet_begin
        && symbol < g_alphabet_begin + g_alphabet_length;
}

bool Rotor::is_letter_othercase(Char symbol) {
    return symbol >= g_alphabet_othercase_begin
        && symbol < g_alphabet_othercase_begin + g_alphabet_length;
}

int Rotor::normalize_letter_index(int letter_index) {
    if (letter_index < 0)
        letter_index += ((-letter_index - 1) / 3 + 1) * g_alphabet_length;
    else if (letter_index >= g_alphabet_length)
        letter_index -= letter_index / g_alphabet_length * g_alphabet_length;
    return letter_index;
}

int Rotor::cyclic_shift(int letter_index, bool inverse) {
    letter_index += inverse ? -m_current_rotation : m_current_rotation;
    return normalize_letter_index(letter_index);
}

Char Rotor::shifted_letter(int letter_index, bool inverse) {
    letter_index = cyclic_shift(letter_index, inverse);

    if (!inverse) {
        return m_alphabet_encoding[letter_index];
    }

    auto pos = std::find(
        m_alphabet_encoding.begin(),
        m_alphabet_encoding.end(),
        letter_index + g_alphabet_begin);

    assert(pos != m_alphabet_encoding.end());
    return g_alphabet_begin + (pos - m_alphabet_encoding.begin());
}
