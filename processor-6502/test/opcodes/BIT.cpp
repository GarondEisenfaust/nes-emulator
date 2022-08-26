#include "../BaseFixture.h"
#include "../Processor6502Proxy.h"
#include "Util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

class BIT : public BaseFixture {};

TEST_F(BIT, becomes_negative) {
  {
    const auto value = uint8_t{0b01001101};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->a = value;
    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b01000000};
    uint8_t expected = 0;
    Util::SetFlag(expected, N, true);

    cpu->a = value;
    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }
}

TEST_F(BIT, overflows) {
  {
    const auto value = uint8_t{0b00101101};
    uint8_t expected = 0;
    Util::SetFlag(expected, V, true);

    cpu->a = value;
    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const auto value = uint8_t{0b00100000};
    uint8_t expected = 0;
    Util::SetFlag(expected, V, true);

    cpu->a = value;
    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }
}

TEST_F(BIT, becomes_zero) {
  {
    const uint8_t value = 0;
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);

    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const uint8_t value = 0b00101101;
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);
    Util::SetFlag(expected, V, true);

    cpu->a = 0;
    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }

  {
    const uint8_t value = 0b00100000;
    uint8_t expected = 0;
    Util::SetFlag(expected, Z, true);
    Util::SetFlag(expected, V, true);

    cpu->a = 0;
    cpu->fetched = value;
    cpu->BIT();
    EXPECT_EQ(cpu->status, expected);
  }
}