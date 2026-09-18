#ifndef COORDINATES_H
#define COORDINATES_H

#include <algorithm>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <string>

namespace geo {
inline int digits(const std::string& value) {
    if (value.empty() || !std::all_of(value.begin(), value.end(), [](unsigned char c) {
            return std::isdigit(c) != 0;
        })) throw std::invalid_argument("Invalid coordinate number: " + value);
    std::size_t parsed = 0;
    const int result = std::stoi(value, &parsed);
    if (parsed != value.size()) throw std::invalid_argument("Invalid coordinate number");
    return result;
}

// Dataset notation: 31d37lN (degrees, minutes, hemisphere), optionally seconds.
inline double parseCoordinate(const std::string& value, bool latitude) {
    const auto d = value.find('d');
    const auto m = value.find('l');
    if (d == std::string::npos || m == std::string::npos || d == 0 ||
        m <= d + 1 || m >= value.size() - 1)
        throw std::invalid_argument("Malformed coordinate: " + value);
    const char hemisphere = value.back();
    if (latitude ? (hemisphere != 'N' && hemisphere != 'S') :
                   (hemisphere != 'E' && hemisphere != 'W'))
        throw std::invalid_argument("Invalid coordinate hemisphere: " + value);
    const int degrees = digits(value.substr(0, d));
    const int minutes = digits(value.substr(d + 1, m - d - 1));
    std::string secondsText = value.substr(m + 1, value.size() - m - 2);
    if (!secondsText.empty() && secondsText.back() == 's') secondsText.pop_back();
    const int seconds = secondsText.empty() ? 0 : digits(secondsText);
    const int limit = latitude ? 90 : 180;
    if (degrees > limit || minutes >= 60 || seconds >= 60 ||
        (degrees == limit && (minutes != 0 || seconds != 0)))
        throw std::invalid_argument("Coordinate out of range: " + value);
    const double result = degrees + minutes / 60.0 + seconds / 3600.0;
    return (hemisphere == 'S' || hemisphere == 'W') ? -result : result;
}

inline double distanceKm(double lat1, double lon1, double lat2, double lon2) {
    constexpr double pi = 3.14159265358979323846;
    constexpr double radiusKm = 6371.137; // Same approximate spherical radius as original.
    const double dlat = (lat2 - lat1) * pi / 180.0;
    const double dlon = (lon2 - lon1) * pi / 180.0;
    lat1 *= pi / 180.0;
    lat2 *= pi / 180.0;
    const double a = std::pow(std::sin(dlat / 2), 2) +
                     std::cos(lat1) * std::cos(lat2) *
                     std::pow(std::sin(dlon / 2), 2);
    const double bounded = std::clamp(a, 0.0, 1.0);
    return 2.0 * radiusKm * std::atan2(std::sqrt(bounded), std::sqrt(1.0 - bounded));
}
} // namespace geo

#endif
