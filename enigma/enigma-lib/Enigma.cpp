#include "Enigma.h"
#include <cassert>
#include <sstream>

Enigma::Enigma(Permutator&& permutator, size_t rotors_amount)
    : m_permutator(permutator) {
    m_rotors.reserve(rotors_amount);
    for (int i = 0; i < rotors_amount; i++)
        m_rotors.push_back(move(make_unique<Rotor>(m_permutator)));
    m_reflector = move(make_unique<Reflector>(m_permutator));
    m_patch_panel = move(make_unique<PatchPanel>(m_permutator));
    rotate_on_non_alphabetical = false;
    bypass_non_alphabetical = true;
    m_rotor_encoding_bypass = false;
    m_reflector_encoding_bypass = false;
    m_patch_panel_encoding_bypass = false;
}

Char Enigma::encode_char(Char c, std::ostream* verbose) {
    return process_char(c, verbose);
}

void Enigma::encode_string(std::string& str, std::ostream* verbose) {
    for (auto& el : str)
        el = process_char(el, verbose);
}

void Enigma::encode_stream(std::istream& input, std::ostream& output, std::ostream* verbose) {
    Char c;
    while (input.get(c))
        output << process_char(c, verbose);
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

void Enigma::set_rotor_encoding(Permutator::Alphabet&& encoding, size_t rotor_index) {
    assert(rotor_index < m_rotors.size());
    m_rotors[rotor_index]->set_encoding(move(encoding));
}

void Enigma::set_reflector_encoding(Permutator::Alphabet&& encoding) {
    m_reflector->set_encoding(move(encoding));
}

void Enigma::set_patch_panel_encoding(Permutator::Alphabet&& encoding) {
    m_patch_panel->set_encoding(move(encoding));
}

void Enigma::set_rotor_encoding_bypass(bool bypass) {
    m_rotor_encoding_bypass = bypass;
}

void Enigma::set_reflector_encoding_bypass(bool bypass) {
    m_reflector_encoding_bypass = bypass;
}

void Enigma::set_patch_panel_encoding_bypass(bool bypass) {
    m_patch_panel_encoding_bypass = bypass;
}

Char Enigma::process_char(Char letter, std::ostream* verbose) {
    if (bypass_non_alphabetical && !m_permutator.is_letter(letter))
        return letter;

    // Сначала поворот (если надо)
    if (rotate_on_non_alphabetical || m_permutator.is_letter(letter)) {
        bool rotate_next = true;
        for (int i = m_rotors.size() - 1; rotate_next && i >= 0; i--)
            rotate_next = m_rotors[i]->rotate();
    }

    // На вводе коммутационная панель
    if (verbose) *verbose << letter << " >|К|> ";
    if (!m_patch_panel_encoding_bypass)
        letter = m_patch_panel->put_through(letter);
    if (verbose) *verbose << letter;

    // По очереди роторы, с первого по последний
    for (int i = 0; i < m_rotors.size(); i++) {
        if (verbose) *verbose << " >|Р|> ";
        if (!m_rotor_encoding_bypass)
            letter = m_rotors[i]->encode(letter);
        if (verbose) *verbose << letter;
    }

    // Рефлектор
    if (verbose) {
        *verbose << " >|\n";
        *verbose << "   |О|     ";
        for (int i = 0; i < m_rotors.size(); i++)
            *verbose << "|Т|     ";
        *verbose << "|РЕФЛЕКТОР\n";
    }
    std::stringstream buf;
    if (!m_reflector_encoding_bypass)
        letter = m_reflector->put_through(letter);
    if (verbose) buf << "\n\n\n|< " << letter << " <";


    // Роторы, в обратном порядке
    for (int i = m_rotors.size() - 1; i >= 0; i--) {
        if (verbose) buf << "|" << i + 1 << "|< ";
        if (!m_rotor_encoding_bypass)
            letter = m_rotors[i]->decode(letter);
        if (verbose) buf << letter << " <";
    }

    // На выводе коммутационная панель
    if (verbose) buf << "|М|< ";
    if (!m_patch_panel_encoding_bypass)
        letter = m_patch_panel->put_through(letter);
    if (verbose) {
        buf << letter;
        std::string str = buf.str();
        std::reverse(str.begin(), str.end());
        *verbose << str;
    }

    return letter;
}
