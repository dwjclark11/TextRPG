#pragma once
#include "IState.h"
#include "ui/Selector.h"
#include "items/Equipment.h"
#include "stats/Stats.h"

class Player;

class StatusMenuState : public IState
{
  public:
	StatusMenuState(Player& player, StateContext& context );
	~StatusMenuState();

	virtual void OnEnter() override;
	virtual void OnExit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void ProcessInputs() override;

	virtual bool Exit() override;

private:
	void DrawStatusPanel();
	void DrawPlayerInfo();

private:
	StateContext& m_Context;
	Player& m_Player;

	int m_ScreenWidth;
	int m_ScreenHeight;
	int m_CenterScreenW;
	int m_PanelBarX;
	int m_DiffPosY;
};
