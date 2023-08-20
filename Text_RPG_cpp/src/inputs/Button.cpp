#include "Button.h"

void Button::Update(bool pressed)
{
	m_bIsJustPressed = !m_bIsDown && pressed;
	m_bIsJustReleased = m_bIsDown && !pressed;
	m_bIsDown = pressed;
}
