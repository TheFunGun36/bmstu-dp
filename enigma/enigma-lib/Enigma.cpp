#include "Enigma.h"
#include <cassert>
#include <sstream>

Enigma::Enigma(size_t rotors_amount) {
    Alphabet plain_alphabet =
        "јЅ¬√ƒ≈∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЏџ№Ёёя";
    Alphabet plain_alphabet_halved =
        "–—“”‘’÷„ЎўЏџ№Ёёя";

    m_rotors.reserve(rotors_amount);
    for (int i = 0; i < rotors_amount; i++)
        m_rotors.push_back(move(make_unique<Rotor>(plain_alphabet)));
    m_reflector = move(make_unique<Reflector>(plain_alphabet_halved));
    m_patch_panel = move(make_unique<PatchPanel>(plain_alphabet_halved));
    rotate_on_non_alphabetical = false;
    bypass_non_alphabetical = true;
}

Char Enigma::encode_char(Char c, std::ostream* verbose) {
    return process_char(c, verbose);
}

void Enigma::encode_string(std::string& str, std::ostream* verbose) {
    for (auto& el : str)
        el = process_char(el, verbose);
}

std::vector<int> Enigma::rotor_code() {
    std::vector<int> result(m_rotors.size());
    for (int i = 0; i < m_rotors.size(); i++)
        result[i] = m_rotors[i]->rotation();
    return result;
}

void Enigma::set_rotor_code(const std::vector<int>& rotor_code) {
    assert(rotor_code.size() == m_rotors.size());
    for (int i = 0; i < m_rotors.size(); i++)
        m_rotors[i]->set_rotation(rotor_code[i]);
}

void Enigma::set_rotor_encoding(const Alphabet& encoding, size_t index) {
    assert(index >= 0 && index < m_rotors.size());
    m_rotors[index]->set_encoding(encoding);
}

void Enigma::set_reflector_encoding(const Alphabet& encoding) {
    m_reflector->set_encoding(encoding);
}

void Enigma::set_patch_panel_encoding(const Alphabet& encoding) {
    m_patch_panel->set_encoding(encoding);
}

Char Enigma::process_char(Char letter, std::ostream* verbose) {
    if (bypass_non_alphabetical && !is_letter(letter))
        return letter;

    // —начала поворот (если надо)
    if (rotate_on_non_alphabetical || is_letter(letter)) {
        bool rotate_next = true;
        for (int i = m_rotors.size() - 1; rotate_next && i >= 0; i--)
            rotate_next = m_rotors[i]->rotate();
    }

    // Ќа вводе коммутационна€ панель
    if (verbose) *verbose << letter << " > p > ";
    letter = m_patch_panel->encode(letter);
    if (verbose) *verbose << letter;

    // ѕо очереди роторы, с первого по последний
    for (int i = 0; i < m_rotors.size(); i++) {
        if (verbose) *verbose << " > r" << i + 1 << " > ";
        letter = m_rotors[i]->encode(letter);
        if (verbose) *verbose << letter;
    }

    // –ефлектор
    if (verbose) {
        std::string spacer(20 + 5 * m_rotors.size(), ' ');
        *verbose << '\n';
        *verbose << spacer;
        *verbose << "v\n";
        *verbose << spacer;
        *verbose << "m\n";
        *verbose << spacer;
        *verbose << "v\n";
    }
    std::stringstream buf;
    letter = m_reflector->encode(letter);
    if (verbose) buf << '\n' << letter << " < ";


    // –оторы, в обратном пор€дке
    for (int i = m_rotors.size() - 1; i >= 0; i--) {
        if (verbose) buf << i + 1 << "r < ";
        letter = m_rotors[i]->decode(letter);
        if (verbose) buf << letter << " < ";
    }

    // Ќа выводе коммутационна€ панель
    if (verbose) buf << "p < ";
    letter = m_patch_panel->decode(letter);
    if (verbose) {
        buf << letter;
        std::string str = buf.str();
        std::reverse(str.begin(), str.end());
        *verbose << str;
    }

    return letter;
}
