#pragma once
#include <memory>
#include <stack>
#include "IState.h"

typedef std::unique_ptr<IState> StatePtr;

class StateMachine
{
  private:
	std::stack<StatePtr> m_States;

  public:
	StateMachine();
	~StateMachine();

	void PushState(StatePtr newState);
	StatePtr PopState();
	const bool Empty() const { return m_States.empty(); }
	StatePtr& GetCurrentState();
};