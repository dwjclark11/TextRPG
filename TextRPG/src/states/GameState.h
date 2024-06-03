#pragma once
#include "IState.h"
#include "../Selector.h"
#include "../Player.h"
#include "../Party.h"
#include "../Inventory.h"
#include <memory>
#include "../utilities/Timer.h"
#include "../utilities/Typewriter.h"

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
	std::unique_ptr<Party> m_Party;
	Timer m_Timer;

	// Testing
	Typewriter m_TypeWriter;

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