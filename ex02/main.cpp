#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <deque>
#include "PmergeMe.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <positive integers sequence>" << std::endl;
        return 1;
    }

    std::vector<int> vectorValues;
    std::deque<int> dequeValues;
    for (int i = 1; i < argc; ++i) {
        std::istringstream parser(argv[i]);
        int value;
        char extra;
        if (!(parser >> value) || (parser >> extra) || value <= 0) {
            std::cerr << "Error: invalid positive integer: " << argv[i] << std::endl;
            return 1;
        }
        vectorValues.push_back(value);
        dequeValues.push_back(value);
    }

    PmergeMe sorter(vectorValues, dequeValues);
    sorter.printContainer("Before: ");
    sorter.sortDeque();
    sorter.sortVector();
    //sorter.sortDeque();
    sorter.printContainer("After:  ");
    sorter.printVectorTime();
    sorter.printDequeTime();
    return 0;
}