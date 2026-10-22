#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>

#include "Grades.h"

class Person {
private:
    std::string firstName_;
    std::string lastName_;
    std::vector<int> homework_;
    int exam_;
    double finalAverage_;
    double finalMedian_;

public:
    Person();
    // Throws std::invalid_argument if there are no homework results,
    // std::out_of_range if a result is not in 1-10.
    Person(std::string firstName, std::string lastName, std::vector<int> homework, int exam);

    // Rule of three
    Person(const Person& other);
    Person& operator=(const Person& other);
    ~Person();

    // Move operations, so containers and std::sort move students
    // instead of deep-copying them.
    Person(Person&& other) noexcept;
    Person& operator=(Person&& other) noexcept;

    // Final points = 0.4 * (average | median of homework) + 0.6 * exam
    void calculateFinal();

    const std::string& firstName() const { return firstName_; }
    const std::string& lastName() const { return lastName_; }
    double finalPoints(Method method) const {
        return method == Method::Average ? finalAverage_ : finalMedian_;
    }

    friend std::istream& operator>>(std::istream& in, Person& person);
    friend std::ostream& operator<<(std::ostream& out, const Person& person);
};

#endif
