#include "inputs/Button.h"

void Button::Update(bool pressed)
{
	bIsJustPressed = !bIsDown && pressed;
	bIsJustReleased = bIsDown && !pressed;
	bIsDown = pressed;
}
