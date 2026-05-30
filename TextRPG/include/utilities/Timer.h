#pragma once
#include <chrono>

class Timer
{
  public:
	Timer();
	~Timer() = default;

	void Start();
	void Stop();
	void Pause();
	void Resume();

	const int64_t ElapsedMS();
	const int64_t ElapsedSec();

	inline const bool IsRunning() const { return m_bIsRunning; }
	inline const bool IsPaused() const { return m_bIsPaused; }

private:
	std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;
	std::chrono::time_point<std::chrono::steady_clock> m_PausedPoint;
	bool m_bIsRunning;
	bool m_bIsPaused;
};
