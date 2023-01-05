#include "Console.h"
#include "Logger.h"
#include <vector>

bool Console::SetTextColor(int size, int x, int y, HANDLE handle, WORD color)
{
	COORD pos = { x, y };

	std::vector<WORD> write(size, color);

	DWORD written = 0;
	LPDWORD lpNumWritten = &written;

	if (!WriteConsoleOutputAttribute(handle, &write[0], size, pos, lpNumWritten))
	{
		TRPG_ERROR("Unable to change text color!");
		return false;
	}

	return true;
}

Console::Console()
	: m_pScreen{nullptr}
{
	// Initialize the screen buffer
	m_pScreen = std::make_unique<wchar_t[]>(BUFFER_SIZE);

	// Get a handle to the console window
	m_hConsoleWindow = GetConsoleWindow();

	if (!GetWindowRect(m_hConsoleWindow, &m_ConsoleWindowRect))
		throw("Failed to get the Window Rect when creating the console!");

	// Get the font size 
	CONSOLE_FONT_INFO font_info;
	if (!GetCurrentConsoleFont(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font_info))
		throw("Failed to get the console font!");

	COORD font_size = GetConsoleFontSize(GetStdHandle(STD_OUTPUT_HANDLE), font_info.nFont);

	int actual_screen_x = SCREEN_WIDTH * font_size.X + 16;
	int actual_screen_y = SCREEN_HEIGHT * font_size.Y + 48;

	int pos_x = GetSystemMetrics(SM_CXSCREEN) / 2 - (actual_screen_x / 2);
	int pos_y = GetSystemMetrics(SM_CYSCREEN) / 2 - (actual_screen_y / 2);

	// Set the size and position of the window
	if (!MoveWindow(m_hConsoleWindow, pos_x, pos_y, actual_screen_x, actual_screen_y, TRUE))
		throw("Failed to set and move the console window!");

	// Clear the screen buffer
	ClearBuffer();

	// Create the screen Buffer
	m_hConsole = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	
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
		m_pScreen[i] = L' ';

	// Reset all the buffer to white
	SetTextColor(BUFFER_SIZE, 0, 0, m_hConsole, WHITE);
}

void Console::Write(int x, int y, const std::wstring& text, WORD color)
{
	SetTextColor(text.size(), x, y, m_hConsole, color);
	int pos = y * SCREEN_WIDTH + x;
	swprintf(&m_pScreen[pos], BUFFER_SIZE, text.c_str());
}

void Console::Draw()
{
	// Handle all console drawing
	WriteConsoleOutputCharacter(m_hConsole, m_pScreen.get(), BUFFER_SIZE, { 0, 0 }, &m_BytesWritten);
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
