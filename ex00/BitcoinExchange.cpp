#include "BitcoinExchange.hpp"
#include <exception>
#include <iostream>
#include <fstream>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : map(other.map) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if(this != &other){
        map = other.map;
    }
    return *this;
}
BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::loadData(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("filne didnt open"):
    }
    std::string line;
    /*if (!std::getline(file, first_line) || first_line != "date,exchange_rate") {
        throw std::runtime_error("invalid header");
    } */

    std::string date;
    std::string rate;
    float RATE;
    while (std::getline(file, line)){
        date = line.substr(0, ',');
        rate = line.substr(',' + 1);
        RATE = strtof(rate, NULL);
        map[date] = RATE;
    }

}