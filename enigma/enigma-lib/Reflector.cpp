#include "Reflector.h"
#include <cassert>

Reflector::Reflector(const Alphabet& encoding) {
    set_encoding(encoding);
}

Reflector::Reflector(Alphabet&& encoding) noexcept {
    set_encoding(encoding);
}

Char Reflector::decode(Char symbol) {
    // a lil quicker
    return encode(symbol);
}

void Reflector::set_encoding(Alphabet&& encoding) {
    size_t half = g_alphabet_length / 2;
    assert(encoding.size() == half);
    m_encoding = move(encoding);
    m_encoding.append(half, '*');

    for (int i = 0; i < half; i++) {
        unsigned char idx = m_encoding[i] - g_alphabet_begin;
        assert(idx >= half);
        m_encoding[idx] = i + g_alphabet_begin;
    }
}

void Reflector::set_encoding(const Alphabet& encoding) {
    Alphabet encoding_copied = encoding;
    set_encoding(move(encoding_copied));
}
