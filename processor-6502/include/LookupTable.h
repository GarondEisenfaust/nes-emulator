#pragma once
#include "Instruction.h"
#include <memory>
#include <vector>

class AddressingModeContainer;
class OpcodeContainer;

using LookupTable = std::vector<Instruction>;

std::unique_ptr<LookupTable> MakeLookupTable(AddressingModeContainer& addressingModes, OpcodeContainer& opcodes);
