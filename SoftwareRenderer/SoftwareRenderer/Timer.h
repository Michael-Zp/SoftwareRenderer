#pragma once
#include <chrono>

class Timer
{
public:
	bool IsInitialized() const { return isInitialized; };
	void Initialize() {
		lastSavedTime = std::chrono::steady_clock::now();
		deltaTime = 0.016;
		totalTime = deltaTime;
		isInitialized = true;
	}
	void Update() {
		auto now = std::chrono::steady_clock::now();
		deltaTime = (float)std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSavedTime).count() / std::chrono::milliseconds::period::den;
		totalTime += deltaTime;
		lastSavedTime = now;
	}
	float GetDeltaTime() const { return deltaTime; };
	float GetTotalTime() const { return totalTime; };

private:
	bool isInitialized = false;

	std::chrono::steady_clock::time_point lastSavedTime;
	float deltaTime;
	float totalTime;
};

