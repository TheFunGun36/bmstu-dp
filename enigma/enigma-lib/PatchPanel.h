#pragma once
#include "Reflector.h"

class PatchPanel : public Reflector {
public:
    PatchPanel(const Alphabet& encoding);
    PatchPanel(Alphabet&& encoding) noexcept;
};
