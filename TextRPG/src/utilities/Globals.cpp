#include "Globals.h"
#include "trpg_utilities.h"

std::unique_ptr<TRPG_Globals> TRPG_Globals::m_pInstance = nullptr;

TRPG_Globals::TRPG_Globals()
	: m_GameTime{0}
	, m_SavedGameTime{0}
	, m_Timer{}

{
	m_Timer.Start();
}

TRPG_Globals& TRPG_Globals::GetInstance()
{
	if (!m_pInstance)
		m_pInstance.reset(new TRPG_Globals());

	return *m_pInstance;
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

	std::wstring time{PadNumbers(hours) + L":" + PadNumbers(minutes) + L":" + PadNumbers(seconds)};

	return time;
}
