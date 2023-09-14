#pragma once

class Divider {
 public:
  void Clock();
  void SetPeriod(int period);
  void Reset();
  bool Notify();
  int mPeriod = 0;

 private:
  bool mReload;
  int mCounter = 0;
  bool mNotify = false;
};
