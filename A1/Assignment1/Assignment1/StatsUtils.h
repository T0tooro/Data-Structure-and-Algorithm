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
    if (size == 0) return 0.0;
    return static_cast<double>(CalculateTotal(data)) / size;
}

template <typename T>
double CalculateStandardDeviation(const Vector<T> & data)
{
    int size{data.Size()};
    if (size <= 1) return 0.0;

    double avg{CalculateAverage(data)};
    double sumSqDiff{0.0};

    for (int i{0}; i < size; ++i)
    {
        double diff{static_cast<double>(data[i]) - avg};
        sumSqDiff += diff * diff;
    }
    return std::sqrt(sumSqDiff / (size - 1));
}
