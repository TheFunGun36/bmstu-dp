#include <iostream>
#include <algorithm>
#include <fstream>
#include "enigma-lib/Enigma.h"
#include "enigma-lib/EnigmaFactory.h"

constexpr std::initializer_list<int> rotor_code = { 1, 2, 3 };
constexpr const char *default_input_file_name = "input";
constexpr const char *default_output_file_name = "output";

int main(int argc, const char *argv[])
{
	system("chcp 1251 > nul");
	const char *input_file_name = (argc > 1) ? argv[1] : default_input_file_name;
	const char *output_file_name = (argc > 2) ? argv[2] : default_output_file_name;

	// auto enigma = EnigmaFactory::create_enigma_rus();
	// auto enigma = EnigmaFactory::create_enigma_eng();

	auto enigma = EnigmaFactory::create_enigma_bin();

	enigma.set_rotor_code(rotor_code);

	std::ifstream in(input_file_name, std::ios::binary);
	if (!in.is_open())
		std::cout << "file: " << input_file_name << " not found!";

	std::ofstream out(output_file_name, std::ios::binary);
	enigma.encode_stream(in, out, nullptr);

	return 0;
}
