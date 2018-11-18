#include "CTimer.h"
#include <Windows.h>

CTimer CTimer::st_instance;

CTimer::CTimer()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&m_freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&m_base);
}

CTimer::ticks_t CTimer::getNow()
{
    ticks_t ret;
    QueryPerformanceCounter((LARGE_INTEGER*)&ret);
    return ret;
}

double CTimer::toSeconds(ticks_t time)
{
    return static_cast<double>(time - m_base) / static_cast<double>(m_freq);
}

double CTimer::toMilliseconds(ticks_t time)
{
    time *= 1000;
    return static_cast<double>(time - m_base) / static_cast<double>(m_freq);
}

double CTimer::toSecondsInterval(ticks_t time)
{
    return static_cast<double>(time) / static_cast<double>(m_freq);
}

double CTimer::toMilliSecondsInterval(ticks_t time)
{
    time *= 1000;
    return static_cast<double>(time) / static_cast<double>(m_freq);
}

