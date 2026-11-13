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
        void loadData(const std::string& file);
        void processData(const std::string& file);
};
