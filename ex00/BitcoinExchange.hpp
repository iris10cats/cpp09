#pragma once
#include <map>
#include <string>

class BitcoinExchange {
    private:
        std::map<std::string, float> map;
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();
        void loadDatabase(const std::string& filename);
        void processTXTfile(const std::string& file);
};
