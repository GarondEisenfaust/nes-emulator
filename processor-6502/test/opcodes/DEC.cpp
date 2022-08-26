#include "../BaseFixture.h"
#include "../Processor6502Proxy.h"
#include "Util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class DEC : public BaseFixture {};

TEST_F(DEC, becomes_negative) {
  {
    const auto value = uint8_t{0b01000001};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->fetched = value;
    cpu->DEC();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b00000000};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->fetched = value;
    cpu->DEC();
    EXPECT_EQ(cpu->status, expected);
  }
}

TEST_F(DEC, stays_positive) {
  {
    const auto value = uint8_t{0b00001001};
    const uint8_t expected = 0;

    cpu->fetched = value;
    cpu->DEC();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b00001110};
    const uint8_t expected = 0;
    cpu->fetched = value;
    cpu->DEC();
    EXPECT_EQ(cpu->status, expected);
  }
}

TEST_F(DEC, becomes_zero) {
  {
    const auto value = uint8_t{0b00000001};
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);

    cpu->fetched = value;
    cpu->DEC();
    EXPECT_EQ(cpu->status, expected);
  }
}
