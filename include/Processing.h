#ifndef PROCESSING_H
#define PROCESSING_H

#include <algorithm>
#include <deque>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

#include "Grades.h"
#include "Person.h"

// The same code works with std::vector, std::list and std::deque.

template <typename Container>
struct IsList : std::false_type {};
template <typename T, typename Alloc>
struct IsList<std::list<T, Alloc>> : std::true_type {};

template <typename Container>
std::string containerName() {
    if constexpr (std::is_same_v<Container, std::vector<Person>>) return "std::vector";
    else if constexpr (std::is_same_v<Container, std::list<Person>>) return "std::list";
    else if constexpr (std::is_same_v<Container, std::deque<Person>>) return "std::deque";
    else return "container";
}

// std::list has no random access iterators, so it uses its own sort().
template <typename Container, typename Compare>
void sortStudents(Container& students, Compare compare) {
    if constexpr (IsList<Container>::value)
        students.sort(compare);
    else
        std::sort(students.begin(), students.end(), compare);
}

template <typename Container>
void sortByName(Container& students) {
    sortStudents(students, [](const Person& a, const Person& b) {
        if (a.firstName() != b.firstName()) return a.firstName() < b.firstName();
        return a.lastName() < b.lastName();
    });
}

template <typename Container>
void sortBySurname(Container& students) {
    sortStudents(students, [](const Person& a, const Person& b) {
        if (a.lastName() != b.lastName()) return a.lastName() < b.lastName();
        return a.firstName() < b.firstName();
    });
}

// Highest final points first.
template <typename Container>
void sortByFinal(Container& students, Method method) {
    sortStudents(students, [method](const Person& a, const Person& b) {
        return a.finalPoints(method) > b.finalPoints(method);
    });
}

// Copies students with final points < 5.0 to `failed`, the rest to `passed`.
template <typename Container>
void splitStudents(const Container& students, Method method, Container& failed, Container& passed) {
    failed.clear();
    passed.clear();
    for (const Person& person : students) {
        if (person.finalPoints(method) < 5.0)
            failed.push_back(person);
        else
            passed.push_back(person);
    }
}

#endif
