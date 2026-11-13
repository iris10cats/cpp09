#include "BitcoinExchange.hpp"
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : map(other.map) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if(this != &other){
        map = other.map;
    }
    return *this;
}
BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadData(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("filne didnt open"):
    }
    std::string first_line;
    if (!std::getline(file, first_line) || first_line != "date,exchange_rate") {
        throw std::runtime_error("not a header");
    } 
    std::string line;
    std::string date;
    std::string rate;
    std::string::size_type comma = line.find(',');

    float RATE;
    while (std::getline(file, line)){
        date = line.substr(0, comma);
        rate = line.substr(comma + 1);
        std::istringstream rateStream(rate);
        rateStream >> RATE;
        map[date] = RATE;
    }

}