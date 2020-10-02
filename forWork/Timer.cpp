#include <chrono>
#include "Timer.h"
namespace antonov {
	

	
	void Timer::change()
	{
		beenUsed = true;
		if (isRunning) {
			dur +=	
				(std::chrono::high_resolution_clock::now() - lastStart);
		}
		else {
			lastStart = std::chrono::high_resolution_clock::now();
		}
		onChange();
		isRunning = !isRunning;
	}

	Timer::Timer(std::function<void()> f) : 
		onChange(f) ,
		lastStart(std::chrono::high_resolution_clock::now()),
		dur(std::chrono::high_resolution_clock::duration(0)),
		isRunning(false)
	{}

	long long Timer::getTimeMilli()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(dur +	
				std::chrono::high_resolution_clock::now() 
				- lastStart
				).count();
	}

	std::chrono::high_resolution_clock::duration Timer::getDUR()
	{
		return (dur + std::chrono::high_resolution_clock::now() - lastStart);
	}

	Timer::~Timer()
	{
		if (isRunning) change();
	}


}