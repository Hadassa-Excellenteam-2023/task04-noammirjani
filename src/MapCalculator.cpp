#include "MapCalculator.h"


/**
 * Run the map calculator program.
 */
void MapCalculator::run() {
    std::string cityName = getCity();
    while (cityName != "0") {
        double radius = getRadius();
        int norm = getNorm();

        std::cout << "Searches for " << cityName << " " << radius << " " << norm << "\n";
        print(m_citiesHandler.findCities(cityName, radius, norm));

        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        cityName = getCity();
    }
}

/**
 * Prompt the user for the radius and validate the input.
 * @return The validated radius value.
 */
double MapCalculator::getRadius() {
    return validateInput<double>(
        "Please enter radius (with line break after it)",
        "Radius must be a positive double!",
        [](double radius) { return radius >= 0; }
    );
}


/**
 * Prompt the user for the norm and validate the input.
 * @return The validated norm value.
 */
int MapCalculator::getNorm() {
    return validateInput<int>(
        "Please enter norm (with line break after it)",
        "Norm must be an integer between 0 and 2!",
        [](int norm) { return norm >= 0 && norm <= 2; }
    );
}

/**
 * Prompt the user for the city name and validate the input.
 * @return The validated city name.
 */
std::string MapCalculator::getCity() {
    std::string line;

    while (true) {
        try {
            std::cout << "Please enter selected city name (with line break after it)\n";
            std::getline(std::cin, line);

            if (line == "0") return line;

            if (!m_citiesHandler.contains(line)) {
                throw std::invalid_argument("File data does not exist: " + line);
            }
            return line;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << ", try again!\n";
        }
    }
}


/**
 * Print the search results.
 * @param data The search data to be printed.
 */
void MapCalculator::print(const SearchData& data) {
    std::cout << "Found cities: " << data.foundSize << "\n";
    std::cout << "North cities: " << data.northCitiesSize << "\n";
    std::cout << "City list: \n";

    std::ostream_iterator<std::string> outIterator(std::cout, "\n");
    std::transform(data.cities.begin(), data.cities.end(), outIterator, [](const auto& pair) {
        return pair.second;
        });
}