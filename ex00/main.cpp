#include "BitcoinExchange.hpp"
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error: filename must be provided as an argument" << std::endl;
        return 1;
    }

    try {
        BitcoinExchange exchange;
        exchange.loadDatabase("../cpp_09/data.csv");
        exchange.processTXTfile(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}