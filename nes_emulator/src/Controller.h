#pragma once

#include "Button.h"
#include "IController.h"
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <array>
#include <map>

class SolidRectangleRenderer;

enum TouchEvent { UP, DOWN };
struct Touch {
  const int id;
  const TouchEvent event;
  const int x;
  const int y;
};

class Controller : public IController {
 public:
  Controller(SolidRectangleRenderer& renderer);

  void Write(uint16_t address, uint8_t data);
  uint8_t Read(uint16_t address);
  void HandleInputEvents(android_input_buffer* inputBuffer);

  void ResetRegisters(int index);

 private:
  SolidRectangleRenderer& mRenderer;
  Button mButtonA;
  Button mButtonB;
  Button mButtonStart;
  Button mButtonSelect;
  Button mButtonUp;
  Button mButtonDown;
  Button mButtonRight;
  Button mButtonLeft;

  union ControllerRegister {
    struct {
      bool right : 1;
      bool left : 1;
      bool down : 1;
      bool up : 1;
      bool start : 1;
      bool select : 1;
      bool b : 1;
      bool a : 1;
    };
    uint8_t reg = 0x00;
  };

  std::map<int, Touch> inputEvents;
  std::array<uint8_t, 2> controllerBuffer;
  std::array<ControllerRegister, 2> mControllerRegister;

  void HandleEvent(const Touch& touch);
  void CheckTouchEvents(android_input_buffer* inputBuffer);
  void CheckIfButtonPressed(int index, int x, int y);
  void CheckIfButtonPressed(int index, const Touch& touch);
  void CheckIfButtonsPressed(int index);};
