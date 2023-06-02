
#include "CitiesHandler.h"

/**
 * Constructor for the CitiesHandler class.
 */
CitiesHandler::CitiesHandler() {
    readFile();
}


/**
 * Checks if the cities file is open and valid.
 * Throws exceptions if the file cannot be opened or if it is empty.
 *
 * @param citiesFile The input file stream for the cities file.
 */
void CitiesHandler::checkFile(std::ifstream& citiesFile) {
    if (!citiesFile.is_open()) {
        throw std::invalid_argument("Error opening file: " + fileName);
    }

    auto len = citiesFile.seekg(0, std::ios::end).tellg();
    if (len == 0) {
        throw std::length_error("Input file is empty: " + fileName);
    }

    citiesFile.seekg(0, std::ios::beg);
}

/**
 * Reads the cities file and inserts the city data into the data structures.
 * Throws an exception if the file data is unexpected.
 */
void CitiesHandler::readFile() {
    std::ifstream citiesFile;
    citiesFile.open(fileName);
    checkFile(citiesFile);

    std::string name, coords;
    while (std::getline(citiesFile, name) &&
           std::getline(citiesFile, coords)) {

        std::istringstream input(coords);
        std::string separator;
        double x, y;

        if (!(input >> x >> separator >> y)) {
            std::string errData = name + " " + std::to_string(x) + " " + std::to_string(y);
            throw std::invalid_argument("File data is unexpected: " + errData);
        }

        insert(name, x, y);
    }
}

/**
 * Inserts a city into the data structures.
 *
 * @param name The name of the city.
 * @param x The x-coordinate of the city.
 * @param y The y-coordinate of the city.
 */
void CitiesHandler::insert(const std::string& name, double x, double y) {
    CityLocation city{x, y};
    m_dataKeyX.insert(std::make_pair(city, name));
    m_dataKeyY.insert(std::make_pair(city, name));
    m_dataKeyName.insert(std::make_pair(name, city));
}

/**
 * Checks if a city with the given name exists in the data structures.
 *
 * @param cityName The name of the city to check.
 * @return True if the city exists, false otherwise.
 */
bool CitiesHandler::contains(const std::string& cityName) {
    return m_dataKeyName.find(cityName) != m_dataKeyName.end();
}

/**
 * Finds cities within a given radius of a specified city.
 * Calculates the distance map and the number of cities located to the north of the specified city.
 *
 * @param cityName The name of the city.
 * @param radius The radius within which to search for cities.
 * @param norm The norm to use for distance calculation.
 * @return A SearchData object containing the distance map and other relevant information.
 */
const SearchData CitiesHandler::findCities(const std::string &cityName, double radius, int norm) {

    if (!contains(cityName)) {
        // self check for invalid argument
        throw std::invalid_argument("City not found: " + cityName);
    }

    CityLocation currCity = m_dataKeyName.find(cityName)->second;
    auto distanceMap = searchCities(currCity, radius, norm);

    return { distanceMap, distanceMap.size(), getNorthCitiesAmount(distanceMap, currCity.x) };
}

/**
 * Calculates the number of cities located to the north of a specified city in the distance map.
 *
 * @param distanceMap The distance map containing the cities and their distances.
 * @param currY The y-coordinate of the specified city.
 * @return The number of cities located to the north of the specified city.
 */
size_t CitiesHandler::getNorthCitiesAmount(std::multimap<double, std::string>& distanceMap, double currX) {

	return std::count_if(distanceMap.begin(), distanceMap.end(),
        [currY, this](const std::pair<double, std::string>& city) {
        return m_dataKeyName.find(city.second)->second.x > currX;
        	});

}

/**
 * Searches for cities within a specified radius of a given city.
 *
 * @param currCity The city for which to search for nearby cities.
 * @param radius The radius within which to search for cities.
 * @param norm The norm to use for distance calculation.
 * @return A distance map containing the nearby cities and their distances.
 */
std::multimap<double, std::string> CitiesHandler::searchCities(CityLocation& currCity, double radius, int norm) {
    // rect by square
    auto squareRectCitiesMap = getSquareRectCitiesMap(currCity, radius);
    // rect by radius
    auto distanceMap = getDisatanceMap(squareRectCitiesMap, currCity, norm);

    return std::multimap<double, std::string>(++distanceMap.begin(), distanceMap.upper_bound(radius));
}

/**
 * Retrieves the cities within the square rectangle defined by the given city and radius.
 *
 * @param city The city for which to retrieve nearby cities.
 * @param radius The radius within which to retrieve cities.
 * @return A multimap containing the cities within the square rectangle, sorted by x-coordinate.
 */
std::multimap<CityLocation, std::string, LessX> CitiesHandler::getSquareRectCitiesMap(CityLocation& city, double radius) {

    auto itBeginX = m_dataKeyX.lower_bound({city.x - radius, 0 });
    auto itEndX = m_dataKeyX.upper_bound({ city.x + radius, 0 });
    auto itBeginY = m_dataKeyY.lower_bound({ 0, city.y - radius });
    auto itEndY = m_dataKeyY.upper_bound({ 0, city.y + radius });

    std::multimap<CityLocation, std::string, LessX> subMapYbyX(itBeginY, itEndY);
    std::multimap<CityLocation, std::string, LessX> square;

    std::set_intersection(itBeginX, itEndX, subMapYbyX.begin(), subMapYbyX.end(),
        std::inserter(square, square.begin()), CompareCityByX());

    return square;
}


/**
 * Calculates the distance map between the specified city and the cities in the given square.
 *
 * @param square The square containing the nearby cities.
 * @param city The specified city for distance calculation.
 * @param norm The norm to use for distance calculation.
 * @return A distance map containing the distances between the specified city and the nearby cities.
 */
std::multimap<double, std::string> CitiesHandler::getDisatanceMap(const std::multimap<CityLocation,
    std::string, LessX>& square,  CityLocation& city, int norm) {

    std::multimap<double, std::string> distanceMap;

    std::transform(square.begin(), square.end(), std::inserter(distanceMap, distanceMap.begin()),
        [city, norm, this](const std::pair<CityLocation, std::string>& other) {
		return std::make_pair(m_norms[norm](city, other.first), other.second);
	});

    return distanceMap;
}