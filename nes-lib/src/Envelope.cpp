#include "Envelope.h"
#include <iostream>

void Envelope::Clock() {
  if (startFlag) {
    startFlag = false;
    decayLevelCounter = 15;
    divider.Reset();
  } else {
    divider.Clock();
  }

  if (divider.Notify()) {
    // divider.SetPeriod(volume);
    // divider.Reset();
    if (decayLevelCounter > 0) {
      decayLevelCounter--;
    } else if (loop) {
      decayLevelCounter = 15;
    }
  }
  output = constantVolume ? volume : decayLevelCounter;
}
