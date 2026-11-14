#include "RPN.hpp"
#include <cctype>
#include <stdexcept>

ReversePolishNotation::ReversePolishNotation() {}

ReversePolishNotation::ReversePolishNotation(const ReversePolishNotation& other) : std::list<double>(other) {}

ReversePolishNotation& ReversePolishNotation::operator=(const ReversePolishNotation& other) {
    if (this != &other) {
        std::list<double>::operator=(other);
    }
    return *this;
}

ReversePolishNotation::~ReversePolishNotation() {}

double ReversePolishNotation::evaluate(const std::string& expression) {
    clear();
    bool Space = true;
    for (std::string::const_iterator iter = expression.begin(); iter != expression.end(); ++iter) {
        if (std::isspace(static_cast<unsigned char>(*iter))) {
            Space = true;
            continue;
        }
        if (std::isdigit(static_cast<unsigned char>(*iter))) {
            if (!Space)
                throw std::invalid_argument("Missing space between tokens");
            push_back(*iter - '0');
            Space = false;
            continue;
        }
        if (*iter != '+' && *iter != '-' && *iter != '*' && *iter != '/')
            throw std::invalid_argument("Invalid character in expression");

        if (size() < 2)
            throw std::invalid_argument("Operator does not have two operands");

        double right = back();
        pop_back();
        double left = back();
        pop_back();
        
        if (*iter == '+') {
            if (!Space)
                throw std::invalid_argument("Missing space between tokens");
            push_back(left + right);
        } else if (*iter == '-') {
            if (!Space)
                throw std::invalid_argument("Missing space between tokens");
            push_back(left - right);
        } else if (*iter == '*') {
            if (!Space)
                throw std::invalid_argument("Missing space between tokens");
            push_back(left * right);
        } else {
            if (!Space)
                throw std::invalid_argument("Missing space between tokens");
            if (right == 0)
                throw std::runtime_error("Division by zero");
            push_back(left / right);
        }
    }

    if (size() != 1)
        throw std::invalid_argument("Expression does not produce one result");
    return back();
}