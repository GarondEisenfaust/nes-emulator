#include "../BaseFixture.h"
#include "../Processor6502Proxy.h"
#include "Util.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class ROL : public BaseFixture {};

TEST_F(ROL, rotates_bits) {
  {
    const auto value = uint8_t{0b00000110};
    cpu->SetOpcode(2);
    cpu->a = 99;
    cpu->fetched = value;

    cpu->ROL();

    const auto expected = uint8_t{0b00001100};
    EXPECT_EQ(cpu->a, expected);
  }
}

TEST_F(ROL, rotates_into_carry) {
  {
    const auto value = uint8_t{0b10000000};
    cpu->SetOpcode(2);
    cpu->a = 99;
    cpu->fetched = value;
    cpu->status = 0;
    Util::SetFlag(cpu->status, C, 0);

    cpu->ROL();

    uint8_t expectedStatus = 0;
    Util::SetFlag(expectedStatus, C, 1);
    Util::SetFlag(expectedStatus, Z, 1);

    EXPECT_EQ(cpu->status, expectedStatus);
    EXPECT_EQ(cpu->a, 0b00000000);
  }
}

TEST_F(ROL, rotates_out_off_carry) {
  {
    const auto value = uint8_t{0b00000000};
    cpu->SetOpcode(2);
    cpu->fetched = value;
    cpu->status = 0;
    Util::SetFlag(cpu->status, C, 1);

    cpu->ROL();

    uint8_t expectedStatus = 0;
    Util::SetFlag(expectedStatus, C, 0);
    Util::SetFlag(expectedStatus, Z, 0);

    EXPECT_EQ(cpu->status, expectedStatus);
    EXPECT_EQ(cpu->a, 0b00000001);
  }
}
