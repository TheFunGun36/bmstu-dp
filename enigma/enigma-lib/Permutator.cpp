#include "Permutator.h"
#include <cassert>

Permutator::Permutator(const Alphabet& encoding)
    : m_encoding(encoding) {
    assert(m_encoding.size() == g_alphabet_length);
}

Char Permutator::encode(Char symbol) {
    int letter_index = normalize_letter(symbol);
    
    // not our alphabet
    if (letter_index < 0)
        return symbol;

    return permute_to_letter(letter_index);
}

Char Permutator::decode(Char symbol) {
    int letter_index = normalize_letter(symbol);

    // not our alphabet
    if (letter_index < 0)
        return symbol;

    return permute_to_letter(letter_index, true);
}

int Permutator::normalize_letter(Char& symbol) {
    int letter_index = -1;
    if (is_letter_maincase(symbol)) {
        letter_index = symbol - g_alphabet_begin;
    }
    else if (is_letter_othercase(symbol)) {
        letter_index = symbol - g_alphabet_othercase_begin;
        symbol += g_alphabet_begin - g_alphabet_othercase_begin;
    }

    return letter_index;
}

bool Permutator::is_letter_maincase(Char symbol) {
    unsigned char s = symbol;
    return s >= g_alphabet_begin
        && s < g_alphabet_begin + g_alphabet_length;
}

bool Permutator::is_letter_othercase(Char symbol) {
    unsigned char s = symbol;
    return s >= g_alphabet_othercase_begin
        && s < g_alphabet_othercase_begin + g_alphabet_length;
}

Char Permutator::permute_to_letter(int letter_index, bool inverse) {
    assert(letter_index >= 0 && letter_index < g_alphabet_length);
    if (!inverse) {
        return m_encoding[letter_index];
    }

    auto pos = std::find(
        m_encoding.begin(),
        m_encoding.end(),
        letter_index + g_alphabet_begin);

    assert(pos != m_encoding.end());
    return g_alphabet_begin + (pos - m_encoding.begin());
}

int Permutator::normalize_letter_index(int letter_index) {
    if (letter_index < 0)
        letter_index += ((-letter_index - 1) / 3 + 1) * g_alphabet_length;
    else if (letter_index >= g_alphabet_length)
        letter_index -= letter_index / g_alphabet_length * g_alphabet_length;
    return letter_index;
}
