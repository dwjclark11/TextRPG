#pragma once
#include <states/IState.h>

#include <memory>
#include <Windows.h>

class Game
{ 
  public:
	Game();
	~Game();

	void Run();

private:
	bool Init();

	void ProcessEvents();
	void ProcessInputs();
	void Update();
	void Draw();

	void KeyEventProcess(KEY_EVENT_RECORD keyEvent);

private:	
	std::unique_ptr<class Console> m_pConsole;
	std::unique_ptr<class Keyboard> m_pKeyboard;
	std::unique_ptr<class StateMachine> m_pStateMachine;
	std::unique_ptr<class Party> m_pParty;
	StateContext m_Context;

	bool m_bIsRunning;

	DWORD m_NumRead;
	INPUT_RECORD m_InRecBuf[128];
	HANDLE m_hConsoleIn;
};
