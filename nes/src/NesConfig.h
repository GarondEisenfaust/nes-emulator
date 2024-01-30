#pragma once
#include <string>

class NesConfig {
 public:
  enum SignalDecoder { LookupTable = 0, Ntsc = 1, NtscGpu = 2 };

  std::string mRomPath = "";
  SignalDecoder mDecoder = LookupTable;

  bool Validate();
};
