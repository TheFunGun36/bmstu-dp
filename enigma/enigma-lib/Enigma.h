#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include "SmartPointers.h"
#include "Rotor.h"
#include "Reflector.h"
#include "PatchPanel.h"

class Enigma {
public:
    Enigma(size_t rotors_amount);

    Char encode_char(Char letter, std::ostream* verbose = nullptr);
    void encode_string(std::string& letter, std::ostream* verbose = nullptr);
    void encode_stream(std::istream& input, std::ostream& output, std::ostream* verbose = nullptr);

    std::vector<int> rotor_code();
    void set_rotor_code(const std::vector<int>& rotor_code);
    void set_rotor_encoding(const Alphabet& encoding, size_t rotor_index);
    void set_reflector_encoding(const Alphabet& encoding);
    void set_patch_panel_encoding(const Alphabet& encoding);

    bool rotate_on_non_alphabetical;
    bool bypass_non_alphabetical;

private:
    Char process_char(Char input, std::ostream* verbose);

    std::vector<UniquePtr<Rotor>> m_rotors;
    UniquePtr<Reflector> m_reflector;
    UniquePtr<PatchPanel> m_patch_panel;
};
