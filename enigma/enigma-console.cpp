#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include "enigma-lib/Enigma.h"

constexpr std::initializer_list<int> rotor_code = { 1, 1, 1 };
constexpr const char* default_input_file_name = "input";
constexpr const char* output_file_name = "output";
constexpr const char* doubled_output_file_name = "output2";

//#define USE_RUS
//#define USE_ENG
#define USE_BIN

static void shuffle_alphabet(std::string& alphabet) {
    //static std::random_device rd;
    static std::mt19937 g(27834);
    std::shuffle(alphabet.begin(), alphabet.end(), g);
}

void create_binary(const char* filename) {
    std::ofstream out(filename, std::ios::binary);
    for (int i = 0; i < 256; i++)
        out << Char(i);
}

int main(int argc, const char* argv[]) {
#ifndef WIN32
    static_assert("nope, no support for non-windows os :)\n"
        "good luck on making port though XDXDXD");
#endif
    system("chcp 1251 > nul");
    const char* input_file_name =
        (argc == 2) ? argv[1] : "input";

    std::ostream *debug_out = &std::cout;

#ifdef USE_RUS
    std::string alphabet = "юабцдефгхийклмнопярстужвьызшэщчъ";
    std::string rotor1_encoding = "сзтьнюврмбокгжлцуйщапеъдяфчшэыих";
    std::string rotor2_encoding = "тжбдусозцящкрихйъючвашфьыеэпмлгн";
    std::string rotor3_encoding = "кйоемъаэнвбждзюушыцяьпиртслгчщхф";
    std::string reflector_encoding = "прсзвэтщжучышяьъ****************";
    std::string panel_encoding = "ятщзъчывьэрсжушп****************";
    Permutator permutator("пярстужвьызшэщчъюабцдефгхийклмно", unsigned char('ю'));
    permutator.set_codepage_encoding_othercase(true, 'Ю');
#endif

#ifdef USE_ENG
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string rotor1_encoding = "KTNZMSJHPRQBLFGIDUOVCWEYAX";
    std::string rotor2_encoding = "LNATCBSJKODEGFXYHMUWPRVQIZ";
    std::string rotor3_encoding = "XESCFVWZRUNJYPKTOMIABQHLDG";
    std::string reflector_encoding = "TYPZNSVOWXURQ*************";
    std::string panel_encoding = "TSOYWXVRPZQUN*************";
    Permutator permutator("NOPQRSTUVWXYZ*************", unsigned char('A'));
    permutator.set_codepage_encoding_othercase(true, 'a');
#endif

#ifdef USE_BIN
    debug_out = nullptr;
    std::string alphabet(256, '*');
    for (int i = 0; i < 256; i++)
        alphabet[i] = i;

    std::string left_half = alphabet.substr(0, 128);
    std::string right_half = alphabet.substr(128, 128);

    std::string rotor1_encoding = alphabet;
    std::string rotor2_encoding = alphabet;
    std::string rotor3_encoding = alphabet;
    std::string reflector_encoding = right_half;
    std::string panel_encoding = right_half;

    shuffle_alphabet(rotor1_encoding);
    shuffle_alphabet(rotor2_encoding);
    shuffle_alphabet(rotor3_encoding);
    shuffle_alphabet(reflector_encoding);
    reflector_encoding.append(128, '*');
    shuffle_alphabet(panel_encoding);
    panel_encoding.append(128, '*');

    right_half.append(left_half);
    Permutator permutator(right_half, '\0');
    permutator.set_codepage_encoding_othercase(false, '\0');
#endif

    Enigma enigma(move(permutator), 3);
    enigma.set_rotor_code({ 1, 1, 1 });
    enigma.bypass_non_alphabetical = true;

    enigma.set_reflector_encoding(move(reflector_encoding));
    enigma.set_patch_panel_encoding(move(panel_encoding));
    enigma.set_rotor_encoding(move(rotor1_encoding), 0);
    enigma.set_rotor_encoding(move(rotor2_encoding), 1);
    enigma.set_rotor_encoding(move(rotor3_encoding), 2);

    enigma.set_reflector_encoding_bypass(false);
    enigma.set_patch_panel_encoding_bypass(false);
    enigma.set_rotor_encoding_bypass(false);

    {
        std::ifstream in(input_file_name, std::ios::binary);
        if (!in.is_open())
            std::cout << "no such file: \"" << input_file_name << '"';

        std::ofstream out(output_file_name, std::ios::binary);
        enigma.set_rotor_code(rotor_code);
        enigma.encode_stream(in, out, debug_out);
    }
    {
        std::ifstream in(output_file_name, std::ios::binary);
        std::ofstream out(doubled_output_file_name, std::ios::binary);
        enigma.set_rotor_code(rotor_code);
        enigma.encode_stream(in, out, debug_out);
    }

    create_binary("bin");

    return 0;
}
