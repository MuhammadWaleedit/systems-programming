#include <iomanip>
#include <iostream>
#include <string>

class Person {
private:
    std::string firstName_;
    std::string lastName_;
    int* homework_;      // dynamic array of homework results
    int homeworkCount_;
    int exam_;
    double finalPoints_;

public:
    Person() : homework_(nullptr), homeworkCount_(0), exam_(0), finalPoints_(0.0) {}

    Person(const std::string& firstName, const std::string& lastName,
           const int* homework, int homeworkCount, int exam)
        : firstName_(firstName), lastName_(lastName),
          homework_(nullptr), homeworkCount_(homeworkCount),
          exam_(exam), finalPoints_(0.0) {
        if (homeworkCount_ > 0) {
            homework_ = new int[homeworkCount_];
            for (int i = 0; i < homeworkCount_; ++i) homework_[i] = homework[i];
        }
        calculateFinal();
    }

    // Rule of three: copy constructor
    Person(const Person& other)
        : firstName_(other.firstName_), lastName_(other.lastName_),
          homework_(nullptr), homeworkCount_(other.homeworkCount_),
          exam_(other.exam_), finalPoints_(other.finalPoints_) {
        if (homeworkCount_ > 0) {
            homework_ = new int[homeworkCount_];
            for (int i = 0; i < homeworkCount_; ++i) homework_[i] = other.homework_[i];
        }
    }

    // Rule of three: copy assignment operator
    Person& operator=(const Person& other) {
        if (this == &other) return *this;

        int* copy = nullptr;
        if (other.homeworkCount_ > 0) {
            copy = new int[other.homeworkCount_];
            for (int i = 0; i < other.homeworkCount_; ++i) copy[i] = other.homework_[i];
        }
        delete[] homework_;

        homework_ = copy;
        homeworkCount_ = other.homeworkCount_;
        firstName_ = other.firstName_;
        lastName_ = other.lastName_;
        exam_ = other.exam_;
        finalPoints_ = other.finalPoints_;
        return *this;
    }

    // Rule of three: destructor
    ~Person() { delete[] homework_; }

    // Final points = 0.4 * average of homework + 0.6 * exam
    void calculateFinal() {
        double sum = 0.0;
        for (int i = 0; i < homeworkCount_; ++i) sum += homework_[i];
        double average = homeworkCount_ > 0 ? sum / homeworkCount_ : 0.0;
        finalPoints_ = 0.4 * average + 0.6 * exam_;
    }

    friend std::istream& operator>>(std::istream& in, Person& person);
    friend std::ostream& operator<<(std::ostream& out, const Person& person);
};

std::istream& operator>>(std::istream& in, Person& person) {
    Person read;

    std::cout << "First name: ";
    in >> read.firstName_;
    std::cout << "Surname: ";
    in >> read.lastName_;

    std::cout << "Number of homework results: ";
    in >> read.homeworkCount_;
    if (read.homeworkCount_ < 0) read.homeworkCount_ = 0;

    if (read.homeworkCount_ > 0) {
        read.homework_ = new int[read.homeworkCount_];
        for (int i = 0; i < read.homeworkCount_; ++i) {
            std::cout << "  Homework " << i + 1 << ": ";
            in >> read.homework_[i];
        }
    }

    std::cout << "Exam result: ";
    in >> read.exam_;

    read.calculateFinal();
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

int main() {
    int studentCount = 0;
    std::cout << "How many students? ";
    std::cin >> studentCount;
    if (studentCount <= 0) return 0;

    Person* students = new Person[studentCount];
    for (int i = 0; i < studentCount; ++i) {
        std::cout << "\nStudent " << i + 1 << "\n";
        std::cin >> students[i];
    }

    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(18) << "Final_Point(Aver.)" << "\n";
    std::cout << std::string(48, '-') << "\n";
    for (int i = 0; i < studentCount; ++i) std::cout << students[i] << "\n";

    delete[] students;
    return 0;
}
