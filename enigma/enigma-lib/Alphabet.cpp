#include "Alphabet.h"

int normalize_letter(Char& symbol) {
    unsigned char s = symbol; 
    unsigned char letter_index = -1;

    if (is_letter_maincase(s)) {
        letter_index = s - g_alphabet_begin;
    }
    else if (is_letter_othercase(s)) {
        letter_index = s - g_alphabet_othercase_begin;
        s += g_alphabet_begin - g_alphabet_othercase_begin;
    }
    else
        return -1;

    return letter_index;
}

bool is_letter_maincase(Char symbol) {
    unsigned char s = symbol;
    unsigned char alphabet_begin = g_alphabet_begin;
    return s >= alphabet_begin
        && s < alphabet_begin + g_alphabet_length;
}

bool is_letter_othercase(Char symbol) {
    unsigned char s = symbol;
    unsigned char alphabet_othercase_begin = g_alphabet_othercase_begin;
    return s >= alphabet_othercase_begin
        && s < alphabet_othercase_begin + g_alphabet_length;
}

bool is_letter(Char symbol) {
    return is_letter_maincase(symbol) || is_letter_othercase(symbol);
}

int normalize_letter_index(int letter_index) {
    if (letter_index < 0)
        letter_index += ((-letter_index - 1) / g_alphabet_length + 1) * g_alphabet_length;
    else if (letter_index >= g_alphabet_length)
        letter_index -= letter_index / g_alphabet_length * g_alphabet_length;
    return letter_index;
}