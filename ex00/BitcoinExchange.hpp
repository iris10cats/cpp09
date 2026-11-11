#pragma once

class BitcoinExchange {
    private:
        std::map<std::string, float> map;
    pubic:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        Span& operator=(const BitcoinExchange& other);
        ~BitcoinExchange();
        void loadData(const std::string& file);
        void processData(const std::string& file);
};
