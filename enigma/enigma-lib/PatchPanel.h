#pragma once
#include "Reflector.h"

class PatchPanel : public Reflector {
public:
    PatchPanel(const Permutator& permutator);
    PatchPanel(Permutator&& permutator) noexcept;
};
