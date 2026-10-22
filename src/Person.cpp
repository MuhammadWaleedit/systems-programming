#include "Person.h"

#include <iomanip>
#include <stdexcept>
#include <utility>

#include "Input.h"

namespace {
void checkResult(int result) {
    if (result < 1 || result > 10)
        throw std::out_of_range("Result " + std::to_string(result) + " is not in range 1-10");
}
}  // namespace

Person::Person() : exam_(0), finalAverage_(0.0), finalMedian_(0.0) {}

Person::Person(std::string firstName, std::string lastName, std::vector<int> homework, int exam)
    : firstName_(std::move(firstName)), lastName_(std::move(lastName)),
      homework_(std::move(homework)), exam_(exam), finalAverage_(0.0), finalMedian_(0.0) {
    if (homework_.empty())
        throw std::invalid_argument("Student " + firstName_ + " " + lastName_ + " has no homework results");
    for (int result : homework_) checkResult(result);
    checkResult(exam_);
    calculateFinal();
}

Person::Person(const Person& other)
    : firstName_(other.firstName_), lastName_(other.lastName_),
      homework_(other.homework_), exam_(other.exam_),
      finalAverage_(other.finalAverage_), finalMedian_(other.finalMedian_) {}

Person& Person::operator=(const Person& other) {
    if (this == &other) return *this;
    firstName_ = other.firstName_;
    lastName_ = other.lastName_;
    homework_ = other.homework_;
    exam_ = other.exam_;
    finalAverage_ = other.finalAverage_;
    finalMedian_ = other.finalMedian_;
    return *this;
}

Person::~Person() { homework_.clear(); }

Person::Person(Person&& other) noexcept
    : firstName_(std::move(other.firstName_)), lastName_(std::move(other.lastName_)),
      homework_(std::move(other.homework_)), exam_(other.exam_),
      finalAverage_(other.finalAverage_), finalMedian_(other.finalMedian_) {}

Person& Person::operator=(Person&& other) noexcept {
    if (this == &other) return *this;
    firstName_ = std::move(other.firstName_);
    lastName_ = std::move(other.lastName_);
    homework_ = std::move(other.homework_);
    exam_ = other.exam_;
    finalAverage_ = other.finalAverage_;
    finalMedian_ = other.finalMedian_;
    return *this;
}

void Person::calculateFinal() {
    finalAverage_ = 0.4 * average(homework_) + 0.6 * exam_;
    finalMedian_ = 0.4 * median(homework_) + 0.6 * exam_;
}

std::istream& operator>>(std::istream& in, Person& person) {
    std::string firstName, lastName;
    std::cout << "First name: ";
    in >> firstName;
    std::cout << "Surname: ";
    in >> lastName;

    // The number of homework results is not known in advance:
    // the user enters 0 when all results are entered.
    std::vector<int> homework;
    std::cout << "Enter homework results (1-10), 0 to finish:\n";
    while (true) {
        std::cout << "  Homework " << homework.size() + 1 << ": ";
        int result = readNumber(in, 0, 10);
        if (result == 0) {
            if (homework.empty()) {
                std::cout << "  At least one homework result is required.\n";
                continue;
            }
            break;
        }
        homework.push_back(result);
    }

    std::cout << "Exam result (1-10): ";
    int exam = readNumber(in, 1, 10);

    person = Person(firstName, lastName, homework, exam);
    return in;
}

// Prints name, surname, final points by average and by median.
std::ostream& operator<<(std::ostream& out, const Person& person) {
    out << std::left << std::setw(15) << person.firstName_
        << std::setw(15) << person.lastName_
        << std::right << std::fixed << std::setprecision(2)
        << std::setw(14) << person.finalAverage_ << " |"
        << std::setw(13) << person.finalMedian_;
    return out;
}
