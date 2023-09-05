#pragma once

class Divider {
 public:
  void Clock();
  void SetPeriod(int period);
  void Reset();
  bool Notify();

 private:
  bool mReload;
  int mCounter = 0;
  int mPeriod = 0;
  bool mNotify = false;
};
