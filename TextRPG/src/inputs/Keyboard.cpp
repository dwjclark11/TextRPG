#include "Keyboard.h"
#include "../Logger.h"

void Keyboard::Update()
{
	for (int i = 0; i < KEY_LAST; i++)
	{
		m_Keys[ i ].m_bIsJustPressed = false;
		m_Keys[ i ].m_bIsJustReleased = false;
	}
}

void Keyboard::OnKeyDown(int key)
{
	// Check to see if the key is defined
	if (key >= KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return;
	}
	m_Keys[ key ].Update(true);
}

void Keyboard::OnKeyUp(int key)
{
	// Check to see if the key is defined
	if (key >= KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return;
	}
	m_Keys[ key ].Update(false);
}

bool Keyboard::IsKeyHeld(int key) const
{
	// Check to see if the key is defined
	if (key >= KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return m_Keys[ key ].m_bIsDown;
}

bool Keyboard::IsKeyJustPressed(int key) const
{
	// Check to see if the key is defined
	if (key >= KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return m_Keys[ key ].m_bIsJustPressed;
}

bool Keyboard::IsKeyJustReleased(int key) const
{
	// Check to see if the key is defined
	if (key >= KEY_LAST)
	{
		TRPG_ERROR("[" + std::to_string(key) + "] - Is not defined!");
		return false;
	}
	return m_Keys[ key ].m_bIsJustReleased;
}
