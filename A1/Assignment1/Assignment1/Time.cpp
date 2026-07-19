#include "Time.h"

Time::Time()
{
}

Time::Time(int hour, int minute, int second)
    : m_hour{hour}, m_minute{minute}, m_second{second}
{
}

int Time::GetHour() const { return m_hour; }
int Time::GetMinute() const { return m_minute; }
int Time::GetSecond() const { return m_second; }

void Time::SetTime(int hour, int minute, int second)
{
    m_hour = hour;
    m_minute = minute;
    m_second = second;
}
