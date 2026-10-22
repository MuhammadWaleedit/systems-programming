#ifndef PROCESSING_H
#define PROCESSING_H

#include <vector>

#include "Grades.h"
#include "Person.h"

void sortByName(std::vector<Person>& students);
void sortBySurname(std::vector<Person>& students);

// Highest final points first.
void sortByFinal(std::vector<Person>& students, Method method);

// Copies students with final points < 5.0 to `failed`, the rest to `passed`.
void splitStudents(const std::vector<Person>& students, Method method,
                   std::vector<Person>& failed, std::vector<Person>& passed);

#endif
