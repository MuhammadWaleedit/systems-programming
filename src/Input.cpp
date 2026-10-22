#include "Input.h"

#include <stdexcept>
#include <vector>

int readNumber(std::istream& in, int min, int max) {
    std::string token;
    while (in >> token) {
        try {
            size_t used = 0;
            int value = std::stoi(token, &used);
            if (used != token.size()) throw std::invalid_argument(token);
            if (value < min || value > max) throw std::out_of_range(token);
            return value;
        } catch (const std::invalid_argument&) {
            std::cout << "  \"" << token << "\" is not a number. Try again: ";
        } catch (const std::out_of_range&) {
            std::cout << "  Enter a number from " << min << " to " << max << ": ";
        }
    }
    throw std::runtime_error("Input ended unexpectedly");
}

bool askYesNo(const std::string& question) {
    std::string answer;
    while (true) {
        std::cout << question << " (y/n): ";
        if (!(std::cin >> answer)) throw std::runtime_error("Input ended unexpectedly");
        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
    }
}

Method askMethod() {
    std::string answer;
    while (true) {
        std::cout << "Calculate final points by (a)verage or (m)edian? ";
        if (!(std::cin >> answer)) throw std::runtime_error("Input ended unexpectedly");
        if (answer == "a" || answer == "A") return Method::Average;
        if (answer == "m" || answer == "M") return Method::Median;
    }
}

std::string askFileName(const std::string& defaultName) {
    std::cout << "File name (- for " << defaultName << "): ";
    std::string name;
    if (!(std::cin >> name)) throw std::runtime_error("Input ended unexpectedly");
    return name == "-" ? defaultName : name;
}

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
