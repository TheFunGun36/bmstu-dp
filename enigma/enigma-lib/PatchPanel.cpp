#include "PatchPanel.h"
#include "SmartPointers.h"

PatchPanel::PatchPanel(const Permutator& permutator)
    : Reflector(permutator) {
}

PatchPanel::PatchPanel(Permutator&& permutator) noexcept
    : Reflector(move(permutator)) {
}
