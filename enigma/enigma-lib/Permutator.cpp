#include "Permutator.h"
#include <cassert>

Permutator::Permutator(const Alphabet& encoding) {
    set_encoding(encoding);
}

Permutator::Permutator(Alphabet&& encoding) {
    set_encoding(move(encoding));
}

Permutator::Permutator(Permutator&& other) noexcept
    : m_encoding(move(other.m_encoding)){
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

void Permutator::set_encoding(const Alphabet& encoding) {
    Alphabet encoding_copied = encoding;
    set_encoding(move(encoding_copied));
}

void Permutator::set_encoding(Alphabet&& encoding) {
    assert(encoding.size() == g_alphabet_length);
    m_encoding = move(encoding);
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