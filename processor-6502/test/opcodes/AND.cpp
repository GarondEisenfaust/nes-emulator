#include "../BaseFixture.h"
#include "../Processor6502Proxy.h"
#include "Util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class AND : public BaseFixture {};

TEST_F(AND, equal) {
  {
    const auto value = uint8_t{0b01000001};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->fetched = value;
    cpu->a = value;
    cpu->AND();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b01111001};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->fetched = value;
    cpu->a = value;
    cpu->AND();
    EXPECT_EQ(cpu->status, expected);
  }
}

TEST_F(AND, unequal) {
  {
    const auto value = uint8_t{0b01000001};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->fetched = 0b00110001;
    cpu->a = value;
    cpu->AND();
    EXPECT_NE(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b01001101};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->fetched = 0b00110001;
    cpu->a = value;
    cpu->AND();
    EXPECT_NE(cpu->status, expected);
  }
}

TEST_F(AND, zero_flag_set) {
  {
    const auto value = uint8_t{0b01011101};
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);

    cpu->fetched = 0b00000000;
    cpu->a = value;

    cpu->AND();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b01010001};
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);

    cpu->fetched = 0b00000000;
    cpu->a = value;

    cpu->AND();
    EXPECT_EQ(cpu->status, expected);
  }
}