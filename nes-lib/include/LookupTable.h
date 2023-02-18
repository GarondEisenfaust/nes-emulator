#pragma once
#include "Instruction.h"
#include <array>
#include <memory>

class AddressingModeContainer;
class OpcodeContainer;

using LookupTable = std::array<Instruction, 256>;

std::unique_ptr<LookupTable> MakeLookupTable(AddressingModeContainer& addressingModes, OpcodeContainer& opcodes);
