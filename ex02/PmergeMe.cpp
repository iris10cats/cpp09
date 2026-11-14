#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <iomanip>
#include <time.h>

PmergeMe::PmergeMe() : VectorTimeNanoseconds(0.0), DequeTimeNanoseconds(0.0) {}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe& other)
        : Vector(other.Vector), Deque(other.Deque),
            VectorTimeNanoseconds(other.VectorTimeNanoseconds),
            DequeTimeNanoseconds(other.DequeTimeNanoseconds) {}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        Vector = other.Vector;
        Deque = other.Deque;
        VectorTimeNanoseconds = other.VectorTimeNanoseconds;
        DequeTimeNanoseconds = other.DequeTimeNanoseconds;
    }
    return *this;
}

PmergeMe::PmergeMe(std::vector<int> vec, std::deque<int> deq)
        : Vector(vec), Deque(deq),
            VectorTimeNanoseconds(0.0), DequeTimeNanoseconds(0.0) {}

void PmergeMe::printContainer(const std::string& label) const {
    std::cout << label;
    const std::size_t displayedCount = Vector.size() < 10 ? Vector.size() : 10;
    for (std::size_t i = 0; i < displayedCount; ++i) {
        std::cout << Vector[i] << " ";
    }
    if (Vector.size() > displayedCount)
        std::cout << "[.. << " << Vector.size() - displayedCount << " more numbers]" << std::endl;
    std::cout << std::endl;
}

// Vector implementation: all recursive work sequences in this section use std::vector.
std::vector<int> PmergeMe::fordJohnsonSort(const std::vector<int>& values) {
    // Recursion stops when zero or one value remains; that sequence is sorted.
    if (values.size() < 2)
        return values;

    // Pair adjacent values, keeping each pair as (smaller, larger).
    std::vector<std::pair<int, int> > pairs;
    pairs.reserve(values.size() / 2);
    for (std::size_t i = 0; i + 1 < values.size(); i += 2) {
        int first = values[i];
        int second = values[i + 1];
        if (first > second)
            std::swap(first, second);
        pairs.push_back(std::make_pair(first, second));
    }

    // Save an unpaired final value; it will be inserted after the pairs are ordered.
    const bool hasStraggler = values.size() % 2 != 0;
    const int leftover_value = hasStraggler ? values.back() : 0;

    // Recursively sort the larger partner from each pair.
    std::vector<int> largerValues;
    largerValues.reserve(pairs.size());
    for (std::size_t i = 0; i < pairs.size(); ++i)
        largerValues.push_back(pairs[i].second);
    const std::vector<int> sortedLarger = fordJohnsonSort(largerValues);

    // Record each distinct sorted maximum and the first output slot for its duplicates.
    std::vector<std::pair<int, std::size_t> > maximumGroups;
    std::vector<std::size_t> groupOffsets;
    for (std::size_t i = 0; i < sortedLarger.size(); ++i) {
        if (i == 0 || sortedLarger[i] != sortedLarger[i - 1]) {
            maximumGroups.push_back(std::make_pair(sortedLarger[i], i));
            groupOffsets.push_back(0);
        }
    }

    // Place each original pair into its maximum's sorted slots using binary search.
    std::vector<std::pair<int, int> > orderedPairs;
    orderedPairs.resize(pairs.size());
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        std::size_t low = 0;
        std::size_t high = maximumGroups.size();
        while (low < high) {
            const std::size_t middle = low + (high - low) / 2;
            if (maximumGroups[middle].first < pairs[i].second)
                low = middle + 1;
            else
                high = middle;
        }
        const std::size_t outputIndex = maximumGroups[low].second + groupOffsets[low];
        orderedPairs[outputIndex] = pairs[i];
        ++groupOffsets[low];
    }

    // Seed the main chain with the first small partner and all sorted maxima.
    std::vector<int> chain;
    chain.reserve(values.size());
    chain.push_back(orderedPairs[0].first);
    for (std::size_t i = 0; i < orderedPairs.size(); ++i)
        chain.push_back(orderedPairs[i].second);

    // Build the Ford-Johnson insertion order using Jacobsthal-number blocks.
    const std::size_t pendingCount = pairs.size() + (hasStraggler ? 1 : 0);
    std::vector<std::size_t> insertionOrder;
    insertionOrder.reserve(pendingCount);
    std::size_t previousJacobsthal = 1;
    std::size_t jacobsthal = 3;
    while (previousJacobsthal < pendingCount) {
        const std::size_t blockEnd =
            jacobsthal < pendingCount ? jacobsthal : pendingCount;
        for (std::size_t index = blockEnd; index > previousJacobsthal; --index)
            insertionOrder.push_back(index);
        const std::size_t nextJacobsthal = jacobsthal + 2 * previousJacobsthal;
        previousJacobsthal = jacobsthal;
        jacobsthal = nextJacobsthal;
    }

    // Insert pending small partners before their matching maximum using binary search.
    for (std::size_t i = 0; i < insertionOrder.size(); ++i) {
        const std::size_t pendingIndex = insertionOrder[i];
        if (pendingIndex > pairs.size()) {
            // The odd leftover has no partner, so its search covers the whole chain.
            std::vector<int>::iterator insertion =
                std::lower_bound(chain.begin(), chain.end(), leftover_value);
                chain.insert(insertion, leftover_value);
            continue;
        }

        const std::size_t pairIndex = pendingIndex - 1;
        // Find this pair's maximum in the sorted chain; its small partner belongs before it.
        std::vector<int>::iterator partner = std::lower_bound(
            chain.begin(), chain.end(), orderedPairs[pairIndex].second);

        std::vector<int>::iterator insertion = std::lower_bound(
            chain.begin(), partner,
            orderedPairs[pairIndex].first);
        chain.insert(insertion, orderedPairs[pairIndex].first);
    }

    return chain;
}

