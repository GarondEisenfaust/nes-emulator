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

std::unique_ptr<IMapper> MakeMapper(int mapperId, int programBanks, int characterBanks) {
  if (mapperId == 0) {
    return std::make_unique<Mapper000>(programBanks, characterBanks);
  }
  if (mapperId == 2) {
    return std::make_unique<Mapper002>(programBanks, characterBanks);
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
  mMirror = DetermineMirror(header);

  mProgramBanks = header.programRomChunks;
  mProgramMemory.resize(mProgramBanks * 16384);
  romStream.read(reinterpret_cast<char*>(mProgramMemory.data()), mProgramMemory.size());

  mCharacterBanks = header.characterRomChunks;
  auto banksToAllocate = mCharacterBanks > 0 ? mCharacterBanks : 1;
  mCharacterMemory.resize(banksToAllocate * 8192);
  romStream.read(reinterpret_cast<char*>(mCharacterMemory.data()), mCharacterMemory.size());

  mMapper = MakeMapper(mMapperId, mProgramBanks, mCharacterBanks);
  mMapper->Reset();
  mImageValid = true;
  romStream.close();
}

uint8_t Cartridge::CpuRead(uint16_t address) {
  auto mappedAddr = mMapper->CpuMapRead(address);
  return mProgramMemory[mappedAddr];
}

void Cartridge::CpuWrite(uint16_t address, uint8_t data) {
  auto mappedAddr = mMapper->CpuMapWrite(address, data);
  mProgramMemory[mappedAddr] = data;
}

uint8_t Cartridge::PpuRead(uint16_t address) {
  auto mappedAddr = mMapper->PpuMapRead(address);
  return mCharacterMemory[mappedAddr];
}

void Cartridge::PpuWrite(uint16_t address, uint8_t data) {
  uint32_t mappedAddr = mMapper->PpuMapWrite(address);
  mCharacterMemory[mappedAddr] = data;
}

bool Cartridge::Interrupt() { return mMapper->Interrupt(); }

void Cartridge::ClearInterrupt() { mMapper->ClearInterrupt(); }

void Cartridge::Reset() {
  if (mMapper) {
    mMapper->Reset();
  }
}
