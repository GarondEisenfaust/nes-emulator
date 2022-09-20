#include "../BaseFixture.h"
#include "../Processor6502Proxy.h"
#include "Util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class JMP : public BaseFixture {};

TEST_F(JMP, is_set) {
  {
    const auto value = uint16_t{222};
    cpu->SetAddrAbs(value);
    cpu->JMP();
    EXPECT_EQ(cpu->GetAddrAbs(), cpu->pc);
  }
}
