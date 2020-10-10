#include <chrono>
#include "Timer.h"
namespace antonov {
	
	void Timer::change()
	{
		beenUsed = true;
		if (isRunning) {
			dur +=	
				(Timer::clock::now() - Timer::lastStart);
		}
		else {
			lastStart = Timer::clock::now();
		}
		onChange();
		isRunning = !isRunning;
	}

	Timer::Timer(std::function<void()> f) :
		onChange(f),
		lastStart(Timer::clock::now()),
		dur(),
		isRunning(false)
	{}

	long long Timer::getTimeMilli()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>
			(dur +	
				Timer::clock::now()
				- lastStart
				).count();
	}

	Timer::clock::duration Timer::getDUR(){
		return (dur + Timer::clock::now() - Timer::lastStart);
	}

	Timer::~Timer(){
		if (isRunning) change();
	}
	std::wstring Timer::toWstring()
	{
		std::wostringstream out{};
		auto t = getTimeMilli();
		if (!beenUsed)t = 0;

		out << std::setfill(L'0')
			<< std::setw(2) << (t / 3600000) << L":"
			<< std::setw(2) << ((t / 60000) % 60) << L":"
			<< std::setw(2) << ((t / 1000) % 60) << L","
			<< std::setw(3) << (t % 1000);
		return out.str();
	}
}