#ifndef GRADES_H
#define GRADES_H

#include <vector>

enum class Method { Average, Median };

// Both throw std::domain_error when there are no values.
double average(const std::vector<int>& values);
double median(std::vector<int> values);

// Random result from 1 to 10, seeded with the current time.
int randomResult();

#endif
