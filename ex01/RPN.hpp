#include <list>
#include <string>

class ReversePolishNotation : public std::list<double> {
public:
    
    
    ReversePolishNotation();
    ~ReversePolishNotation();
    ReversePolishNotation(const ReversePolishNotation& other);
    ReversePolishNotation& operator=(const ReversePolishNotation& other);

    
    double evaluate(const std::string& expression);
};