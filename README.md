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
- [v0.25](#v025) - the same program with `std::list` and `std::deque`,
  speed analysis of all three containers.

## v0.25

### Changes

- Reading, sorting, splitting and writing are now function templates, so the
  same code works with `std::vector`, `std::list` and `std::deque`:
  - `include/FileIO.h` - `readStudents<Container>()`, `writeStudents<Container>()`;
  - `include/Processing.h` - `sortByFinal`, `sortByName`, `sortBySurname`,
    `splitStudents` (`src/Processing.cpp` was removed - template code lives in
    the header).
- `std::list` has no random access iterators, so `std::sort` cannot be used;
  `sortStudents()` calls `list::sort()` for lists (`if constexpr` + an `IsList`
  type trait) and `std::sort` for the other containers.
- Menu option `5` asks which container to test: `std::vector`, `std::list`,
  `std::deque` or all three.

### Speed analysis (`std::vector` vs `std::list` vs `std::deque`)

Test system: AMD Ryzen 5 5600X (6 cores), 16 GB RAM, NVMe SSD, Windows 11,
g++ 16.2 (MinGW-w64), `-O2`. Final points by average. Same five data files.

| Container | Records | Read (s) | Sort (s) | Split (s) | Write failed (s) | Write passed (s) | Total (s) |
|---|---:|---:|---:|---:|---:|---:|---:|
| std::vector | 1 000 | 0.0010 | 0.0003 | 0.0001 | 0.0007 | 0.0008 | 0.0029 |
| std::vector | 10 000 | 0.0058 | 0.0014 | 0.0010 | 0.0033 | 0.0039 | 0.0155 |
| std::vector | 100 000 | 0.0589 | 0.0172 | 0.0195 | 0.0270 | 0.0378 | 0.1603 |
| std::vector | 1 000 000 | 0.5609 | 0.2183 | 0.2153 | 0.2562 | 0.3490 | 1.5997 |
| std::vector | 10 000 000 | 5.4425 | 2.5708 | 2.2089 | 2.5109 | 3.4711 | 16.2042 |
| std::list | 1 000 | 0.0014 | 0.0001 | 0.0002 | 0.0009 | 0.0009 | 0.0034 |
| std::list | 10 000 | 0.0087 | 0.0017 | 0.0022 | 0.0043 | 0.0053 | 0.0221 |
| std::list | 100 000 | 0.0818 | 0.0192 | 0.0210 | 0.0266 | 0.0377 | 0.1862 |
| std::list | 1 000 000 | 0.5445 | 0.4780 | 0.2312 | 0.2621 | 0.3712 | 1.8871 |
| std::list | 10 000 000 | 5.3634 | 8.9626 | 2.7285 | 2.6552 | 3.8574 | 23.5672 |
| std::deque | 1 000 | 0.0007 | 0.0001 | 0.0001 | 0.0008 | 0.0006 | 0.0024 |
| std::deque | 10 000 | 0.0049 | 0.0015 | 0.0010 | 0.0030 | 0.0040 | 0.0143 |
| std::deque | 100 000 | 0.0513 | 0.0191 | 0.0170 | 0.0267 | 0.0365 | 0.1506 |
| std::deque | 1 000 000 | 0.5255 | 0.3182 | 0.2081 | 0.2614 | 0.3695 | 1.6826 |
| std::deque | 10 000 000 | 5.1920 | 3.5937 | 2.1472 | 2.5873 | 3.6131 | 17.1332 |

### Conclusions

- **Reading** takes about the same time with all three containers - most of
  the time is spent parsing lines and creating `Person` objects. `std::vector`
  also has to move all elements when it grows; `std::list` and `std::deque`
  never move existing elements.
- **Sorting** is where the containers differ most. `std::vector` is fastest:
  elements are stored contiguously, which suits the CPU cache. `std::deque`
  is about 1.4x slower (its elements are stored in blocks, so iterators do
  more work). `std::list::sort()` is about 3.5x slower on 10 000 000 records:
  the nodes are scattered in memory, so almost every comparison is a cache miss.
- **Splitting** and **writing** take about the same time with all three
  containers, because they only go through the elements once from start to end.
- Overall `std::vector` is the best choice for this task, `std::deque` is
  close behind, and `std::list` is the slowest.

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
