# Student Final Points Calculator

A C++ console application that collects students' homework and exam results
and calculates each student's final points.

## Formula

Final points are calculated from the homework results (average **or** median,
chosen by the user) and the exam result:

```
Final points = 0.4 * (Average of HW | Median of HW) + 0.6 * Exam
```

All results use a 10-point grading scale.

## v0.1

### Features

- `Person` class holding the first name, surname, homework results, exam result
  and final points, implementing the rule of three (copy constructor, copy
  assignment operator, destructor).
- Overloaded `operator>>` (data input) and `operator<<` (data output).
- Final points calculated by average or median (user's choice).
- The number of homework results is not known in advance; the user enters `0`
  when all results are entered. Results are stored in `std::vector`.
- Homework and exam results can be generated randomly (`<random>` + `<chrono>`).
- Data can be read from `Students.txt`; output shows both average and median
  final points, sorted by name or surname.

### Build and run

```
g++ -std=c++17 -O2 -static main.cpp -o final_points
./final_points
```

`-static` avoids picking up a different `libstdc++` DLL from `PATH` on Windows
(MinGW).

### Program modes

```
1 - enter all data manually
2 - enter names, generate homework and exam results randomly
3 - read data from file Students.txt
```

### `Students.txt` format

```
Name        Surname       HW1   HW2   HW3   HW4   HW5   Exam
Jonas       Jonaitis        8     9    10     6    10      9
Petras      Petraitis       7    10     8     5     4      6
...
```

The first line is a header. Each line has a name, a surname, any number of
homework results and the exam result as the last number.

### Example output (mode 3, sorted by surname)

```
Name           Surname          Final (Avg.) | Final (Med.)
-----------------------------------------------------------
Eva            Adomaityte               8.48 |         8.40
Dovydas        Balciunas                3.40 |         3.40
Greta          Butkute                  4.76 |         4.60
Jonas          Jonaitis                 8.84 |         9.00
Ana            Karenina                 9.44 |         9.60
...
```
