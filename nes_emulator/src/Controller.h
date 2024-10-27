#pragma once

#include "IController.h"
#include "Button.h"
#include <array>

class SolidRectangleRenderer;

class Controller : public IController {
public:
    Controller(SolidRectangleRenderer &renderer);
    void CheckButtons(int x, int y);
    void Write(uint16_t address, uint8_t data);
    uint8_t Read(uint16_t address);
    void ResetRegisters();
private :
    SolidRectangleRenderer &mRenderer;
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
    std::array<uint8_t, 2> controllerBuffer;
    std::array<ControllerRegister, 2> mControllerRegister;
};
