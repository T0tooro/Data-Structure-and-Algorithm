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

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear()  != rhs.GetYear())  return lhs.GetYear()  < rhs.GetYear();
    if (lhs.GetMonth() != rhs.GetMonth()) return lhs.GetMonth() < rhs.GetMonth();
    return lhs.GetDay() < rhs.GetDay();
}


std::ostream& operator<<(std::ostream& os, const Date& d)
{
    os << d.GetDay() << "/" << d.GetMonth() << "/" << d.GetYear();
    return os;
}
