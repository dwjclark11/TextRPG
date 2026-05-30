#pragma once
#include <memory>
#include <string>
#include "Timer.h"

class TRPG_Globals
{
  public:
	static TRPG_Globals& GetInstance();

	TRPG_Globals(const TRPG_Globals&) = delete;
	TRPG_Globals& operator=(const TRPG_Globals&) = delete;
	TRPG_Globals(TRPG_Globals&&) = delete;
	TRPG_Globals& operator=(TRPG_Globals&&) = delete;

	const int GetGameTime() const { return m_GameTime; }
	void SetSaveGameTime(int saved_time) { m_SavedGameTime = saved_time; }
	void Update();

	[[nodiscard]] const std::wstring GetTime();

private:
	int m_GameTime;
	int m_SavedGameTime;
	Timer m_Timer;

	TRPG_Globals();
	~TRPG_Globals() = default;
};