void PmergeMe::sortVector() {
    struct timespec start;
    struct timespec finish;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
        throw std::runtime_error("could not read monotonic clock");
    Vector = fordJohnsonSort(Vector);
    if (clock_gettime(CLOCK_MONOTONIC, &finish) != 0)
        throw std::runtime_error("could not read monotonic clock");
    const double elapsedNanoseconds =
        (finish.tv_sec - start.tv_sec) * 1000000000.0 +
        (finish.tv_nsec - start.tv_nsec);
    VectorTimeNanoseconds = elapsedNanoseconds;
}

void PmergeMe::printVectorTime() const {
    // Report the duration recorded by the most recent sortVector call.
    std::cout << "Time to process a range of " << Vector.size()
        << " elements with std::vector : "
        << std::fixed << std::setprecision(9)
        << VectorTimeNanoseconds / 1000000000.0 << " s"
        << std::endl;
}

// Deque implementation: all recursive work sequences in this section use std::deque.
std::deque<int> PmergeMe::fordJohnsonSort(const std::deque<int>& values) {
    // Recursion stops when zero or one value remains; that sequence is sorted.
    if (values.size() < 2)
        return values;

    // Pair adjacent values, keeping each pair as (smaller, larger).
    std::deque<std::pair<int, int> > pairs;
    for (std::size_t i = 0; i + 1 < values.size(); i += 2) {
        int first = values[i];
        int second = values[i + 1];
        if (first > second)
            std::swap(first, second);
        pairs.push_back(std::make_pair(first, second));
    }

    // Save an unpaired final value; it will be inserted after the pairs are ordered.
    const bool hasStraggler = values.size() % 2 != 0;
    const int leftover_value = hasStraggler ? values.back() : 0;

    // Recursively sort the larger partner from each pair.
    std::deque<int> largerValues;
    for (std::size_t i = 0; i < pairs.size(); ++i)
        largerValues.push_back(pairs[i].second);
    const std::deque<int> sortedLarger = fordJohnsonSort(largerValues);

    // Record each distinct sorted maximum and the first output slot for its duplicates.
    std::deque<std::pair<int, std::size_t> > maximumGroups;
    std::deque<std::size_t> groupOffsets;
    for (std::size_t i = 0; i < sortedLarger.size(); ++i) {
        if (i == 0 || sortedLarger[i] != sortedLarger[i - 1]) {
            maximumGroups.push_back(std::make_pair(sortedLarger[i], i));
            groupOffsets.push_back(0);
        }
    }

    // Place each original pair into its maximum's sorted slots using binary search.
    std::deque<std::pair<int, int> > orderedPairs;
    orderedPairs.resize(pairs.size());
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        std::size_t low = 0;
        std::size_t high = maximumGroups.size();
        while (low < high) {
            const std::size_t middle = low + (high - low) / 2;
            if (maximumGroups[middle].first < pairs[i].second)
                low = middle + 1;
            else
                high = middle;
        }
        const std::size_t outputIndex = maximumGroups[low].second + groupOffsets[low];
        orderedPairs[outputIndex] = pairs[i];
        ++groupOffsets[low];
    }

    // Seed the main chain with the first small partner and all sorted maxima.
    std::deque<int> chain;
    chain.push_back(orderedPairs[0].first);
    for (std::size_t i = 0; i < orderedPairs.size(); ++i)
        chain.push_back(orderedPairs[i].second);

    // Build the Ford-Johnson insertion order using Jacobsthal-number blocks.
    const std::size_t pendingCount = pairs.size() + (hasStraggler ? 1 : 0);
    std::deque<std::size_t> insertionOrder;
    std::size_t previousJacobsthal = 1;
    std::size_t jacobsthal = 3;
    while (previousJacobsthal < pendingCount) {
        const std::size_t blockEnd =
            jacobsthal < pendingCount ? jacobsthal : pendingCount;
        for (std::size_t index = blockEnd; index > previousJacobsthal; --index)
            insertionOrder.push_back(index);
        const std::size_t nextJacobsthal = jacobsthal + 2 * previousJacobsthal;
        previousJacobsthal = jacobsthal;
        jacobsthal = nextJacobsthal;
    }

    // Insert pending small partners before their matching maximum using binary search.
    for (std::size_t i = 0; i < insertionOrder.size(); ++i) {
        const std::size_t pendingIndex = insertionOrder[i];
        if (pendingIndex > pairs.size()) {
            // The odd leftover has no partner, so its search covers the whole chain.
            std::deque<int>::iterator insertion =
                std::lower_bound(chain.begin(), chain.end(), leftover_value);
            chain.insert(insertion, leftover_value);
            continue;
        }

        const std::size_t pairIndex = pendingIndex - 1;
        // Find this pair's maximum in the sorted chain; its small partner belongs before it.
        std::deque<int>::iterator partner = std::lower_bound(
            chain.begin(), chain.end(), orderedPairs[pairIndex].second);

        std::deque<int>::iterator insertion = std::lower_bound(
            chain.begin(), partner,
            orderedPairs[pairIndex].first);
        chain.insert(insertion, orderedPairs[pairIndex].first);
    }

    return chain;
}

void PmergeMe::sortDeque() {
    struct timespec start;
    struct timespec finish;
    if (clock_gettime(CLOCK_MONOTONIC, &start) != 0)
        throw std::runtime_error("could not read monotonic clock");
    Deque = fordJohnsonSort(Deque);
    if (clock_gettime(CLOCK_MONOTONIC, &finish) != 0)
        throw std::runtime_error("could not read monotonic clock");
    const double elapsedNanoseconds =
        (finish.tv_sec - start.tv_sec) * 1000000000.0 +
        (finish.tv_nsec - start.tv_nsec);
    DequeTimeNanoseconds = elapsedNanoseconds;
}

void PmergeMe::printDequeTime() const {
    // Report the duration recorded by the most recent sortDeque call.
    std::cout << "Time to process a range of " << Deque.size()
        << " elements with std::deque : "
        << std::fixed << std::setprecision(9)
        << DequeTimeNanoseconds / 1000000000.0 << " s"
        << std::endl;
}

