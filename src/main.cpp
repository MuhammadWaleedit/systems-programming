#include <deque>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

#include "FileIO.h"
#include "Grades.h"
#include "Input.h"
#include "Person.h"
#include "Processing.h"
#include "Timer.h"

namespace {
const std::string kDataFolder = "data";
const std::vector<size_t> kTestSizes = {1000, 10000, 100000, 1000000, 10000000};
const int kGeneratedHomeworkCount = 7;

std::string testFileName(size_t size) {
    return kDataFolder + "/students" + std::to_string(size) + ".txt";
}

// "data/students1000.txt" -> "data/students1000_failed.txt"
std::string outputFileName(const std::string& inputName, const std::string& suffix) {
    std::string base = inputName;
    size_t dot = base.rfind('.');
    if (dot != std::string::npos) base = base.substr(0, dot);
    return base + "_" + suffix + ".txt";
}

void sortForPrinting(std::vector<Person>& students) {
    std::string answer;
    while (answer != "n" && answer != "s") {
        std::cout << "Sort by (n)ame or (s)urname? ";
        if (!(std::cin >> answer)) throw std::runtime_error("Input ended unexpectedly");
    }
    if (answer == "n")
        sortByName(students);
    else
        sortBySurname(students);
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

void enterStudents(bool randomResults) {
    std::vector<Person> students;
    do {
        std::cout << "\nStudent " << students.size() + 1 << "\n";
        if (randomResults) {
            students.push_back(generatePerson());
        } else {
            Person person;
            std::cin >> person;
            students.push_back(person);
        }
    } while (askYesNo("Add another student?"));

    Method method = askMethod();
    sortForPrinting(students);
    printOneMethod(students, method);
}

void readAndPrint() {
    std::string fileName = askFileName("Students.txt");
    size_t skipped = 0;
    std::vector<Person> students = readStudents<std::vector<Person>>(fileName, skipped);
    if (skipped > 0) std::cout << "Skipped " << skipped << " invalid line(s).\n";
    if (students.empty()) throw std::runtime_error("No students found in " + fileName);
    sortForPrinting(students);
    printBothMethods(students);
}

void generateTestFiles() {
    std::filesystem::create_directories(kDataFolder);
    for (size_t size : kTestSizes) {
        Timer timer;
        generateStudentFile(testFileName(size), size, kGeneratedHomeworkCount);
        std::cout << std::left << std::setw(32) << testFileName(size)
                  << "generated in " << std::fixed << std::setprecision(4)
                  << timer.elapsed() << " s\n";
    }
}

struct Timings {
    std::string container;
    size_t records = 0;
    double read = 0, sort = 0, split = 0, writeFailed = 0, writePassed = 0, total = 0;
};

template <typename Container>
Timings analyseFile(const std::string& fileName, Method method) {
    Timings t;
    t.container = containerName<Container>();
    Timer total, step;

    size_t skipped = 0;
    Container students = readStudents<Container>(fileName, skipped);
    t.read = step.elapsed();
    t.records = students.size();

    step.reset();
    sortByFinal(students, method);
    t.sort = step.elapsed();

    step.reset();
    Container failed, passed;
    splitStudents(students, method, failed, passed);
    t.split = step.elapsed();

    step.reset();
    writeStudents(outputFileName(fileName, "failed"), failed, method);
    t.writeFailed = step.elapsed();

    step.reset();
    writeStudents(outputFileName(fileName, "passed"), passed, method);
    t.writePassed = step.elapsed();

    t.total = total.elapsed();

    std::cout << "\n" << t.container << ": " << fileName << " (" << t.records << " records";
    if (skipped > 0) std::cout << ", " << skipped << " invalid lines skipped";
    std::cout << ")\n" << std::right << std::fixed << std::setprecision(4)
              << "  Read from file:      " << t.read << " s\n"
              << "  Sort:                " << t.sort << " s\n"
              << "  Split in two:        " << t.split << " s\n"
              << "  Write failed (" << std::setw(8) << failed.size() << "): " << t.writeFailed << " s\n"
              << "  Write passed (" << std::setw(8) << passed.size() << "): " << t.writePassed << " s\n"
              << "  Total:               " << t.total << " s\n";
    return t;
}

template <typename Container>
void analyseAllFiles(Method method, std::vector<Timings>& results) {
    for (size_t size : kTestSizes) {
        try {
            results.push_back(analyseFile<Container>(testFileName(size), method));
        } catch (const std::runtime_error& e) {
            std::cout << "\n" << e.what() << " - generate the test files first (option 4).\n";
        }
    }
}

void performanceAnalysis() {
    std::cout << "Container: 1 - std::vector, 2 - std::list, 3 - std::deque, 4 - all three: ";
    int container = readNumber(std::cin, 1, 4);
    Method method = askMethod();

    std::vector<Timings> results;
    if (container == 1 || container == 4) analyseAllFiles<std::vector<Person>>(method, results);
    if (container == 2 || container == 4) analyseAllFiles<std::list<Person>>(method, results);
    if (container == 3 || container == 4) analyseAllFiles<std::deque<Person>>(method, results);

    // Summary table (Markdown), ready to paste into README.
    std::cout << "\n| Container | Records | Read (s) | Sort (s) | Split (s) | Write failed (s) | Write passed (s) | Total (s) |\n"
              << "|---|---:|---:|---:|---:|---:|---:|---:|\n";
    for (const Timings& t : results) {
        std::cout << std::fixed << std::setprecision(4)
                  << "| " << t.container << " | " << t.records << " | " << t.read << " | " << t.sort
                  << " | " << t.split << " | " << t.writeFailed << " | " << t.writePassed
                  << " | " << t.total << " |\n";
    }
}
}  // namespace

int main() {
    while (true) {
        std::cout << "\n1 - enter all data manually\n"
                  << "2 - enter names, generate homework and exam results randomly\n"
                  << "3 - read data from file and print it\n"
                  << "4 - generate test files (1 000 ... 10 000 000 students)\n"
                  << "5 - performance analysis (read, sort, split, write)\n"
                  << "0 - exit\n"
                  << "Choice: ";
        try {
            int choice = readNumber(std::cin, 0, 5);
            switch (choice) {
                case 0: return 0;
                case 1: enterStudents(false); break;
                case 2: enterStudents(true); break;
                case 3: readAndPrint(); break;
                case 4: generateTestFiles(); break;
                case 5: performanceAnalysis(); break;
            }
        } catch (const std::bad_alloc&) {
            std::cout << "Error: not enough memory.\n";
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
            if (!std::cin) return 1;  // input ended, nothing more to do
        }
    }
}
