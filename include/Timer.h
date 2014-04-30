/*
 * Timer.h
 *
 *  Created on: 30 avr. 2014
 *      Author: bastien
 */

#ifndef TIMER_H_
#define TIMER_H_

#ifndef _WIN32
#include <sys/time.h>
#endif

class Timer
{

public:

	/**
	 * Constructor
	 */
	explicit Timer(void)
	{
		#ifdef _WIN32
			QueryPerformanceFrequency(&m_frequency);
		#endif
	}

	/**
	 * Destructor
	 */
	virtual ~Timer(void)
	{
		// ...
	}

	/**
	 * Start the timer
	 */
	void Start(void)
	{
		SetTime(&m_startTime);
	}

	/**
	 * Stop the Timer
	 */
	void Stop(void)
	{
		SetTime(&m_endTime);
	}

	/**
	 * Get the time ellapsed in micro-seconds
	 */
	double getElapsedTimeInMs(void)
	{
#ifdef _WIN32
		return((m_endTime.QuadPart - m_startTime.QuadPart)*1000.0 / m_frequency.QuadPart);
#else
		return((m_endTime.tv_sec - m_startTime.tv_sec)*1000.0)+((m_endTime.tv_usec - m_startTime.tv_usec)/1000.0);
#endif
	}

	/**
	 * Get the time ellapsed in seconds
	 */
	double getElapsedTime(void)
	{
#ifdef _WIN32
		return((m_endTime.QuadPart - m_startTime.QuadPart) / m_frequency.QuadPart);
#else
		return((m_endTime.tv_sec - m_startTime.tv_sec))+((m_endTime.tv_usec - m_startTime.tv_usec)*1e-6);
#endif
	}

private:

#ifdef _WIN32

	void SetTime(LARGE_INTEGER * pTime)
	{
		QueryPerformanceCounter(pTime);
	}

    LARGE_INTEGER m_frequency;
    LARGE_INTEGER m_startTime;
    LARGE_INTEGER m_endTime;

#else

	void SetTime(timeval * pTime)
	{
		gettimeofday(pTime, NULL);
	}

	timeval m_startTime;
	timeval m_endTime;

#endif

};

#endif /* TIMER_H_ */
