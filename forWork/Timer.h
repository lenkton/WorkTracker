#pragma once
#include  <functional>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
namespace antonov {
	class Timer {
	private:
		bool beenUsed = false;
		std::chrono::time_point<std::chrono::high_resolution_clock> lastStart;
		std::chrono::high_resolution_clock::duration dur;
		std::function<void()> onChange;
		bool isRunning;
	public:
		bool isRun() { return isRunning; }
		void change();
		Timer(std::function<void()> f);
		long long getTimeMilli();
		std::chrono::high_resolution_clock::duration getDUR();
		~Timer();
		std::wstring toWstring() {
			std::wostringstream out{};
			auto t = getTimeMilli();
			if (!beenUsed)t = 0;

			out<< std::setfill(L'0') 
				<< std::setw(2)		<<(t / 3600000)		<< L":"
				<< std::setw(2)		<< ((t / 60000) % 60)	<< L":"
				<< std::setw(2)		<<( (t / 1000) % 60)	<< L"," 
				<< std::setw(3)		<<(t % 1000);
			return out.str();
		}

	};
}