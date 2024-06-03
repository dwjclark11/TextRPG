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

	SelectorParams(int x = 20, int y = 10, int columns = 1, int spacing_x = 20, int spacing_y = 5,
				   std::wstring cursor = L"->")
		: x{x}
		, y{y}
		, columns{columns}
		, currentX{0}
		, currentY{0}
		, spacingX{spacing_x}
		, spacingY{spacing_y}
		, cursor{cursor}
	{
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

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void OnAction();

	void DrawItem(int x, int y, T item);
	void OnSelection(int index, std::vector<T> data);

  public:
	Selector(Console& console, Keyboard& keyboard, std::vector<T> data, SelectorParams params = SelectorParams());
	Selector(Console& console, Keyboard& keyboard, std::function<void(int, std::vector<T>)> on_selection,
			 std::function<void(int, int, T)> on_draw_item, std::vector<T> data,
			 SelectorParams params = SelectorParams());
	~Selector();

	void SetData(std::vector<T> data)
	{
		m_Data = data;
		m_Rows = static_cast<int>(std::ceil(m_Data.size() / (m_Params.columns == 0 ? 1 : m_Params.columns)));

		// Check to see if rows is < 1
		if (m_Rows < 1)
			m_Rows = 1;
	}

	std::vector<T>& GetData() { return m_Data; }

	/*
	 * This sets the OnSelection function that MUST be overwritten for each new selector object.
	 * What each selector does will vary; Therefore, this function should be updated for each new Selector object.
	 * @param This takes in an std::function ~ func(int x, int y, T item).
	 */
	void SetSelectionFunc(std::function<void(int, std::vector<T>)> on_selection) { m_OnSelection = on_selection; }

	/*
	 * This sets the DrawItem function that should be overwritten for each new selector object
	 * @param This takes in an std::function ~ func(int x, int y, T item).
	 */
	void SetDrawFunc(std::function<void(int, int, T)> on_draw_item) { m_OnDrawItem = on_draw_item; }

	void ShowCursor() { m_bShowCursor = true; }
	void HideCursor() { m_bShowCursor = false; }

	/*
	 * @return This returns the current cursor index as an integer value
	 */
	const int GetIndex() const { return m_Params.currentX + (m_Params.currentY * m_Params.columns); }

	/*
	 * Process the user inputs. The move functions and the action functions
	 * are called here.
	 */
	void ProcessInputs();

	/*
	 * Draw the cursor and all of the Selectable items
	 */
	void Draw();
};

template <typename T>
inline Selector<T>::Selector(Console& console, Keyboard& keyboard, std::vector<T> data, SelectorParams params)
	: Selector(
		  console, keyboard, [ this ](int index, std::vector<T> data) { Selector::OnSelection(index, data); },
		  [ this ](int x, int y, T item) { Selector::DrawItem(x, y, item); }, data, params)
{
}

template <typename T>
inline Selector<T>::Selector(Console& console, Keyboard& keyboard,
							 std::function<void(int, std::vector<T>)> on_selection,
							 std::function<void(int, int, T)> on_draw_item, std::vector<T> data, SelectorParams params)
	: m_Console(console)
	, m_Keyboard(keyboard)
	, m_OnSelection(on_selection)
	, m_OnDrawItem(on_draw_item)
	, m_Data(data)
	, m_Params(params)
	, m_bShowCursor(true)
{
	// Initialize the number of rows
	m_Rows = std::ceil(m_Data.size() / (params.columns == 0 ? 1 : params.columns));

	// Check to see if rows is < 1
	if (m_Rows < 1)
		m_Rows = 1;
}

template <typename T>
inline Selector<T>::~Selector()
{
}

template <typename T>
inline void Selector<T>::ProcessInputs()
{
	if (m_Keyboard.IsKeyJustPressed(KEY_W))
		MoveUp();
	else if (m_Keyboard.IsKeyJustPressed(KEY_S))
		MoveDown();
	else if (m_Keyboard.IsKeyJustPressed(KEY_A))
		MoveLeft();
	else if (m_Keyboard.IsKeyJustPressed(KEY_D))
		MoveRight();
	else if (m_Keyboard.IsKeyJustPressed(KEY_SPACE))
		OnAction();
}

template <typename T>
inline void Selector<T>::MoveUp()
{
	m_Params.currentY = std::max(m_Params.currentY - 1, 0);
}

template <typename T>
inline void Selector<T>::MoveDown()
{
	m_Params.currentY = std::min(m_Params.currentY + 1, m_Rows - 1);
}

template <typename T>
inline void Selector<T>::MoveLeft()
{
	m_Params.currentX = std::max(m_Params.currentX - 1, 0);
}

template <typename T>
inline void Selector<T>::MoveRight()
{
	m_Params.currentX = std::min(m_Params.currentX + 1, m_Params.columns - 1);
}

template <typename T>
inline void Selector<T>::OnAction()
{
	int index = GetIndex();
	m_OnSelection(index, m_Data);
}

template <typename T>
inline void Selector<T>::DrawItem(int x, int y, T item)
{
	// Check to see if the type is wstring
	if constexpr (std::is_same<T, std::wstring>::value)
	{
		m_Console.Write(x, y, item);
		return;
	}

	// Get the type and let user know to override
	std::string type = typeid(item).name();
	TRPG_ERROR("Data type [" + type + "] - needs to be a wstring, Please create the DrawItem function to override!");
}

// This function should be overriden
template <typename T>
inline void Selector<T>::OnSelection(int index, std::vector<T> data)
{
	m_Console.Write(50, 20, L"Index: " + std::to_wstring(index));
}

template <typename T>
inline void Selector<T>::Draw()
{
	// If there is not data, nothing to draw
	if (m_Data.empty())
		return;

	int itemIndex = 0;
	int x = m_Params.x;
	int y = m_Params.y;
	int rowHeight = m_Params.spacingY;
	int spacingX = m_Params.spacingX;

	int maxData = m_Data.size();

	for (int i = 0; i < m_Rows; i++)
	{
		for (int j = 0; j < m_Params.columns; j++)
		{
			if (i == m_Params.currentY && j == m_Params.currentX)
			{
				if (m_bShowCursor)
				{
					// Reset the areas behind the cursor as it moves
					if (m_Params.currentY != 0)
						m_Console.Write(x - (x == 0 ? 0 : 2), y - rowHeight, L" ");

					if (m_Params.currentX != 0)
						m_Console.Write(x - (x == 0 ? 0 : 2) - spacingX, y, L" ");

					if (m_Params.currentY != m_Rows - 1)
						m_Console.Write(x - (x == 0 ? 0 : 2), y + rowHeight, L" ");

					if (m_Params.currentX != m_Params.columns - 1)
						m_Console.Write(x - (x == 0 ? 0 : 2) + spacingX, y, L" ");

					// Draw the cursor
					m_Console.Write(x - (x == 0 ? 0 : 2), y, m_Params.cursor, RED);
				}
				else
				{
					// Clear the cursor
					m_Console.Write(x - (x == 0 ? 0 : 2), y, L" ");
				}
			}

			if (itemIndex < maxData)
			{
				T item = m_Data[ itemIndex ];
				m_OnDrawItem(x, y, item);
				x += spacingX;
				itemIndex++;
			}
		}
		y += rowHeight;
		x = m_Params.x;
	}
}
