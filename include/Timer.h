#ifndef TIMER_H
#define TIMER_H

#include <chrono>

// Measures elapsed time in seconds.
class Timer {
private:
    using Clock = std::chrono::high_resolution_clock;
    Clock::time_point start_;

public:
    Timer() : start_(Clock::now()) {}
    void reset() { start_ = Clock::now(); }
    double elapsed() const {
        return std::chrono::duration<double>(Clock::now() - start_).count();
    }
};

#endif
