#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

enum class Method { Average, Median };

// Random results from 1 to 10, seeded with the current time.
int randomResult() {
    static std::mt19937 engine(
        static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    static std::uniform_int_distribution<int> distribution(1, 10);
    return distribution(engine);
}

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
    double finalAverage_;
    double finalMedian_;

public:
    Person() : exam_(0), finalAverage_(0.0), finalMedian_(0.0) {}

    Person(const std::string& firstName, const std::string& lastName,
           const std::vector<int>& homework, int exam)
        : firstName_(firstName), lastName_(lastName),
          homework_(homework), exam_(exam), finalAverage_(0.0), finalMedian_(0.0) {
        calculateFinal();
    }

    // Rule of three: copy constructor
    Person(const Person& other)
        : firstName_(other.firstName_), lastName_(other.lastName_),
          homework_(other.homework_), exam_(other.exam_),
          finalAverage_(other.finalAverage_), finalMedian_(other.finalMedian_) {}

    // Rule of three: copy assignment operator
    Person& operator=(const Person& other) {
        if (this == &other) return *this;
        firstName_ = other.firstName_;
        lastName_ = other.lastName_;
        homework_ = other.homework_;
        exam_ = other.exam_;
        finalAverage_ = other.finalAverage_;
        finalMedian_ = other.finalMedian_;
        return *this;
    }

    // Rule of three: destructor
    ~Person() { homework_.clear(); }

    // Final points = 0.4 * (average | median of homework) + 0.6 * exam
    void calculateFinal() {
        finalAverage_ = 0.4 * average(homework_) + 0.6 * exam_;
        finalMedian_ = 0.4 * median(homework_) + 0.6 * exam_;
    }

    const std::string& firstName() const { return firstName_; }
    const std::string& lastName() const { return lastName_; }
    double finalPoints(Method method) const {
        return method == Method::Average ? finalAverage_ : finalMedian_;
    }

    // Reads one record from a file line: Name Surname HW1 ... HWn Exam
    bool readFromLine(const std::string& line) {
        std::istringstream in(line);
        std::string firstName, lastName;
        if (!(in >> firstName >> lastName)) return false;

        std::vector<int> results;
        int value;
        while (in >> value) results.push_back(value);
        if (results.empty()) return false;

        exam_ = results.back();
        results.pop_back();
        firstName_ = firstName;
        lastName_ = lastName;
        homework_ = results;
        calculateFinal();
        return true;
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

    read.calculateFinal();
    person = read;
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

// Asks only for the name; homework and exam results are generated randomly.
Person generatePerson() {
    std::string firstName, lastName;
    std::cout << "First name: ";
    std::cin >> firstName;
    std::cout << "Surname: ";
    std::cin >> lastName;
    std::cout << "How many homework results to generate (1-20)? ";
    int count = readNumber(std::cin, 1, 20);

    std::vector<int> homework;
    for (int i = 0; i < count; ++i) homework.push_back(randomResult());
    int exam = randomResult();

    std::cout << "  Generated homework:";
    for (int result : homework) std::cout << " " << result;
    std::cout << ", exam: " << exam << "\n";
    return Person(firstName, lastName, homework, exam);
}

// Reads all students from a file. The first line is a header.
std::vector<Person> readFromFile(const std::string& fileName) {
    std::vector<Person> students;
    std::ifstream file(fileName);
    if (!file) {
        std::cout << "Could not open file " << fileName << "\n";
        return students;
    }

    std::string line;
    std::getline(file, line);  // skip header
    while (std::getline(file, line)) {
        Person person;
        if (person.readFromLine(line)) students.push_back(person);
    }
    return students;
}

void sortStudents(std::vector<Person>& students) {
    std::string answer;
    while (answer != "n" && answer != "s") {
        std::cout << "Sort by (n)ame or (s)urname? ";
        std::cin >> answer;
    }
    bool byName = answer == "n";
    std::sort(students.begin(), students.end(), [byName](const Person& a, const Person& b) {
        if (byName) {
            if (a.firstName() != b.firstName()) return a.firstName() < b.firstName();
            return a.lastName() < b.lastName();
        }
        if (a.lastName() != b.lastName()) return a.lastName() < b.lastName();
        return a.firstName() < b.firstName();
    });
}

// Output with one final points column, for the chosen method.
void printOneMethod(const std::vector<Person>& students, Method method) {
    std::string header = method == Method::Average ? "Final_Point(Aver.)" : "Final_Point(Med.)";
    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(18) << header << "\n";
    std::cout << std::string(48, '-') << "\n";
    for (const Person& person : students) {
        std::cout << std::left << std::setw(15) << person.firstName()
                  << std::setw(15) << person.lastName()
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(18) << person.finalPoints(method) << "\n";
    }
}

// Output with both final points columns.
void printBothMethods(const std::vector<Person>& students) {
    std::cout << "\n" << std::left << std::setw(15) << "Name"
              << std::setw(15) << "Surname"
              << std::right << std::setw(14) << "Final (Avg.)" << " |"
              << std::setw(13) << "Final (Med.)" << "\n";
    std::cout << std::string(59, '-') << "\n";
    for (const Person& person : students) std::cout << person << "\n";
}

int main() {
    std::cout << "1 - enter all data manually\n"
              << "2 - enter names, generate homework and exam results randomly\n"
              << "3 - read data from file Students.txt\n"
              << "Choice: ";
    int choice = readNumber(std::cin, 1, 3);

    std::vector<Person> students;
    if (choice == 3) {
        students = readFromFile("Students.txt");
        if (students.empty()) return 1;
        sortStudents(students);
        printBothMethods(students);
        return 0;
    }

    do {
        std::cout << "\nStudent " << students.size() + 1 << "\n";
        if (choice == 1) {
            Person person;
            std::cin >> person;
            students.push_back(person);
        } else {
            students.push_back(generatePerson());
        }
    } while (askYesNo("Add another student?"));

    Method method = askMethod();
    sortStudents(students);
    printOneMethod(students, method);
    return 0;
}
