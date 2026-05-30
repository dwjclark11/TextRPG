#pragma once
#include "IState.h"
#include "ui/Selector.h"

#include "utilities/Timer.h"

#include <memory>

class Console;
class Keyboard;
class StateMachine;
class Party;

class GameState : public IState
{
  public:
	GameState(StateContext& context);
	~GameState();

	void OnEnter() override;
	void OnExit() override;
	void Update() override;
	void Draw() override;
	void ProcessInputs() override;
	bool Exit() override;

private:
	StateContext& m_Context;

	Selector<> m_Selector;
	
	Timer m_Timer;
};