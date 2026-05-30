#pragma once
#include "IState.h"
#include "ui/Selector.h"

class Player;

class GameMenuState : public IState
{
  public:
	GameMenuState(StateContext& context);
	~GameMenuState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	void DrawPanels();
	void DrawPlayerInfo();

	void OnMenuSelect(int index, std::vector<std::wstring> data);
	void OnPlayerSelect(int index, std::vector<std::shared_ptr<Player>> data);
	void OnDrawPlayerSelect(int x, int y, std::shared_ptr<Player> player);
	void SetOrderPlacement(int playerPosition);
	void UpdatePlayerOrder();

private:
	StateContext& m_Context;

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
};