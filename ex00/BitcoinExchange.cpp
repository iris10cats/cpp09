#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : map(other.map) {}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if(this != &other){
        map = other.map;
    }
    return *this;
}
BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::loadData(const std::string& file) {
    std::ifstream File(file);
    if (!file.is_open()) {
        std::cerr << "can not open file" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
}