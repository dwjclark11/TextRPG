#pragma once

#include "utilities/Colors.h"
#include <Windows.h>
#include <memory>
#include <string>

class Console
{
private:
	const int SCREEN_WIDTH = 128;
	const int SCREEN_HEIGHT = 48;

	const int BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;
	
	HANDLE m_hConsole;
	HWND m_hConsoleWindow;
	RECT m_ConsoleWindowRect;

	DWORD m_BytesWritten;

	std::unique_ptr<wchar_t[]> m_pScreen;
	
	bool SetTextColor(int size, int x, int y, HANDLE handle, WORD color);

public:
	Console();
	~Console();

	void ClearBuffer();
	void Write(int x, int y, const std::wstring& text, WORD color = WHITE);
	void Draw();
	bool ShowConsoleCursor(bool show);
};