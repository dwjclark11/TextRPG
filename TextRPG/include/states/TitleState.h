#pragma once
#include "IState.h"
#include "ui/Selector.h"

class Timer;

class TitleState : public IState
{
public:
	TitleState(StateContext& context);
	~TitleState();

	void OnEnter()		override;
	void OnExit()		override;
	void Update()		override;
	void Draw()			override;
	void ProcessInputs() override;
	bool Exit() override;
	void OnMenuSelect(int index, std::vector<std::wstring> data);

private:
	void DrawTitleScreen();

private:
	StateContext& m_Context;

	Selector<> m_Selector;
	bool m_bExitGame;
	bool m_bShowContinue;
};
