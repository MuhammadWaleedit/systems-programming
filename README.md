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
