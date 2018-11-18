#pragma once


class CTimer
{
public:
    using ticks_t = unsigned __int64;

    CTimer();

    ticks_t getNow();
    double toSeconds(ticks_t time);
    double toMilliseconds(ticks_t time);

    double toSecondsInterval(ticks_t time);
    double toMilliSecondsInterval(ticks_t time);

    static inline CTimer& getInstance()
    {
        return st_instance;
    }

private:
    ticks_t         m_base;
    ticks_t         m_freq;

    static CTimer   st_instance;
};