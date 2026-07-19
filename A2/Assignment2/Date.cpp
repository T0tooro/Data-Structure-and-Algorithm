#include "Date.h"

Date::Date()
{
}

Date::Date(int day, int month, int year)
    : m_day{day}, m_month{month}, m_year{year}
{
}

int Date::GetDay() const { return m_day; }
int Date::GetMonth() const { return m_month; }
int Date::GetYear() const { return m_year; }

void Date::SetDate(int day, int month, int year)
{
    m_day = day;
    m_month = month;
    m_year = year;
}
