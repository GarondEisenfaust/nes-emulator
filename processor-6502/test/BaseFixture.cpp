#include "BaseFixture.h"

Processor6502GmockProxy ::Processor6502GmockProxy() : Processor6502Proxy() {
  ON_CALL(*this, Fetch).WillByDefault(testing::Return(0));
  ON_CALL(*this, Write).WillByDefault(testing::Return());

  // Suppresses the uninteresting mock warning
  EXPECT_CALL(*this, Fetch).Times(testing::AtLeast(0));
  EXPECT_CALL(*this, Write).Times(testing::AtLeast(0));
}

void BaseFixture::SetUp() { cpu = std::make_unique<Processor6502GmockProxy>(); }
void BaseFixture::TearDown() {}
