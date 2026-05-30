#include "utilities/Globals.h"
#include "utilities/Utilities.h"

TRPG_Globals::TRPG_Globals()
	: m_GameTime{ 0 }
	, m_SavedGameTime{ 0 }
	, m_Timer{}

{
	m_Timer.Start();
}

TRPG_Globals& TRPG_Globals::GetInstance()
{
	static TRPG_Globals instance{};
	return instance;
}

void TRPG_Globals::Update()
{
	m_GameTime = m_Timer.ElapsedSec() + m_SavedGameTime;
}

const std::wstring TRPG_Globals::GetTime()
{
	int hours = m_GameTime / 3600;
	int minutes = (m_GameTime % 3600) / 60;
	int seconds = m_GameTime % 60;

	std::wstring time{
		Utilities::PadNumbers(hours) +
		L":" +
		Utilities::PadNumbers(minutes) +
		L":"
		+ Utilities::PadNumbers(seconds)
	};

	return time;
}
