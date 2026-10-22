#include "FileIO.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

namespace {
const size_t kBufferSize = 1 << 20;  // write in 1 MB chunks

// Splits "Name Surname HW1 ... HWn Exam" into its parts.
// Throws std::invalid_argument if the line is not in this format.
void parseLine(const std::string& line, std::string& firstName, std::string& lastName,
               std::vector<int>& results) {
    const char* p = line.c_str();
    auto skipSpaces = [&p]() {
        while (*p != '\0' && std::isspace(static_cast<unsigned char>(*p))) ++p;
    };
    auto readWord = [&p, &skipSpaces]() {
        skipSpaces();
        const char* start = p;
        while (*p != '\0' && !std::isspace(static_cast<unsigned char>(*p))) ++p;
        return std::string(start, p);
    };

    firstName = readWord();
    lastName = readWord();
    if (firstName.empty() || lastName.empty())
        throw std::invalid_argument("Missing name or surname");

    results.clear();
    while (true) {
        skipSpaces();
        if (*p == '\0') break;
        char* end = nullptr;
        long value = std::strtol(p, &end, 10);
        if (end == p) throw std::invalid_argument("Not a number in line: " + line);
        results.push_back(static_cast<int>(value));
        p = end;
    }
    if (results.size() < 2)
        throw std::invalid_argument("Line needs at least one homework result and an exam result");
}
}  // namespace

void generateStudentFile(const std::string& fileName, size_t count, int homeworkCount) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot create file " + fileName);

    std::string buffer;
    buffer.reserve(kBufferSize + 256);
    char text[64];

    std::snprintf(text, sizeof(text), "%-16s%-17s", "Name", "Surname");
    buffer += text;
    for (int i = 1; i <= homeworkCount; ++i) {
        std::snprintf(text, sizeof(text), "%5s", ("HW" + std::to_string(i)).c_str());
        buffer += text;
    }
    buffer += " Exam\n";

    for (size_t i = 1; i <= count; ++i) {
        int length = std::snprintf(text, sizeof(text), "Name%-12zuSurname%-10zu", i, i);
        buffer.append(text, length);
        for (int j = 0; j <= homeworkCount; ++j) {  // homework results + exam
            length = std::snprintf(text, sizeof(text), "%5d", randomResult());
            buffer.append(text, length);
        }
        buffer += '\n';

        if (buffer.size() >= kBufferSize) {
            file.write(buffer.data(), buffer.size());
            buffer.clear();
        }
    }
    file.write(buffer.data(), buffer.size());
    if (!file) throw std::runtime_error("Failed writing to file " + fileName);
}

std::vector<Person> readStudents(const std::string& fileName, size_t& skippedLines) {
    std::ifstream file(fileName);
    if (!file) throw std::runtime_error("Cannot open file " + fileName);

    std::vector<Person> students;
    skippedLines = 0;

    std::string line, firstName, lastName;
    std::vector<int> results;
    std::getline(file, line);  // skip header

    while (std::getline(file, line)) {
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;  // empty line
        try {
            parseLine(line, firstName, lastName, results);
            int exam = results.back();
            results.pop_back();
            students.emplace_back(firstName, lastName, results, exam);
        } catch (const std::invalid_argument&) {
            ++skippedLines;
        } catch (const std::out_of_range&) {
            ++skippedLines;
        }
    }
    return students;
}

void writeStudents(const std::string& fileName, const std::vector<Person>& students, Method method) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot create file " + fileName);

    std::string buffer;
    buffer.reserve(kBufferSize + 256);
    char text[128];

    const char* header = method == Method::Average ? "Final (Avg.)" : "Final (Med.)";
    int length = std::snprintf(text, sizeof(text), "%-16s%-17s%12s\n", "Name", "Surname", header);
    buffer.append(text, length);

    for (const Person& person : students) {
        length = std::snprintf(text, sizeof(text), "%-16s%-17s%12.2f\n",
                               person.firstName().c_str(), person.lastName().c_str(),
                               person.finalPoints(method));
        buffer.append(text, length);

        if (buffer.size() >= kBufferSize) {
            file.write(buffer.data(), buffer.size());
            buffer.clear();
        }
    }
    file.write(buffer.data(), buffer.size());
    if (!file) throw std::runtime_error("Failed writing to file " + fileName);
}
