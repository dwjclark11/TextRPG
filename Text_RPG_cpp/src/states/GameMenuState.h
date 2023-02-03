#pragma once
#include "IState.h"

class Party;
class Console;
class StateMachine;
class Keyboard;

class GameMenuState : public IState
{
private:
	Party& m_Party;
	Console& m_Console;
	StateMachine& m_StateMachine;
	Keyboard& m_Keyboard;

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