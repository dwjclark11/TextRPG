#pragma once
#include "IState.h"
#include "../Selector.h"
#include "../Equipment.h"
#include "../Stats.h"

class Console;
class StateMachine;
class Keyboard;
class Player;

class StatusMenuState : public IState
{
  private:
	const int PANEL_BARS = 90;
	const int STATUS_SIZE = 32;
	const int STAT_VAL_X_POS = 70;
	const int STAT_LABEL_X_POS = 50;
	const int STAT_LABEL_START_Y_POS = 30;

	Console& m_Console;
	StateMachine& m_StateMachine;
	Keyboard& m_Keyboard;
	Player& m_Player;

	int m_ScreenWidth, m_ScreenHeight, m_CenterScreenW, m_PanelBarX, m_DiffPosY;

	void DrawStatusPanel();
	void DrawPlayerInfo();

  public:
	StatusMenuState(Player& player, Console& console, StateMachine& stateMachine, Keyboard& keyboard);
	~StatusMenuState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;
};
