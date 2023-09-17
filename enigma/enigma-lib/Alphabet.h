#pragma once
#include <string>

// Используем русский алфавит. Букву Ё исключаем удобства ради. 
using Char = char;
using Alphabet = std::string;
constexpr size_t g_alphabet_length = 32;
constexpr Char g_alphabet_begin = 'А';
constexpr Char g_alphabet_othercase_begin = 'а';
