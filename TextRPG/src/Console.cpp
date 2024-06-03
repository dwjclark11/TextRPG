#include "Console.h"
#include "Logger.h"
#include <algorithm>
#include <vector>
#include <cassert>
#include <iostream>

bool Console::SetTextColor(int size, int x, int y, HANDLE handle, WORD color)
{
	COORD pos = {x, y};

	std::vector<WORD> write(size, color);

	DWORD written = 0;
	LPDWORD lpNumWritten = &written;

	if (!WriteConsoleOutputAttribute(handle, &write[ 0 ], size, pos, lpNumWritten))
	{
		TRPG_ERROR("Unable to change text color!");
		return false;
	}

	return true;
}

void Console::DrawBorder()
{
	DrawPanel(1, 0, SCREEN_WIDTH - 2, SCREEN_HEIGHT - 1);
}

Console::Console()
	: m_pScreen{nullptr}
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	bool bAdjustBuffer{false};
	int minX{GetSystemMetrics(SM_CXMIN)};
	int minY{GetSystemMetrics(SM_CYMIN)};

	if (minX > SCREEN_WIDTH)
	{
		SCREEN_WIDTH = static_cast<SHORT>(minX);
		bAdjustBuffer = true;
	}

	if (minY > SCREEN_HEIGHT)
	{
		SCREEN_HEIGHT = static_cast<SHORT>(minY);
		bAdjustBuffer = true;
	}

	if (bAdjustBuffer)
		BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

	COORD consoleBuffer{.X = SCREEN_WIDTH, .Y = SCREEN_HEIGHT};
	if (!SetConsoleScreenBufferSize(hConsole, consoleBuffer))
	{
		auto error = GetLastError();
		TRPG_ERROR("ERROR: " + std::to_string(error));
		throw("Failed to set the console screen buffer size when creating the console!");
	}

	SMALL_RECT windowRect{.Left = 0, .Top = 0, .Right = SCREEN_WIDTH - 1, .Bottom = SCREEN_HEIGHT - 1};
	if (!SetConsoleWindowInfo(hConsole, TRUE, &windowRect))
	{
		auto error = GetLastError();
		TRPG_ERROR("ERROR: " + std::to_string(error));
		throw("Failed to set the console window info when creating the console!");
	}

	// Get a handle to the console window
	m_hConsoleWindow = GetConsoleWindow();

	if (!GetWindowRect(m_hConsoleWindow, &m_ConsoleWindowRect))
		throw("Failed to get the Window Rect when creating the console!");

	// Center the window
	int posX = GetSystemMetrics(SM_CXSCREEN) / 2 - (m_ConsoleWindowRect.right - m_ConsoleWindowRect.left) / 2;
	int posY = GetSystemMetrics(SM_CYSCREEN) / 2 - (m_ConsoleWindowRect.bottom - m_ConsoleWindowRect.top) / 2;

	// Set the window pos, don't change the size
	SetWindowPos(m_hConsoleWindow, NULL, posX, posY, -1, -1, SWP_NOSIZE);

	// Initialize the screen buffer
	m_pScreen = std::make_unique<wchar_t[]>(BUFFER_SIZE);

	// Clear the screen buffer
	ClearBuffer();

	// Create the screen Buffer
	m_hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	if (!m_hConsole)
		throw("Failed to create the console screen buffer!");

	// Set the buffer to be active!
	if (!SetConsoleActiveScreenBuffer(m_hConsole))
		throw("Failed to set the active screen buffer");

	// Hide the cursor
	if (!ShowConsoleCursor(false))
		throw("Failed to hide the console cursor!");
}

Console::~Console()
{
}

void Console::ClearBuffer()
{
	// Set all the values of the buffer to an empty space
	for (int i = 0; i < BUFFER_SIZE; i++)
		m_pScreen[ i ] = L' ';
}

void Console::Write(int x, int y, const std::wstring& text, WORD color)
{
	std::vector<wchar_t> invalidCharacters{L' ', L'\n', L'\t', L'\r'};

	auto is_any_of = [ & ](wchar_t character) {
		if (text.size() > 1)
			return false;

		if (text.empty())
			return true;

		return character == text[ 0 ];
	};

	if (std::find_if(invalidCharacters.begin(), invalidCharacters.end(), is_any_of) == std::end(invalidCharacters))
		SetTextColor(text.size(), x, y, m_hConsole, color);

	// Get the position in the buffer based on the index
	int pos = y * SCREEN_WIDTH + x;

	// Check to see if the position goes beyond the BUFFER_SIZE
	assert(pos + text.size() < BUFFER_SIZE);

	// We do not wat to write to a position that is beyond the buffer size
	if (pos + text.size() >= BUFFER_SIZE)
	{
		TRPG_ERROR("Trying to write to a position that is beyond the BUFFER SIZE!");
		return;
	}

	swprintf(&m_pScreen[ pos ], BUFFER_SIZE, text.c_str());
}

void Console::Draw()
{
	DrawBorder();
	// Handle all console drawing
	WriteConsoleOutputCharacterW(m_hConsole, m_pScreen.get(), BUFFER_SIZE, {0, 0}, &m_BytesWritten);
}

bool Console::ShowConsoleCursor(bool show)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	if (!GetConsoleCursorInfo(m_hConsole, &cursorInfo))
	{
		TRPG_ERROR("Failed to get the cursor info!");
		return false;
	}
	cursorInfo.bVisible = show;

	return SetConsoleCursorInfo(m_hConsole, &cursorInfo);
}

void Console::DrawPanelHorz(int x, int y, size_t length, WORD color, const std::wstring& character)
{
	std::wstring sPanelHorz = L"";
	for (int i = 0; i < length; i++)
		sPanelHorz += character;

	Write(x, y, sPanelHorz, color);
}

void Console::DrawPanelVert(int x, int y, size_t height, WORD color, const std::wstring& character)
{
	for (int i = 0; i < height; i++)
		Write(x, y + i, character, color);
}

void Console::DrawPanel(int x, int y, size_t width, size_t height, WORD color, const std::wstring& width_char,
						const std::wstring& height_char)
{
	DrawPanelHorz(x, y, width, color, width_char);
	DrawPanelHorz(x, y + height, width, color, width_char);

	DrawPanelVert(x, y + 1, height - 1, color, height_char);
	DrawPanelVert(x + width - 1, y + 1, height - 1, color, height_char);
}
