#include <iostream>
#include <algorithm>
#include <random>
#include "enigma-lib/Rotor.h"

static void shuffle_alphabet(Rotor::Alphabet& alphabet) {
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
    Rotor::Alphabet plain_alphabet =
        "юабцдефгхийклмнопярстужвьызшэщчъ";

    Rotor::Alphabet shuffled1 =
        "сзтьнюврмбокгжлцуйщапеъдяфчшэыих";
    Rotor::Alphabet shuffled2 =
        "тжбдусозцящкрихйъючвашфьыеэпмлгн";
    Rotor::Alphabet shuffled3 =
        "кйоемъаэнвбждзюушыцяьпиртслгчщхф";

    Rotor rotor(plain_alphabet);

    for (int i = 0; i < g_alphabet_length + 1; i++) {
        std::cout << rotor.encode('Ю') << ' ' << rotor.encode('ю') << '\n';
        rotor.rotate(1);
    }
    return 0;
}
