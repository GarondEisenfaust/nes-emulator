#pragma once
#include "Processor6502Proxy.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <memory>

class Processor6502GmockProxy : public Processor6502Proxy {
 public:
  Processor6502GmockProxy();
  ~Processor6502GmockProxy() = default;

  // Prevents the real method from being called, since that would overwrite the 'fetched' variable.
  MOCK_METHOD0(Fetch, uint8_t());
  MOCK_METHOD2(Write, void(uint16_t addr, uint8_t data));
};

class BaseFixture : public ::testing::Test {
 public:
  virtual void SetUp();
  virtual void TearDown();

  std::unique_ptr<Processor6502Proxy> cpu;
};
