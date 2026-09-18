#include "BinaryTree.h"
#include "Coordinates.h"
#include "binaryTree.cpp" // Template definitions must be visible to this translation unit.

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
using CityTree = binaryTree<std::string, std::string>;

void loadCities(std::istream& input, CityTree& tree) {
    std::string line;
    if (!std::getline(input, line)) throw std::runtime_error("City CSV is empty");
    std::size_t row = 1;
    std::size_t inserted = 0;
    while (std::getline(input, line)) {
        ++row;
        if (line.empty()) continue;
        std::istringstream record(line);
        std::string id, city, latitude, longitude, country;
        if (!std::getline(record, id, ',') || !std::getline(record, city, ',') ||
            !std::getline(record, latitude, ',') || !std::getline(record, longitude, ',') ||
            !std::getline(record, country) || city.empty() || latitude.empty() ||
            longitude.empty())
            throw std::runtime_error("Malformed CSV record at line " + std::to_string(row));
        if (!country.empty() && country.back() == '\r') country.pop_back();
        if (tree.insert(city, latitude + " " + longitude + " " + country)) ++inserted;
    }
    if (inserted == 0) throw std::runtime_error("City CSV contains no usable cities");
}

double cityDistance(const std::string& first, const std::string& second,
                    const CityTree& cities) {
    std::string data1, data2;
    if (!cities.retrieve(first, data1)) throw std::runtime_error("City not found: " + first);
    if (!cities.retrieve(second, data2)) throw std::runtime_error("City not found: " + second);
    std::string lat1, lon1, lat2, lon2;
    std::istringstream a(data1), b(data2);
    if (!(a >> lat1 >> lon1) || !(b >> lat2 >> lon2))
        throw std::runtime_error("Missing coordinates in city record");
    return geo::distanceKm(geo::parseCoordinate(lat1, true),
                           geo::parseCoordinate(lon1, false),
                           geo::parseCoordinate(lat2, true),
                           geo::parseCoordinate(lon2, false));
}
} // namespace

int main(int argc, char* argv[]) {
    const std::string filename = argc > 1 ? argv[1] : "MCity.csv";
    try {
        std::ifstream input(filename);
        if (!input) throw std::runtime_error("Cannot open city file: " + filename);
        CityTree cities;
        loadCities(input, cities);
        std::string first, second;
        std::cout << "First city: " << std::flush;
        if (!std::getline(std::cin, first)) return 1;
        std::cout << "Second city: " << std::flush;
        if (!std::getline(std::cin, second)) return 1;
        std::cout << "Distance between " << first << " and " << second << ": "
                  << std::fixed << std::setprecision(2)
                  << cityDistance(first, second, cities) << " km\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "City error: " << error.what() << '\n';
        return 1;
    }
}
