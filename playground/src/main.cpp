#include "Header.h"
#include <iostream>

int main() {
  auto a = static_cast<uint8_t>(0b01000000);
  auto first = static_cast<bool>(a & (1 << 6));

  auto b = static_cast<uint8_t>(0b00000000);
  auto second = static_cast<bool>(b & (1 << 6));

  std::cout << first << " " << second << std::endl;
}
