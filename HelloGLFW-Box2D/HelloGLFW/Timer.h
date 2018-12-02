#pragma once
#include<time.h>

class Timer
{

private:
	clock_t begin, end;
	bool notEnded = true;

public:

	Timer()
	{
		this->reset();
	}

	void start()
	{
		this->begin = clock();

		while (notEnded) {
			begin++;
		}
	}

	void finish()
	{
		this->end = clock();
	}

	void reset()
	{
		this->begin = 0;
		this->end = 0;
	}

	double getElapsedTime()
	{
		//return ((double)(this->end - this->begin)) / CLOCKS_PER_SEC;
		return begin;
	}

	double getElapsedTimeMs()
	{
		return this->getElapsedTime() * 1000;
	}
};