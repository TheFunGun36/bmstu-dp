#pragma once
#include <string>

// ���������� ������� �������. ����� � ��������� �������� ����. 
using Char = char;
using Alphabet = std::string;
constexpr size_t g_alphabet_length = 32;
constexpr Char g_alphabet_begin = '�';
constexpr Char g_alphabet_othercase_begin = '�';

int normalize_letter(Char& symbol);
int normalize_letter_index(int letter_index);
bool is_letter_maincase(Char symbol);
bool is_letter_othercase(Char symbol);
bool is_letter(Char symbol);
