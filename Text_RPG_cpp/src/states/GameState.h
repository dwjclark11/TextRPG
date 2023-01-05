#pragma once
#include "IState.h"

class Keyboard;
class StateMachine;

class GameState : public IState
{
private:
	Keyboard& m_Keyboard;
	StateMachine& m_StateMachine;
public:
	GameState(Keyboard& keyboard, StateMachine& stateMachine);
	~GameState();

	void OnEnter() override;
	void OnExit() override;
	void Update() override;
	void Draw() override;
	void ProcessInputs() override;
	bool Exit() override;
};