#pragma once
#include <memory>
#include <stack>
#include "IState.h"

using StatePtr = std::unique_ptr<IState>;

class StateMachine
{
  public:
	StateMachine();
	~StateMachine();

	void PushState(StatePtr newState);
	StatePtr PopState();
	const bool Empty() const { return m_States.empty(); }
	StatePtr& GetCurrentState();

private:
	std::stack<StatePtr> m_States;
};
