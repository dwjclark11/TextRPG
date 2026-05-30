#pragma once

struct Button
{
	bool bIsDown{ false };
	bool bIsJustPressed{ false };
	bool bIsJustReleased{ false };

	void Update(bool pressed);
};