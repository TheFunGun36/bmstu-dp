#include <iostream>
#include <algorithm>
#include <random>
#include "enigma-lib/Enigma.h"

static void shuffle_alphabet(Alphabet& alphabet) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(alphabet.begin(), alphabet.end(), g);
}

int main() {
#ifndef WIN32
    static_assert("nope, no support for non-windows os :)\n"
        "good luck on making port though XDXDXD");
#endif
    system("chcp 1251 > nul");
    Alphabet plain_alphabet =
        "юабцдефгхийклмнопярстужвьызшэщчъ";
    Alphabet halved_alphabet = "пярстужвьызшэщчъ";

    
    Alphabet rotor1_encoding =
        "сзтьнюврмбокгжлцуйщапеъдяфчшэыих";
    Alphabet rotor2_encoding =
        "тжбдусозцящкрихйъючвашфьыеэпмлгн";
    Alphabet rotor3_encoding =
        "кйоемъаэнвбждзюушыцяьпиртслгчщхф";
    Alphabet reflector_encoding =
        "прсзвэтщжучышяьъ";
    Alphabet panel_encoding =
        "ятщзъчывьэрсжушп";

    Enigma enigma(3);
    enigma.bypass_non_alphabetical = true;
    enigma.set_reflector_encoding(reflector_encoding);
    enigma.set_patch_panel_encoding(panel_encoding);
    enigma.set_rotor_encoding(rotor1_encoding, 0);
    enigma.set_rotor_encoding(rotor2_encoding, 1);
    enigma.set_rotor_encoding(rotor3_encoding, 2);

    using namespace std;
    string message = "оПХБЕР ЛХП!";
    cout << message << "\n";
    enigma.set_rotor_code({ 31, 31, 29 });
    enigma.encode_string(message);
    cout << message << "\n";
    enigma.set_rotor_code({ 31, 31, 29 });
    enigma.encode_string(message);
    cout << message << "\n";

    return 0;
}
