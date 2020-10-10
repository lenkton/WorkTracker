#pragma once
#include  <functional>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
namespace antonov {
	class Timer {
	public:
		typedef  std::chrono::high_resolution_clock clock;
		bool isRun() { return isRunning; }
		void change();
		Timer(std::function<void()> f);
		long long getTimeMilli();
		clock::duration getDUR();
		~Timer();
		std::wstring toWstring();
	
	private:
		bool beenUsed = false;
		std::chrono::time_point<clock> lastStart;
		clock::duration dur;
		std::function<void()> onChange;
		bool isRunning;
	};
}