#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

enum class Method { Average, Median };

double average(const std::vector<int>& values) {
    if (values.empty()) return 0.0;
    double sum = 0.0;
    for (int v : values) sum += v;
    return sum / values.size();
}

double median(std::vector<int> values) {
    if (values.empty()) return 0.0;
    std::sort(values.begin(), values.end());
    size_t mid = values.size() / 2;
    if (values.size() % 2 == 0) return (values[mid - 1] + values[mid]) / 2.0;
    return values[mid];
}

class Person {
private:
    std::string firstName_;
    std::string lastName_;
    std::vector<int> homework_;
    int exam_;
    double finalPoints_;

public:
    Person() : exam_(0), finalPoints_(0.0) {}

    Person(const std::string& firstName, const std::string& lastName,
           const std::vector<int>& homework, int exam)
        : firstName_(firstName), lastName_(lastName),
          homework_(homework), exam_(exam), finalPoints_(0.0) {}

    // Rule of three: copy constructor
    Person(const Person& other)
        : firstName_(other.firstName_), lastName_(other.lastName_),
          homework_(other.homework_), exam_(other.exam_),
          finalPoints_(other.finalPoints_) {}

    // Rule of three: copy assignment operator
    Person& operator=(const Person& other) {
        if (this == &other) return *this;
        firstName_ = other.firstName_;
        lastName_ = other.lastName_;
        homework_ = other.homework_;
        exam_ = other.exam_;
        finalPoints_ = other.finalPoints_;
        return *this;
    }

    // Rule of three: destructor
    ~Person() { homework_.clear(); }

    // Final points = 0.4 * (average | median of homework) + 0.6 * exam
    void calculateFinal(Method method) {
        double hw = method == Method::Average ? average(homework_) : median(homework_);
        finalPoints_ = 0.4 * hw + 0.6 * exam_;
    }

    friend std::istream& operator>>(std::istream& in, Person& person);
    friend std::ostream& operator<<(std::ostream& out, const Person& person);
};

// Reads an integer in [min, max]; asks again on invalid input.
int readNumber(std::istream& in, int min, int max) {
    int value;
    while (!(in >> value) || value < min || value > max) {
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  Enter a number from " << min << " to " << max << ": ";
    }
    return value;
}

std::istream& operator>>(std::istream& in, Person& person) {
    Person read;

    std::cout << "First name: ";
    in >> read.firstName_;
    std::cout << "Surname: ";
    in >> read.lastName_;

    // The number of homework results is not known in advance:
    // the user enters 0 when all results are entered.
    std::cout << "Enter homework results (1-10), 0 to finish:\n";
    while (true) {
        std::cout << "  Homework " << read.homework_.size() + 1 << ": ";
        int result = readNumber(in, 0, 10);
        if (result == 0) break;
        read.homework_.push_back(result);
    }

    std::cout << "Exam result (1-10): ";
    read.exam_ = readNumber(in, 1, 10);

    person = read;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Person& person) {
    out << std::left << std::setw(15) << person.firstName_
        << std::setw(15) << person.lastName_
        << std::right << std::setw(18) << std::fixed << std::setprecision(2)
        << person.finalPoints_;
    return out;
}

bool askYesNo(const std::string& question) {
    std::string answer;
    while (true) {
        std::cout << question << " (y/n): ";
        std::cin >> answer;
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
    }
}

Method askMethod() {
    std::string answer;
    while (true) {
        std::cout << "Calculate final points by (a)verage or (m)edian? ";
        std::cin >> answer;
        if (answer == "a" || answer == "A") return Method::Average;
        if (answer == "m" || answer == "M") return Method::Median;
    }
}

int main() {
    std::vector<Person> students;

    do {
        std::cout << "\nStudent " << students.size() + 1 << "\n";
        Person person;
        std::cin >> person;
        students.push_back(person);
    } while (askYesNo("Add another student?"));

    Method method = askMethod();
    for (Person& person : students) person.calculateFinal(method);

    std::string header = method == Method::Average ? "Final_Point(Aver.)" : "Final_Point(Med.)";
    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(18) << header << "\n";
    std::cout << std::string(48, '-') << "\n";
    for (const Person& person : students) std::cout << person << "\n";

    return 0;
}
