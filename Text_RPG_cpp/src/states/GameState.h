#pragma once
#include "IState.h"
#include "../Selector.h"

class Console;
class Keyboard;
class StateMachine;

class GameState : public IState
{
private:
	Console& m_Console;
	Keyboard& m_Keyboard;
	StateMachine& m_StateMachine;

	Selector<> m_Selector;

public:
	GameState(Console& console, Keyboard& keyboard, StateMachine& stateMachine);
	~GameState();

	void OnEnter() override;
	void OnExit() override;
	void Update() override;
	void Draw() override;
	void ProcessInputs() override;
	bool Exit() override;
};