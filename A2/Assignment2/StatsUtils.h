#pragma once
#include "Vector.h"
#include <cmath>

/**
 * @brief Global template calculation utility functions.
 * Operates independently of specific record models or structures.
 */

template <typename T>
T CalculateTotal(const Vector<T> & data)
{
    T total{0};
    int size{data.Size()};
    for (int i{0}; i < size; ++i)
    {
        total += data[i];
    }
    return total;
}

template <typename T>
double CalculateAverage(const Vector<T> & data)
{
    int size{data.Size()};
    if (size == 0)
        return 0.0;
    return static_cast<double>(CalculateTotal(data)) / size;
}

template <typename T>
double CalculateStandardDeviation(const Vector<T> & data)
{
    int size{data.Size()};
    if (size <= 1)
        return 0.0;

    double avg{CalculateAverage(data)};
    double sumSqDiff{0.0};

    for (int i{0}; i < size; ++i)
    {
        double diff{static_cast<double>(data[i]) - avg};
        sumSqDiff += diff * diff;
    }
    return std::sqrt(sumSqDiff / (size - 1));
}

/**
 * @brief Mean Absolute Deviation of a set of values.
 *
 * The average of the absolute differences between each value and the mean.
 * Unlike the standard deviation it does not square the differences, so a
 * single extreme value moves it less.
 *
 * @param  data - the values to measure
 * @return double - the mean absolute deviation, 0.0 if there is no data
 */
template <typename T>
double CalculateMAD(const Vector<T> & data)
{
    int size{data.Size()};
    if (size == 0)
        return 0.0;

    double avg{CalculateAverage(data)};
    double sumAbsDiff{0.0};

    for (int i{0}; i < size; ++i)
    {
        sumAbsDiff += std::fabs(static_cast<double>(data[i]) - avg);
    }
    return sumAbsDiff / size;
}

/**
 * @brief Sample Pearson Correlation Coefficient of two paired sets of values.
 *
 * Measures how strongly the two sets move together, from -1 to +1. The values
 * at the same index in each set form one pair, so both sets must be the same
 * size.
 *
 * @param  x - the first set of values
 * @param  y - the second set of values, paired by index with x
 * @return double - the coefficient, 0.0 if the sets are too small,
 *         mismatched in size, or either set has no variation
 */
template <typename T>
double CalculateSPCC(const Vector<T> & x, const Vector<T> & y)
{
    int size{x.Size()};
    if (size != y.Size() || size <= 1)
        return 0.0;

    double sumX{0.0};
    double sumY{0.0};
    double sumXY{0.0};
    double sumXX{0.0};
    double sumYY{0.0};

    for (int i{0}; i < size; ++i)
    {
        double xi{static_cast<double>(x[i])};
        double yi{static_cast<double>(y[i])};
        sumX  += xi;
        sumY  += yi;
        sumXY += xi * yi;
        sumXX += xi * xi;
        sumYY += yi * yi;
    }

    double n{static_cast<double>(size)};
    double numerator{(n * sumXY) - (sumX * sumY)};
    double denominator{std::sqrt(((n * sumXX) - (sumX * sumX)) *
                                 ((n * sumYY) - (sumY * sumY)))};

    if (denominator == 0.0)
        return 0.0;   // no variation in one of the sets
    return numerator / denominator;
}
