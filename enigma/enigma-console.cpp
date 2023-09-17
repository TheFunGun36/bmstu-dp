#include <iostream>
#include <algorithm>
#include <random>
#include "enigma-lib/Rotor.h"

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

    
    Alphabet rotor1_encoding =
        "сзтьнюврмбокгжлцуйщапеъдяфчшэыих";
    Alphabet rotor2_encoding =
        "тжбдусозцящкрихйъючвашфьыеэпмлгн";
    Alphabet rotor3_encoding =
        "кйоемъаэнвбждзюушыцяьпиртслгчщхф";

    Rotor rotor(plain_alphabet);

    for (int i = 0; i < g_alphabet_length + 1; i++) {
        Char encoded = rotor.encode('Ю');
        std::cout << "a->" << rotor.encode('Ю')
            << ", inverse: "
            << encoded << "->"
            << rotor.decode(encoded)
            << '\n';
        rotor.rotate(1);
    }
    

    //Alphabet panel_encoding =
    //    "ыздачкрэшуъжняхлвоьгитцщпфмюбесй";
    //
    //shuffle_alphabet(plain_alphabet);
    //std::cout << plain_alphabet;

    return 0;
}
