#include "../BaseFixture.h"
#include "../Processor6502Proxy.h"
#include "Util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class DEY : public BaseFixture {};

TEST_F(DEY, becomes_negative) {
  {
    const auto value = uint8_t{0b01000001};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->y = value;
    cpu->DEY();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b01001100};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->y = value;
    cpu->DEY();
    EXPECT_EQ(cpu->status, expected);
  }
}

TEST_F(DEY, becomes_zero) {
  {
    const auto value = uint8_t{0b00000001};
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);

    cpu->y = value;
    cpu->DEY();
    EXPECT_EQ(cpu->status, expected);
  }
}
