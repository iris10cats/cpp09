#include <vector>
#include <deque>
#include <string>

class PmergeMe {
public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);
    PmergeMe(std::vector<int> vec, std::deque<int> deq);
    void printContainer(const std::string& label) const;
    void sortVector();
    void sortDeque();
    void printVectorTime() const;
    void printDequeTime() const;
private:
    static std::vector<int> fordJohnsonSort(const std::vector<int>& values);
    static std::deque<int> fordJohnsonSort(const std::deque<int>& values);
    std::vector<int> Vector;
    std::deque<int> Deque;
    double VectorTimeNanoseconds;
    double DequeTimeNanoseconds;
};