#include <iostream>
#include <functional>
#include <limits>
#include <stdexcept>
#include "CitiesHandler.h"

class MapCalculator {
public:
    MapCalculator() : m_citiesHandler() {}
    void run();
    
private:
    CitiesHandler m_citiesHandler;

    void print(const SearchData& data);
    std::string getCity();
    double getRadius();
    int getNorm();

    /**
     * Validates user input based on a condition.
     * @param prompt The prompt message to display to the user.
     * @param errorMessage The error message to display for invalid input.
     * @param condition The condition function for input validation.
     * @return The validated input value.
     * @throws std::invalid_argument if the input is invalid.
     */
    template <typename T>
    T validateInput(const std::string& prompt, const std::string& errorMessage, const std::function<bool(T)>& condition) {
        T value;

        while (true) {
            std::cout << prompt << "\n";
            std::cin >> value;

            if (std::cin.fail()) {
                throw std::invalid_argument(errorMessage);
            }

            if (!condition(value)) {
                std::cout << "Invalid input: " << value << ", " << errorMessage << ", try again!\n";
            }
            else {
                break;
            }

            // Clear input buffer
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return value;
    }
};
