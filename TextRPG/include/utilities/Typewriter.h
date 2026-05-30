#pragma once

#include <string>
#include <Windows.h>
#include <vector>
#include "Timer.h"
#include "Colors.h"

class Console;

class Typewriter
{
  public:
	Typewriter(Console& console);
	Typewriter(Console& console, int start_x, int start_y, const std::wstring& text, int text_wrap, int speed,
			   WORD textColor = WHITE, WORD borderColor = WHITE);

	~Typewriter();

	bool SetText(const std::wstring& text);
	inline void SetBorderColor(WORD color) { m_BorderColor = color; }

	void UpdateText();
	void Draw(bool showborder = true);
	inline const bool IsFinished() const { return m_bFinished; }

private:
	bool SetBorderProperties();
	void DrawBorder();
	void ClearArea();

private:
	Console& m_Console;
	std::wstring m_sText;
	std::wstring m_sCurrentText;
	int m_X;
	int m_Y;
	int m_BorderX;
	int m_BorderY;
	int m_BorderWidth;
	int m_BorderHeight;

	int m_TextSpeed; 
	int m_TextWrap;
	int m_Index;
	int m_CharIndex;
	int m_TextIndex;

	WORD m_TextColor;
	WORD m_BorderColor;

	Timer m_Timer;
	bool m_bFinished;

	std::vector<std::wstring> m_sTextChunks;
};
