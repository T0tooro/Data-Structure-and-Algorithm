#pragma once
#include <iostream>
/**
 * @class Date
 * @brief Stores a calendar date as day, month and year.
 */
class Date
{
public:
    /**
     * @brief Default constructor. Instantiates a baseline date set to zero indices.
     */
    Date();

    /**
     * @brief Parameterized initialization constructor. Sets target dates.
     * @param day Day coordinate identifier mapping.
     * @param month Month coordinate identifier mapping.
     * @param year Year coordinate identifier mapping.
     */
    Date(int day, int month, int year);

    /**
     * @brief Accessor retrieving the current day value.
     * @return Integer value representing the calendar day.
     */
    int GetDay() const;

    /**
     * @brief Accessor retrieving the current month value.
     * @return Integer value representing the calendar month.
     */
    int GetMonth() const;

    /**
     * @brief Accessor retrieving the current year value.
     * @return Integer value representing the calendar year.
     */
    int GetYear() const;

    /**
     * @brief Programmatic modifier updating state tracking fields.
     * @param day Dynamic parameter processing target day adjustments.
     * @param month Dynamic parameter processing target month adjustments.
     * @param year Dynamic parameter processing target year adjustments.
     */
    void SetDate(int day, int month, int year);

private:
    int m_day{0};
    int m_month{0};
    int m_year{0};
};

std::ostream& operator<<(std::ostream& os, const Date& d);
bool operator<(const Date& lhs, const Date& rhs);   // the comparison
