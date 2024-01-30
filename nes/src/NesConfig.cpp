#include "NesConfig.h"
#include <iostream>

bool NesConfig::Validate() {
  if (mRomPath.empty()) {
    std::cout << "You have to specify a ROM to load!\n";
    return false;
  }
  return true;
}
