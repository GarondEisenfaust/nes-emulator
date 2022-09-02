#include "Util.h"
#include "gtest/gtest.h"
#include <cstdint>

TEST(Util, C) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b00000001;
    Util::SetFlag(toChange, C, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b11111110;
    Util::SetFlag(toChange, C, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, Z) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b00000010;
    Util::SetFlag(toChange, Z, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b11111101;
    Util::SetFlag(toChange, Z, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, I) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b00000100;
    Util::SetFlag(toChange, I, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b11111011;
    Util::SetFlag(toChange, I, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, D) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b00001000;
    Util::SetFlag(toChange, D, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b11110111;
    Util::SetFlag(toChange, D, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, B) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b00010000;
    Util::SetFlag(toChange, B, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b11101111;
    Util::SetFlag(toChange, B, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, U) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b00100000;
    Util::SetFlag(toChange, U, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b11011111;
    Util::SetFlag(toChange, U, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, V) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b01000000;
    Util::SetFlag(toChange, V, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b10111111;
    Util::SetFlag(toChange, V, false);

    ASSERT_EQ(toChange, expected);
  }
}

TEST(Util, N) {
  {
    uint8_t toChange = 0;
    const uint8_t expected = 0b10000000;
    Util::SetFlag(toChange, N, true);

    ASSERT_EQ(toChange, expected);
  }

  {
    uint8_t toChange = 0xff;
    const uint8_t expected = 0b01111111;
    Util::SetFlag(toChange, N, false);

    ASSERT_EQ(toChange, expected);
  }
}
