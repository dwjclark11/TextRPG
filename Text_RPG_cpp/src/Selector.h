#pragma once

#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <type_traits>
#include "Console.h"
#include "inputs/Keyboard.h"
#include "Logger.h"

struct SelectorParams
{
	int x, y, columns, currentX, currentY, spacingX, spacingY;
	std::wstring cursor;

	SelectorParams(
		int x = 20, int y = 10, int columns = 1, int current_x = 0, int current_y = 0,
		int spacing_x = 20, int spacing_y = 5, std::wstring cursor = L"->"
	)
	{
		this->x = x;
		this->y = y;
		this->columns = columns;
		this->currentX = current_x;
		this->currentY = current_y;
		this->spacingX = spacing_x;
		this->spacingY = spacing_y;
		this->cursor = cursor;
	}
};

template <typename T = std::wstring>
class Selector
{
private:
	Console& m_Console;
	Keyboard& m_Keyboard;

	std::function<void(int, std::vector<T>)> m_OnSelection;
	std::function<void(int, int, T)> m_OnDrawItem;
	std::vector<T> m_Data;
	SelectorParams m_Params;
	bool m_bShowCursor;
	int m_Rows;

public:
	Selector();
	~Selector();

	void SetData(std::vector<T> data) { mData = data; }
	const std::vector<T>& GetData() { return mData; }
	void SetSelectionFunc(std::function<void(int, std::vector<T>)> on_selection) { m_OnSelection = on_selection; }
	void SetDrawFunc(std::function<void(int, int, T)> on_draw_item) { m_OnDrawItem = on_draw_item; }
	void ShowCursor() { m_bShowCursor = true; }
	void HideCursor() { m_bShowCursor = false; }
	const int GetIndex() const { return m_Params.currentX + (m_Params.currentY * m_Params.columns); }
	
	void ProcessInputs();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void OnAction();

	void DrawItem(int x, int y, T item);
	void OnSelection(int index, std::vector<T> data);

	void Draw();
};