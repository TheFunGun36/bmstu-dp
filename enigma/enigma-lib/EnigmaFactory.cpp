#include "EnigmaFactory.h"
#include <random>

Enigma EnigmaFactory::create_enigma_rus()
{
    std::string alphabet = "юабцдефгхийклмнопярстужвьызшэщчъ";
    std::string rotor1_encoding = "сзтьнюврмбокгжлцуйщапеъдяфчшэыих";
    std::string rotor2_encoding = "тжбдусозцящкрихйъючвашфьыеэпмлгн";
    std::string rotor3_encoding = "кйоемъаэнвбждзюушыцяьпиртслгчщхф";
    std::string reflector_encoding = "прсзвэтщжучышяьъ****************";
    std::string panel_encoding = "ятщзъчывьэрсжушп****************";
    Permutator permutator("пярстужвьызшэщчъюабцдефгхийклмно", unsigned char('ю'));
    permutator.set_codepage_encoding_othercase(true, 'Ю');

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

	return enigma;
}

Enigma EnigmaFactory::create_enigma_eng()
{
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string rotor1_encoding = "KTNZMSJHPRQBLFGIDUOVCWEYAX";
    std::string rotor2_encoding = "LNATCBSJKODEGFXYHMUWPRVQIZ";
    std::string rotor3_encoding = "XESCFVWZRUNJYPKTOMIABQHLDG";
    std::string reflector_encoding = "TYPZNSVOWXURQ*************";
    std::string panel_encoding = "TSOYWXVRPZQUN*************";
    Permutator permutator("NOPQRSTUVWXYZ*************", unsigned char('A'));
    permutator.set_codepage_encoding_othercase(true, 'a');

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

    return enigma;
}

static void shuffle_alphabet(std::string &alphabet)
{
    static std::mt19937 g(27834);
    std::shuffle(alphabet.begin(), alphabet.end(), g);
}

Enigma EnigmaFactory::create_enigma_bin()
{
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

    return enigma;
}
