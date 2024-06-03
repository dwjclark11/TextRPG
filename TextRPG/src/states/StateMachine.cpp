#include "StateMachine.h"

StateMachine::StateMachine()
	: m_States()
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::PushState(StatePtr newState)
{
	m_States.push(std::move(newState));
	m_States.top()->OnEnter();
}

StatePtr StateMachine::PopState()
{
	if (m_States.empty())
		return nullptr;

	auto oldState = std::move(m_States.top());

	m_States.pop();

	oldState->OnExit();

	return oldState;
}

StatePtr& StateMachine::GetCurrentState()
{
	return m_States.top();
}
