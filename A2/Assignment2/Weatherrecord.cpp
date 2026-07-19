#include "WeatherRecord.h"

WeatherRecord::WeatherRecord()
{
    // All members are initialised in the header (zeros, flags false).
}

void WeatherRecord::SetDate(const Date & date)
{
    m_date = date;
}

void WeatherRecord::SetTime(const Time & time)
{
    m_time = time;
}

void WeatherRecord::SetWindSpeed(double windSpeed)
{
    m_windSpeed = windSpeed;
    m_validWind = true;
}

void WeatherRecord::SetAmbientTemp(double ambientTemp)
{
    m_ambientTemp = ambientTemp;
    m_validTemp = true;
}

void WeatherRecord::SetSolarRad(double solarRad)
{
    m_solarRad = solarRad;
    m_validSolar = true;
}

Date WeatherRecord::GetDate() const
{
    return m_date;
}

Time WeatherRecord::GetTime() const
{
    return m_time;
}

double WeatherRecord::GetWindSpeed() const
{
    return m_windSpeed;
}

double WeatherRecord::GetAmbientTemp() const
{
    return m_ambientTemp;
}

double WeatherRecord::GetSolarRad() const
{
    return m_solarRad;
}

bool WeatherRecord::IsValidWind() const
{
    return m_validWind;
}

bool WeatherRecord::IsValidTemp() const
{
    return m_validTemp;
}

bool WeatherRecord::IsValidSolar() const
{
    return m_validSolar;
}

// Ordering used by the binary search tree. Date first, then time, so that two
// readings taken on the same day remain distinct keys.
bool operator<(const WeatherRecord & lhs, const WeatherRecord & rhs)
{
    Date ld{lhs.GetDate()};
    Date rd{rhs.GetDate()};

    if (ld.GetYear()  != rd.GetYear())
        return ld.GetYear()  < rd.GetYear();
    if (ld.GetMonth() != rd.GetMonth())
        return ld.GetMonth() < rd.GetMonth();
    if (ld.GetDay()   != rd.GetDay())
        return ld.GetDay()   < rd.GetDay();

    Time lt{lhs.GetTime()};
    Time rt{rhs.GetTime()};

    if (lt.GetHour()   != rt.GetHour())
        return lt.GetHour()   < rt.GetHour();
    if (lt.GetMinute() != rt.GetMinute())
        return lt.GetMinute() < rt.GetMinute();
    return lt.GetSecond() < rt.GetSecond();
}
