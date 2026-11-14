#include "BitcoinExchange.hpp"
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

static std::string trim(const std::string& text) {
        const std::string::size_type first = text.find_first_not_of(" \t\r\n");
        if (first == std::string::npos)
            return "";
        const std::string::size_type last = text.find_last_not_of(" \t\r\n");
        return text.substr(first, last - first + 1);
}
        

static bool isValidDate(const std::string& date) {
        if (date.length() != 10 || date[4] != '-' || date[7] != '-')
            return false;
        for (std::string::size_type i = 0; i < date.length(); ++i) {
            if (i == 4 || i == 7)
                continue;
            if (date[i] < '0' || date[i] > '9')
                return false;
        }

        const int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
            (date[2] - '0') * 10 + (date[3] - '0');
        const int month = (date[5] - '0') * 10 + (date[6] - '0');
        const int day = (date[8] - '0') * 10 + (date[9] - '0');
        if (year == 0 || month < 1 || month > 12 || day < 1)
            return false;

        const bool leapYear = year % 400 == 0 ||
            (year % 4 == 0 && year % 100 != 0);
        const int daysInMonth[] = {
            31, leapYear ? 29 : 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };
        return day <= daysInMonth[month - 1];
}

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : map(other.map) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if(this != &other){
        map = other.map;
    }
    return *this;
}
BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("could not open database file");
    }
    std::string line;
    std::getline(file, line);
    

    std::string date;
    std::string rate;
    float RATE;

    while (std::getline(file, line)) {
        date = line.substr(0, 10);
        rate = line.substr(11);
        std::istringstream rateStream(rate);
        rateStream >> RATE;
        map[date] = RATE;
    }

}

void BitcoinExchange::processTXTfile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    if (!input.is_open()) {
        throw std::runtime_error("could not open input file");
    }

    std::string line;
    if (!std::getline(input, line) || trim(line) != "date | value") {
        throw std::runtime_error("invalid input header");
    }

    while (std::getline(input, line)) {
        const std::string::size_type separator = line.find('|');
        if (separator == std::string::npos || line.find('|', separator + 1) != std::string::npos)
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        const std::string date = trim(line.substr(0, separator));
        const std::string valueText = trim(line.substr(separator + 1));
        if (!isValidDate(date) || valueText.empty()) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        std::istringstream valueStream(valueText);
        double value;
        char extra;
        if (!(valueStream >> value) || valueStream >> extra) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }
        
        if (value < 0) {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        
        if (value > 1000) {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }
        

        std::map<std::string, float>::const_iterator it = map.lower_bound(date);
        if (it == map.end() || it->first != date) {
            if (it == map.begin()) {
                std::cerr << "Error: no exchange rate on or before "
                    << date << "." << std::endl;
                continue;
            }

            --it;
        }

        std::cout<<date<<" => "<< value <<" = "<< value * it->second << std::endl;
    }
}