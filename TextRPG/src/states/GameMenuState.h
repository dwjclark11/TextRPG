#pragma once
#include "IState.h"
#include "../Selector.h"

class Party;
class Console;
class StateMachine;
class Keyboard;
class Player;

class GameMenuState : public IState
{
  private:
	const int PANEL_BARS = 90;
	const int MENU_SIZE = 27;
	const int CLEAR_ROW = 90;
	const int SMALL_PANEL_BAR = 29;

	Party& m_Party;
	Console& m_Console;
	StateMachine& m_StateMachine;
	Keyboard& m_Keyboard;

	Selector<> m_MenuSelector;
	Selector<std::shared_ptr<Player>> m_PlayerSelector;

	bool m_bExitGame, m_bInMenuSelect;
	int m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX;
	int m_FirstChoice, m_SecondChoice;

	enum class SelectType
	{
		ITEM = 0,
		MAGIC,
		EQUIPMENT,
		STATS,
		ORDER,
		NONE,
	};

	SelectType m_eSelectType;

	void DrawPanels();
	void DrawPlayerInfo();

	void OnMenuSelect(int index, std::vector<std::wstring> data);
	void OnPlayerSelect(int index, std::vector<std::shared_ptr<Player>> data);
	void OnDrawPlayerSelect(int x, int y, std::shared_ptr<Player> player);
	void SetOrderPlacement(int playerPosition);
	void UpdatePlayerOrder();

  public:
	GameMenuState(Party& party, Console& console, StateMachine& stateMachine, Keyboard& keyboard);
	~GameMenuState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;
};