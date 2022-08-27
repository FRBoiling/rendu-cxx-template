/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_DURATION_H_
#define RENDU_DURATION_H_

#include <chrono>

/// Milliseconds shorthand typedef.
typedef std::chrono::milliseconds Milliseconds;

/// Seconds shorthand typedef.
typedef std::chrono::seconds Seconds;

/// Minutes shorthand typedef.
typedef std::chrono::minutes Minutes;

/// Hours shorthand typedef.
typedef std::chrono::hours Hours;

/// time_point shorthand typedefs
typedef std::chrono::steady_clock::time_point TimePoint;
typedef std::chrono::system_clock::time_point SystemTimePoint;

/// Makes std::chrono_literals globally available.
using namespace std::chrono_literals;

constexpr std::chrono::hours operator""_days(unsigned long long days)
{
    return std::chrono::hours(days * 24h);
}

#endif // RENDU_DURATION_H_
