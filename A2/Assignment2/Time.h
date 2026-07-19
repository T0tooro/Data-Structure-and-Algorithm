#pragma once

/**
 * @class Time
 * @brief Stores a time value as hours, minutes and seconds.
 */
class Time
{
public:
    /**
     * @brief Default constructor. Instantiates standard zeroed timeline components.
     */
    Time();

    /**
     * @brief Parameterized implementation constructor tracking time profiles.
     * @param hour Hour values parameter.
     * @param minute Minute values parameter.
     * @param second Second values parameter defaulting to zero tracking fields.
     */
    Time(int hour, int minute, int second = 0);

    /**
     * @brief Accessor tracking internal hour configuration state values.
     * @return Integer tracking designated hour configurations.
     */
    int GetHour() const;

    /**
     * @brief Accessor tracking internal minute configuration state values.
     * @return Integer tracking designated minute configurations.
     */
    int GetMinute() const;

    /**
     * @brief Accessor tracking internal second configuration state values.
     * @return Integer tracking designated second configurations.
     */
    int GetSecond() const;

    /**
     * @brief Direct mutation updates modifying stored timeline tracking parameters.
     * @param hour Mutator parameter modifying localized values.
     * @param minute Mutator parameter modifying localized values.
     * @param second Mutator parameter modifying localized values.
     */
    void SetTime(int hour, int minute, int second);

private:
    int m_hour{0};
    int m_minute{0};
    int m_second{0};
};
