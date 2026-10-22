#ifndef FILEIO_H
#define FILEIO_H

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Grades.h"
#include "Person.h"

// Writes a file with `count` random students named Name1 Surname1, Name2 Surname2, ...
// Throws std::runtime_error if the file cannot be created.
void generateStudentFile(const std::string& fileName, size_t count, int homeworkCount);

// Splits "Name Surname HW1 ... HWn Exam" into its parts.
// Throws std::invalid_argument if the line is not in this format.
void parseLine(const std::string& line, std::string& firstName, std::string& lastName,
               std::vector<int>& results);

// Output file lines: header and one line per student.
std::string outputHeader(Method method);
void appendOutputLine(std::string& buffer, const Person& person, Method method);

// Writes the buffer to the file when it is full (or always when `force`).
void flushBuffer(std::ofstream& file, std::string& buffer, bool force);

// Reads students from a file (first line is a header).
// Throws std::runtime_error if the file cannot be opened.
// Lines with invalid data are skipped and counted in `skippedLines`.
template <typename Container>
Container readStudents(const std::string& fileName, size_t& skippedLines) {
    std::ifstream file(fileName);
    if (!file) throw std::runtime_error("Cannot open file " + fileName);

    Container students;
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

// Writes students with final points of the chosen method.
// Throws std::runtime_error if the file cannot be created.
template <typename Container>
void writeStudents(const std::string& fileName, const Container& students, Method method) {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot create file " + fileName);

    std::string buffer = outputHeader(method);
    for (const Person& person : students) {
        appendOutputLine(buffer, person, method);
        flushBuffer(file, buffer, false);
    }
    flushBuffer(file, buffer, true);
    if (!file) throw std::runtime_error("Failed writing to file " + fileName);
}

#endif
