#pragma once
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>
#include "math.h"
#include <array>
#include <functional>
#include <algorithm>
#include "Location.h"

struct SearchData {
    std::multimap<double, std::string> cities;
    size_t foundSize;
    size_t northCitiesSize;
};


class CitiesHandler {
public:
    
    CitiesHandler();
    const SearchData findCities(const std::string&, double, int);
    bool contains(const std::string& cityName);

private:;

    const std::string fileName = "data.txt";

    std::multimap<CityLocation, std::string, LessX> m_dataKeyX;
    std::multimap<CityLocation, std::string, LessY> m_dataKeyY;
    std::multimap<std::string, CityLocation>   m_dataKeyName;


    std::array<std::function<double(const CityLocation&, const CityLocation&)>, 3> m_norms = {
	    [](const CityLocation& l, const CityLocation& r) {return std::sqrt(std::pow(l.x - r.x, 2) + std::pow(l.y - r.y, 2));},
		[](const CityLocation& l, const CityLocation& r) {return std::max(std::abs(l.x - r.x), std::abs(l.y - r.y));},
		[](const CityLocation& l, const CityLocation& r) {return std::abs(l.x - r.x) + std::abs(r.y - l.y);}
	};

    void readFile();
    void checkFile(std::ifstream& );
    void insert(const std::string&, double, double);
    size_t getNorthCitiesAmount(std::multimap<double, std::string>&, double);
    std::multimap<double, std::string> searchCities(CityLocation&, double, int);
    std::multimap<CityLocation, std::string, LessX> getSquareRectCitiesMap(CityLocation&, double);
    std::multimap<double, std::string> getDisatanceMap(const  std::multimap<CityLocation, std::string, LessX>&, CityLocation&, int);
};
