#pragma once

struct Button
{
	bool m_bIsDown{false}, m_bIsJustPressed{false}, m_bIsJustReleased{false};
	void Update(bool pressed);
};