#include "Permutator.h"
#include <cassert>

Permutator::Permutator(const Alphabet& encoding, Char codepage_encoding_begin) {
    set_encoding(encoding);
    m_codepage_encoding_begin = codepage_encoding_begin;
    m_codepage_encoding_othercase_begin = 0;
    m_othercase_exists = false;
}

Permutator::Permutator(Alphabet&& encoding, Char codepage_encoding_begin) {
    set_encoding(move(encoding));
    m_codepage_encoding_begin = codepage_encoding_begin;
    m_codepage_encoding_othercase_begin = 0;
    m_othercase_exists = false;
}

Permutator::Permutator(Permutator&& other) noexcept
    : m_encoding(move(other.m_encoding)) {
    m_codepage_encoding_begin = other.m_codepage_encoding_begin;
    m_codepage_encoding_othercase_begin = other.m_codepage_encoding_othercase_begin;
    m_othercase_exists = other.m_othercase_exists;
}

Permutator& Permutator::operator=(const Permutator& other) {
    m_encoding = other.m_encoding;
    return *this;
}

Permutator& Permutator::operator=(Permutator&& other) noexcept {
    m_encoding = move(other.m_encoding);
    return *this;
}

Char Permutator::encode(Char symbol) const {
    int letter_index = normalize_letter(symbol);
    
    // not our alphabet
    if (letter_index < 0)
        return symbol;

    return permute_to_letter(letter_index);
}

Char Permutator::decode(Char symbol) const {
    int letter_index = normalize_letter(symbol);

    // not our alphabet
    if (letter_index < 0)
        return symbol;

    return permute_to_letter(letter_index, true);
}

void Permutator::set_encoding(const Alphabet& encoding) {
    m_encoding = encoding;
}

void Permutator::set_encoding(Alphabet&& encoding) {
    m_encoding = move(encoding);
}

void Permutator::set_codepage_encoding_begin(Char position) {
    m_codepage_encoding_begin = position;
}

void Permutator::set_codepage_encoding_othercase(bool exists, Char position) {
    m_codepage_encoding_othercase_begin = position;
    m_othercase_exists = exists;
}

void Permutator::make_mirrored() {
    assert(m_encoding.length() % 2 == 0);
    size_t half = m_encoding.length() / 2;
    for (int i = 0; i < half; i++) {
        unsigned char idx = m_encoding[i] - m_codepage_encoding_begin;
        assert(idx >= half);
        m_encoding[idx] = i + m_codepage_encoding_begin;
    }
}

size_t Permutator::encoding_length() const noexcept {
    return m_encoding.length();
}

Char Permutator::alphabetical_cyclic_shift(Char symbol, int value) const {
    int letter_index = normalize_letter(symbol);
    if (letter_index < 0)
        return symbol;

    return m_codepage_encoding_begin + normalize_letter_index(letter_index + value);
}

Char Permutator::permute_to_letter(int letter_index, bool inverse) const {
    assert(letter_index >= 0 && letter_index < m_encoding.length());
    if (!inverse)
        return m_encoding[letter_index];

    auto pos = std::find(
        m_encoding.begin(),
        m_encoding.end(),
        Char(letter_index) + m_codepage_encoding_begin);

    assert(pos != m_encoding.end());
    return m_codepage_encoding_begin + (pos - m_encoding.begin());
}

int Permutator::normalize_letter(Char& symbol) const {
    unsigned char s = symbol;
    unsigned char letter_index = -1;

    if (is_letter_maincase(s)) {
        letter_index = s - m_codepage_encoding_begin;
    }
    else if (m_othercase_exists && is_letter_othercase(s)) {
        letter_index = s - m_codepage_encoding_othercase_begin;
        s += m_codepage_encoding_begin - m_codepage_encoding_othercase_begin;
    }
    else
        return -1;

    return letter_index;
}

bool Permutator::is_letter_maincase(Char symbol) const {
    unsigned char s = symbol;
    unsigned char alphabet_begin = m_codepage_encoding_begin;
    return s >= alphabet_begin
        && s < alphabet_begin + m_encoding.length();
}

bool Permutator::is_letter_othercase(Char symbol) const {
    const unsigned char s = symbol;
    const unsigned char alphabet_othercase_begin = m_codepage_encoding_othercase_begin;
    return m_othercase_exists
        && s >= alphabet_othercase_begin
        && s < alphabet_othercase_begin + m_encoding.length();
}

bool Permutator::is_letter(Char symbol) const {
    return is_letter_maincase(symbol) || is_letter_othercase(symbol);
}

int Permutator::normalize_letter_index(int letter_index) const {
    if (letter_index < 0)
        letter_index += ((-letter_index - 1) / m_encoding.length() + 1) * m_encoding.length();
    else if (letter_index >= m_encoding.length())
        letter_index -= letter_index / m_encoding.length() * m_encoding.length();
    return letter_index;
}
