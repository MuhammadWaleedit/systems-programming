#include "Grades.h"

#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>

double average(const std::vector<int>& values) {
    if (values.empty()) throw std::domain_error("Cannot calculate average of an empty list");
    double sum = 0.0;
    for (int v : values) sum += v;
    return sum / values.size();
}

double median(std::vector<int> values) {
    if (values.empty()) throw std::domain_error("Cannot calculate median of an empty list");
    std::sort(values.begin(), values.end());
    size_t mid = values.size() / 2;
    if (values.size() % 2 == 0) return (values[mid - 1] + values[mid]) / 2.0;
    return values[mid];
}

int randomResult() {
    static std::mt19937 engine(
        static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    static std::uniform_int_distribution<int> distribution(1, 10);
    return distribution(engine);
}
