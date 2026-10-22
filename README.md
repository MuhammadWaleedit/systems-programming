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

## Releases

- [v0.1](#v01) - `Person` class, manual/random/file input, average or median.
- [v0.2](#v02) - refactoring into header files, exception handling, test file
  generation, splitting students into failed/passed, speed analysis.

## v0.2

### Changes

- Code split into several header and source files:

  | File | Contents |
  |---|---|
  | `include/Person.h`, `src/Person.cpp` | `Person` class (rule of three + move operations), `operator>>`, `operator<<` |
  | `include/Grades.h`, `src/Grades.cpp` | `Method`, `average`, `median`, random result generation |
  | `include/Input.h`, `src/Input.cpp` | Validated console input |
  | `include/FileIO.h`, `src/FileIO.cpp` | Generating, reading and writing student files |
  | `include/Processing.h`, `src/Processing.cpp` | Sorting and splitting students |
  | `include/Timer.h` | `Timer` class (`std::chrono`) |
  | `src/main.cpp` | Menu and speed analysis |

- Exception handling:
  - a file that cannot be opened or created throws `std::runtime_error`;
  - a line with missing data throws `std::invalid_argument`, a result outside
    1-10 throws `std::out_of_range`; such lines are skipped and counted;
  - average/median of an empty list throws `std::domain_error`;
  - console input uses `std::stoi` and catches `std::invalid_argument` /
    `std::out_of_range`;
  - `main` catches `std::bad_alloc` and any other `std::exception`.
- Generation of five test files with 1 000, 10 000, 100 000, 1 000 000 and
  10 000 000 students (`Name1 Surname1`, `Name2 Surname2`, ...; 7 homework
  results and an exam result each) into the `data/` folder.
- Students are sorted by final points and split into two new containers:
  - **failed** - final points < 5.0,
  - **passed** - final points >= 5.0,

  which are written to `data/studentsN_failed.txt` and `data/studentsN_passed.txt`.
- Speed analysis: the time of each step is measured with `Timer`.

`Person` also has a move constructor and move assignment operator. Without
them every element move inside `std::vector` growth and `std::sort` would be a
deep copy (a new heap allocation for the homework vector).

### Build

With CMake:

```
cmake -S . -B build
cmake --build build
./build/final_points
```

Or directly with g++:

```
g++ -std=c++17 -O2 -static -Iinclude src/*.cpp -o final_points
```

### Program modes

```
1 - enter all data manually
2 - enter names, generate homework and exam results randomly
3 - read data from file and print it
4 - generate test files (1 000 ... 10 000 000 students)
5 - performance analysis (read, sort, split, write)
0 - exit
```

Run option `4` once to create the test files, then option `5` for the analysis.

### Speed analysis (`std::vector`)

Test system: AMD Ryzen 5 5600X (6 cores), 16 GB RAM, NVMe SSD, Windows 11,
g++ 16.2 (MinGW-w64), `-O2`. Final points by average.

| Records | Read (s) | Sort (s) | Split (s) | Write failed (s) | Write passed (s) | Total (s) |
|---:|---:|---:|---:|---:|---:|---:|
| 1 000 | 0.0008 | 0.0002 | 0.0001 | 0.0007 | 0.0006 | 0.0024 |
| 10 000 | 0.0058 | 0.0016 | 0.0014 | 0.0033 | 0.0048 | 0.0169 |
| 100 000 | 0.0517 | 0.0155 | 0.0182 | 0.0243 | 0.0353 | 0.1451 |
| 1 000 000 | 0.5508 | 0.2090 | 0.2145 | 0.2453 | 0.3603 | 1.5800 |
| 10 000 000 | 5.6280 | 2.7404 | 2.3234 | 2.6443 | 3.6774 | 17.0135 |

File generation time:

| Records | Generation (s) |
|---:|---:|
| 1 000 | 0.0020 |
| 10 000 | 0.0144 |
| 100 000 | 0.1319 |
| 1 000 000 | 1.3062 |
| 10 000 000 | 12.7887 |

All steps grow roughly linearly with the number of records (sorting is
O(n log n)). Reading the file is the slowest step: every line is parsed and a
`Person` with its own homework vector is created.

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
