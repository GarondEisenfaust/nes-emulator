#include "Cartridge.h"
#include "cstring"
#include "mapper/Mappers.h"
#include <fstream>
#include <unistd.h>

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
  if (mapperId == 7) {
    return std::make_unique<Mapper007>(programBanks, characterBanks, mirrorMode);
  }
  return std::unique_ptr<IMapper>();
}

Cartridge::Cartridge(const int fd) {
  auto* file = fdopen(fd, "r");
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  fseek(file, 0, SEEK_SET);

  std::vector<uint8_t> romData;
  romData.resize(fileSize);
  fread(romData.data(), fileSize, 1, file);

  fclose(file);
  close(fd);
  Init(romData);
}

Cartridge::Cartridge(const std::string& path) {
  std::ifstream romStream(path);
  std::vector<uint8_t> romData;
  romData.insert(romData.begin(), std::istreambuf_iterator<char>(romStream), std::istreambuf_iterator<char>());
  Init(romData);
  romStream.close();
}

void Cartridge::Init(const std::vector<uint8_t>& romData) {
  Header header;
  mImageValid = false;

  auto iter = romData.begin();
  std::memcpy(&header, &(*iter), sizeof(Header));
  iter += sizeof(Header);
  if (header.mapper1 & 0x04) {
    iter += 512;
  }

  mMapperId = DetermineMapperId(header);
  const auto mirrorMode = DetermineMirror(header);

  mProgramBanks = header.programRomChunks;
  mProgramMemory.resize(mProgramBanks * 16384);
  std::memcpy(mProgramMemory.data(), &(*iter), mProgramMemory.size());
  iter += mProgramMemory.size();

  mCharacterBanks = header.characterRomChunks;
  const auto banksToAllocate = mCharacterBanks > 0 ? mCharacterBanks : 1;
  mCharacterMemory.resize(banksToAllocate * 8192);
  std::memcpy(mCharacterMemory.data(), &(*iter), mCharacterMemory.size());
  iter += mCharacterMemory.size();

  mMapper = MakeMapper(mMapperId, mirrorMode, mProgramBanks, mCharacterBanks);
  mMapper->Reset();
  mImageValid = true;
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
