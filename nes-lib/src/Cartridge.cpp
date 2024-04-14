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

MirrorMode DetermineMirror(const Header& header) {
  return (header.mapper1 & 0x01) ? MirrorMode::Vertical : MirrorMode::Horizontal;
}

std::unique_ptr<IMapper> MakeMapper(int mapperId, MirrorMode mirrorMode, int programBanks, int characterBanks) {
  if (mapperId == 0) {
    return std::make_unique<Mapper000>(programBanks, characterBanks, mirrorMode);
  }
  if (mapperId == 1) {
    return std::make_unique<Mapper001>(programBanks, characterBanks, mirrorMode);
  }
  if (mapperId == 2) {
    return std::make_unique<Mapper002>(programBanks, characterBanks, mirrorMode);
  }
  if (mapperId == 4) {
    return std::make_unique<Mapper004>(programBanks, characterBanks, mirrorMode);
  }
  return std::unique_ptr<IMapper>();
}

Cartridge::Cartridge(const std::string& path) {
  Header header;

  mImageValid = false;
  std::ifstream romStream;

  romStream.open(path, std::ifstream::binary);

  if (!romStream.is_open()) {
    return;
  }

  romStream.read(reinterpret_cast<char*>(&header), sizeof(Header));
  if (header.mapper1 & 0x04) {
    romStream.seekg(512, std::ios_base::cur);
  }

  mMapperId = DetermineMapperId(header);
  const auto mirrorMode = DetermineMirror(header);

  mProgramBanks = header.programRomChunks;
  mProgramMemory.resize(mProgramBanks * 16384);
  romStream.read(reinterpret_cast<char*>(mProgramMemory.data()), mProgramMemory.size());

  mCharacterBanks = header.characterRomChunks;
  const auto banksToAllocate = mCharacterBanks > 0 ? mCharacterBanks : 1;
  mCharacterMemory.resize(banksToAllocate * 8192);
  romStream.read(reinterpret_cast<char*>(mCharacterMemory.data()), mCharacterMemory.size());

  mMapper = MakeMapper(mMapperId, mirrorMode, mProgramBanks, mCharacterBanks);
  mMapper->Reset();
  mImageValid = true;
  romStream.close();
}

uint8_t Cartridge::CpuRead(uint16_t address) {
  const auto mappingResult = mMapper->CpuMapRead(address);
  if (mappingResult.data) {
    return *mappingResult.data;
  }
  if (mappingResult.mappedAddress) {
    return mProgramMemory[*mappingResult.mappedAddress];
  }
  return 0;
}

void Cartridge::CpuWrite(uint16_t address, uint8_t data) {
  const auto mappingResult = mMapper->CpuMapWrite(address, data);
  if (mappingResult.mappedAddress) {
    mProgramMemory[*mappingResult.mappedAddress] = data;
  }
}

uint8_t Cartridge::PpuRead(uint16_t address) {
  const auto mappingResult = mMapper->PpuMapRead(address);
  if (mappingResult.data) {
    return *mappingResult.data;
  }
  if (mappingResult.mappedAddress) {
    return mCharacterMemory[*mappingResult.mappedAddress];
  }
  return 0;
}

void Cartridge::PpuWrite(uint16_t address, uint8_t data) {
  const auto mappingResult = mMapper->PpuMapWrite(address);
  if (mappingResult.mappedAddress) {
    mCharacterMemory[*mappingResult.mappedAddress] = data;
  }
}

bool Cartridge::Interrupt() { return mMapper->Interrupt(); }

void Cartridge::ClearInterrupt() { mMapper->ClearInterrupt(); }

void Cartridge::ScanlineCounter() { mMapper->ScanlineCounter(); }

void Cartridge::Reset() {
  if (mMapper) {
    mMapper->Reset();
  }
}

MirrorMode Cartridge::GetMirrorMode() { return mMapper->GetMirrorMode(); }
