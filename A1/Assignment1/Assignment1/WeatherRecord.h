#pragma once
#include "Date.h"
#include "Time.h"

/**
 * @class WeatherRecord
 * @brief Holds one weather log entry: a date, a time, and three sensor readings.
 *
 * The three readings are wind speed, ambient temperature and solar radiation.
 * Each reading has its own valid flag that records whether the field was present
 * in the data file, so a row with one offline sensor can still keep its other
 * readings. The fields are private: a value can only be set through its setter,
 * which also sets the matching valid flag, so a reading and its flag can never
 * disagree.
 *
 * @author <your name>
 * @version 01
 * @date 13/06/2026, Created
 */
class WeatherRecord
{
public:

    /**
     * @brief Default constructor. All readings are zero and all valid flags are false.
     */
    WeatherRecord();

    /**
     * @brief Sets the date the reading was taken.
     * @param date - the date to store
     * @return void
     */
    void SetDate(const Date & date);

    /**
     * @brief Sets the time the reading was taken.
     * @param time - the time to store
     * @return void
     */
    void SetTime(const Time & time);

    /**
     * @brief Stores the wind speed and marks the wind reading as valid.
     * @param windSpeed - wind speed in metres per second
     * @return void
     */
    void SetWindSpeed(double windSpeed);

    /**
     * @brief Stores the ambient temperature and marks the temperature reading as valid.
     * @param ambientTemp - temperature in degrees Celsius
     * @return void
     */
    void SetAmbientTemp(double ambientTemp);

    /**
     * @brief Stores the solar radiation and marks the solar reading as valid.
     * @param solarRad - solar radiation in watts per square metre
     * @return void
     */
    void SetSolarRad(double solarRad);

    /**
     * @brief Returns the date the reading was taken.
     * @return Date - the stored date
     */
    Date GetDate() const;

    /**
     * @brief Returns the time the reading was taken.
     * @return Time - the stored time
     */
    Time GetTime() const;

    /**
     * @brief Returns the wind speed.
     * @return double - wind speed in metres per second
     */
    double GetWindSpeed() const;

    /**
     * @brief Returns the ambient temperature.
     * @return double - temperature in degrees Celsius
     */
    double GetAmbientTemp() const;

    /**
     * @brief Returns the solar radiation.
     * @return double - solar radiation in watts per square metre
     */
    double GetSolarRad() const;

    /**
     * @brief Reports whether a wind speed was parsed for this record.
     * @return bool - true if the wind reading is valid
     */
    bool IsValidWind() const;

    /**
     * @brief Reports whether a temperature was parsed for this record.
     * @return bool - true if the temperature reading is valid
     */
    bool IsValidTemp() const;

    /**
     * @brief Reports whether a solar radiation value was parsed for this record.
     * @return bool - true if the solar reading is valid
     */
    bool IsValidSolar() const;

private:

    /// Date the reading was taken
    Date m_date;
    /// Time the reading was taken
    Time m_time;
    /// Wind speed in metres per second
    double m_windSpeed{0.0};
    /// Ambient temperature in degrees Celsius
    double m_ambientTemp{0.0};
    /// Solar radiation in watts per square metre
    double m_solarRad{0.0};
    /// True if a wind speed was parsed
    bool m_validWind{false};
    /// True if a temperature was parsed
    bool m_validTemp{false};
    /// True if a solar radiation value was parsed
    bool m_validSolar{false};
};
