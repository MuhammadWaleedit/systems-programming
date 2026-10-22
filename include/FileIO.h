#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <vector>

#include "Grades.h"
#include "Person.h"

// Writes a file with `count` random students named Name1 Surname1, Name2 Surname2, ...
// Throws std::runtime_error if the file cannot be created.
void generateStudentFile(const std::string& fileName, size_t count, int homeworkCount);

// Reads students from a file (first line is a header).
// Line format: Name Surname HW1 ... HWn Exam
// Throws std::runtime_error if the file cannot be opened.
// Lines with invalid data are skipped and counted in `skippedLines`.
std::vector<Person> readStudents(const std::string& fileName, size_t& skippedLines);

// Writes students with final points of the chosen method.
// Throws std::runtime_error if the file cannot be created.
void writeStudents(const std::string& fileName, const std::vector<Person>& students, Method method);

#endif
