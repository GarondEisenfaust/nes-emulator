#include "Cartridge.h"
#include "Definitions.h"
#include "mapper/Mappers.h"
#include <fstream>

struct Header {
  char name[4];
  uint8_t programRomChunks;
  uint8_t characterRomChunks;
  uint8_t mapper1;
  uint8_t mapper2;
  uint8_t programRamSize;
  uint8_t tvSystem1;
  uint8_t tvSystem2;
  char unused[5];
};

uint8_t DetermineMapperId(const Header& header) { return ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4); }

Cartridge::MIRROR DetermineMirror(const Header& header) {
  return (header.mapper1 & 0x01) ? Cartridge::VERTICAL : Cartridge::HORIZONTAL;
}

Cartridge::Cartridge(const std::string& path) {
  Header header;

  mImageValid = false;
  std::ifstream romStream;

  romStream.open(path, std::ifstream::binary);

  if (!romStream.is_open()) {
    return;
  }

  romStream.read((char*)&header, sizeof(Header));
  if (header.mapper1 & 0x04) {
    romStream.seekg(512, std::ios_base::cur);
  }

  mMapperId = DetermineMapperId(header);
  mMirror = DetermineMirror(header);

  mProgramBanks = header.programRomChunks;
  mProgramMemory.resize(mProgramBanks * 16384);
  romStream.read(reinterpret_cast<char*>(mProgramMemory.data()), mProgramMemory.size());

  mCharacterBanks = header.characterRomChunks;
  auto banksToAllocate = mCharacterBanks != 0 ? mCharacterBanks : 1;
  mCharacterMemory.resize(banksToAllocate * 8192);
  romStream.read(reinterpret_cast<char*>(mCharacterMemory.data()), mCharacterMemory.size());

  mMapper = std::make_unique<Mapper000>(mProgramBanks, mCharacterBanks);

  mImageValid = true;
  romStream.close();
}

Cartridge::ReadResult Cartridge::CpuRead(uint16_t address) {
  auto mappingResult = mMapper->CpuMapRead(address);
  if (mappingResult.mapped) {
    return {mProgramMemory[mappingResult.mappedAddress], true};
  }
  return {UNDEFINED, false};
}

bool Cartridge::CpuWrite(uint16_t address, uint8_t data) {
  auto mappingResult = mMapper->CpuMapWrite(address);
  if (mappingResult.mapped) {
    mProgramMemory[mappingResult.mappedAddress] = data;
    return true;
  }
  return false;
}

Cartridge::ReadResult Cartridge::PpuRead(uint16_t address) {
  auto mappingResult = mMapper->PpuMapRead(address);
  if (mappingResult.mapped) {
    return {mCharacterMemory[mappingResult.mappedAddress], true};
  }
  return {UNDEFINED, false};
}

bool Cartridge::PpuWrite(uint16_t address, uint8_t data) {
  auto mappingResult = mMapper->PpuMapWrite(address);
  if (mappingResult.mapped) {
    mCharacterMemory[mappingResult.mappedAddress] = data;
    return true;
  }
  return false;
}

void Cartridge::Reset() {
  if (mMapper) {
    mMapper->Reset();
  }
}
