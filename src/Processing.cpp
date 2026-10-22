#include "Processing.h"

#include <algorithm>

void sortByName(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(), [](const Person& a, const Person& b) {
        if (a.firstName() != b.firstName()) return a.firstName() < b.firstName();
        return a.lastName() < b.lastName();
    });
}

void sortBySurname(std::vector<Person>& students) {
    std::sort(students.begin(), students.end(), [](const Person& a, const Person& b) {
        if (a.lastName() != b.lastName()) return a.lastName() < b.lastName();
        return a.firstName() < b.firstName();
    });
}

void sortByFinal(std::vector<Person>& students, Method method) {
    std::sort(students.begin(), students.end(), [method](const Person& a, const Person& b) {
        return a.finalPoints(method) > b.finalPoints(method);
    });
}

void splitStudents(const std::vector<Person>& students, Method method,
                   std::vector<Person>& failed, std::vector<Person>& passed) {
    failed.clear();
    passed.clear();
    for (const Person& person : students) {
        if (person.finalPoints(method) < 5.0)
            failed.push_back(person);
        else
            passed.push_back(person);
    }
}
