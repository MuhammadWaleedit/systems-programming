#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>

#include "Grades.h"
#include "Person.h"

// Reads an integer in [min, max]; asks again on invalid input.
int readNumber(std::istream& in, int min, int max);

bool askYesNo(const std::string& question);
Method askMethod();
std::string askFileName(const std::string& defaultName);

// Asks only for the name; homework and exam results are generated randomly.
Person generatePerson();

#endif
