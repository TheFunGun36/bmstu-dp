#pragma once

#include "Enigma.h"

class EnigmaFactory
{
public:
	static Enigma create_enigma_rus();
	static Enigma create_enigma_eng();
	static Enigma create_enigma_bin();
};