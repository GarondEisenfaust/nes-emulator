#include "Button.h"
#include "ShapeRendering/SolidRectangleRenderer.h"

Button::Button(int x, int y, int width, int height, SolidRectangleRenderer &renderer) :
        mX(x),mY(y),mWidth(width),mHeight(height),mRectangle(
        renderer.CreateRectangle(x, y, width, height, {1, 0, 0, 1})) {}

bool Button::isPressed(int x, int y) {
    return (mX < x && x < (mX + mWidth)) && (mY < y && y < (mY + mHeight));
}
