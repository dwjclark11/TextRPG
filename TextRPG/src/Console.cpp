#include "ui/Console.h"
#include "utilities/Logger.h"

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
    : m_pScreen{ nullptr }
{
    m_hConsole = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        CONSOLE_TEXTMODE_BUFFER,
        nullptr
    );

    if (m_hConsole == INVALID_HANDLE_VALUE)
    {
        throw("Failed to create the console screen buffer!");
    }

    COORD largestSize = GetLargestConsoleWindowSize(m_hConsole);

    if (SCREEN_WIDTH > largestSize.X)
    {
        SCREEN_WIDTH = largestSize.X;
    }

    if (SCREEN_HEIGHT > largestSize.Y)
    {
        SCREEN_HEIGHT = largestSize.Y;
    }

    BUFFER_SIZE = SCREEN_WIDTH * SCREEN_HEIGHT;

    COORD bufferSize =
    {
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    };

    if (!SetConsoleScreenBufferSize(m_hConsole, bufferSize))
    {
        auto error = GetLastError();
        TRPG_ERROR("ERROR: " + std::to_string(error));
        throw("Failed to set the console screen buffer size!");
    }

    SMALL_RECT windowRect =
    {
        0,
        0,
        static_cast<SHORT>(SCREEN_WIDTH - 1),
        static_cast<SHORT>(SCREEN_HEIGHT - 1)
    };

    if (!SetConsoleWindowInfo(m_hConsole, TRUE, &windowRect))
    {
        auto error = GetLastError();
        TRPG_ERROR("ERROR: " + std::to_string(error));
        throw("Failed to set the console window info!");
    }

    if (!SetConsoleActiveScreenBuffer(m_hConsole))
    {
        throw("Failed to set the active screen buffer!");
    }

    m_hConsoleWindow = GetConsoleWindow();

    if (!m_hConsoleWindow)
    {
        throw("Failed to get the console window handle!");
    }

    ShowWindow(m_hConsoleWindow, SW_RESTORE);

    if (!GetWindowRect(m_hConsoleWindow, &m_ConsoleWindowRect))
    {
        throw("Failed to get the Window Rect when creating the console!");
    }

    int windowWidth = m_ConsoleWindowRect.right - m_ConsoleWindowRect.left;
    int windowHeight = m_ConsoleWindowRect.bottom - m_ConsoleWindowRect.top;

    int posX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
    int posY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;

    SetWindowPos(
        m_hConsoleWindow,
        nullptr,
        posX,
        posY,
        0,
        0,
        SWP_NOSIZE | SWP_NOZORDER
    );

    m_pScreen = std::make_unique<wchar_t[]>(BUFFER_SIZE);

    ClearBuffer();

    if (!ShowConsoleCursor(false))
    {
        throw("Failed to hide the console cursor!");
    }
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
