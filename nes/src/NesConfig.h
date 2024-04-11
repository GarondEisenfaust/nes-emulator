#pragma once
#include <string>

class NesConfig {
 public:
  enum SignalDecoder { LookupTable = 0, LookupTableGpu = 1, Ntsc = 2, NtscGpu = 3 };

  std::string mRomPath = "";
  std::string mRomDirPath = "";
  SignalDecoder mDecoder = LookupTable;

  bool Validate();
};
