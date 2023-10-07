#pragma once
#include <string>

using Char = char;

//// Используем русский алфавит. Букву Ё исключаем удобства ради. 
//constexpr size_t g_alphabet_length = 32;
//constexpr Char g_alphabet_begin = 'А';
//constexpr Char g_alphabet_othercase_begin = 'а';
//
//int normalize_letter(Char& symbol);
//int normalize_letter_index(int letter_index);
//bool is_letter_maincase(Char symbol);
//bool is_letter_othercase(Char symbol);
//bool is_letter(Char symbol);

class Permutator {
public:
    using Alphabet = std::string;

    Permutator(const Alphabet& encoding, Char codepage_encoding_begin);
    Permutator(Alphabet&& encoding, Char codepage_encoding_begin);
    Permutator(const Permutator& other) = default;
    Permutator(Permutator&& other) noexcept;

    Permutator& operator=(const Permutator& other);
    Permutator& operator=(Permutator&& other) noexcept;

    Char encode(Char symbol) const;
    Char decode(Char symbol) const;

    void set_encoding(const Alphabet& encoding);
    void set_encoding(Alphabet&& encoding);
    void set_codepage_encoding_begin(Char position);
    void set_codepage_encoding_othercase(bool exists, Char position);

    void make_mirrored();

    size_t encoding_length() const noexcept;

    Char alphabetical_cyclic_shift(Char symbol, int value) const;

    bool is_letter(Char symbol) const;

protected:
    Permutator() = default;
    Char permute_to_letter(int letter_index, bool inverse = false) const;

    int normalize_letter(Char& symbol) const;
    int normalize_letter_index(int letter_index) const;
    bool is_letter_maincase(Char symbol) const;
    bool is_letter_othercase(Char symbol) const;

    Alphabet m_encoding;
    Char m_codepage_encoding_begin;
    Char m_codepage_encoding_othercase_begin;
    bool m_othercase_exists;
};
