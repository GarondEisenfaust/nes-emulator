#pragma once
#include "NesConfig.h"

class ArgumentParser {
 public:
  NesConfig Parse(int argc, char* argv[]);
};
