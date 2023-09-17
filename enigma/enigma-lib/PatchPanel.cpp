#include "PatchPanel.h"

PatchPanel::PatchPanel(const Alphabet& encoding)
    : Reflector(encoding) {
}

PatchPanel::PatchPanel(Alphabet&& encoding) noexcept
    : Reflector(move(encoding)) {
}
