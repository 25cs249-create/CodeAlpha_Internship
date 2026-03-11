# 📊 Task 1 – CGPA Calculator

A console-based C++ application that computes a student's **semester GPA** and **cumulative CGPA** from course grades and credit hours, with formatted tabular output and academic standing classification.

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Grade Scale](#grade-scale)
- [Academic Standing](#academic-standing)
- [Getting Started](#getting-started)
- [Sample Output](#sample-output)
- [Code Structure](#code-structure)
- [Limitations](#limitations)

---

## Overview

This program helps students track their academic performance across multiple semesters. It accepts course names, credit hours, and grades (either as letter grades or numeric points), then calculates and displays both per-semester GPA and an overall CGPA.

---

## Features

- ✅ Input student name and number of semesters
- ✅ Per semester: enter number of courses, course names, credit hours, and grades
- ✅ Flexible grade input — letter grade (`A`, `B+`, `C-`) **or** numeric points (`0.0–4.0`)
- ✅ Calculates grade points per course: `grade × credit hours`
- ✅ Computes **semester GPA** after each semester
- ✅ Computes **overall CGPA** across all semesters at the end
- ✅ Formatted table output for each semester's results
- ✅ Final CGPA report with academic standing classification
- ✅ Input validation with error messages for out-of-range or invalid entries
- ✅ Supports up to **20 semesters** and **15 courses per semester**

---

## How It Works

### Formula

```
Grade Points (per course) = Grade × Credit Hours
Semester GPA = Σ(Grade Points) / Σ(Credit Hours)  [for that semester]
CGPA = Σ(All Grade Points) / Σ(All Credit Hours)   [across all semesters]
```

### Execution Flow

```
Start
 └─> Enter student name
 └─> Enter number of semesters
      └─> For each semester:
           └─> Enter number of courses
                └─> For each course:
                     ├─> Enter course name
                     ├─> Choose grade input mode (letter / numeric)
                     ├─> Enter grade
                     └─> Enter credit hours
           └─> Display semester summary table + GPA
 └─> Display final CGPA report + academic standing
End
```

---

## Grade Scale

| Letter Grade | Grade Points |
|:------------:|:------------:|
| A+ / A       | 4.0          |
| A-           | 3.7          |
| B+           | 3.3          |
| B            | 3.0          |
| B-           | 2.7          |
| C+           | 2.3          |
| C            | 2.0          |
| C-           | 1.7          |
| D+           | 1.3          |
| D            | 1.0          |
| F            | 0.0          |

> Letter grades are case-insensitive (`b+` is treated the same as `B+`).

---

## Academic Standing

| CGPA Range   | Classification                  |
|:------------:|:--------------------------------|
| 3.7 – 4.0    | Distinction / Summa Cum Laude   |
| 3.3 – 3.69   | Merit / Magna Cum Laude         |
| 2.0 – 3.29   | Pass                            |
| Below 2.0    | Below Minimum – Academic Probation |

---

## Getting Started

### Prerequisites
- C++17-compatible compiler (`g++`, `clang++`, or MSVC)

### Compile

```bash
g++ -std=c++17 -o cgpa task1_cgpa_calculator.cpp
```

### Run

**Linux / macOS:**
```bash
./cgpa
```

**Windows:**
```cmd
cgpa.exe
```

---

## Sample Output

```
╔══════════════════════════════════════════╗
║         CGPA CALCULATOR  (C++)           ║
╚══════════════════════════════════════════╝

Enter student name : Alice
Enter number of semesters : 1

──────────────────────────────────────────
  SEMESTER 1
──────────────────────────────────────────
  Number of courses this semester : 3

  Course 1:
    Name        : Mathematics
    Grade input mode:
      [1] Letter grade (A, B+, C-, etc.)
      [2] Numeric grade points (0.0 – 4.0)
    Choose (1 or 2) : 1
    Letter grade  : A
    Credit hours  : 4

  ...

  ┌────────────────────────────────────────────────────────┐
  │            SEMESTER 1 GRADE REPORT                     │
  ├──────────────────────┬───────────────┬────────────────┤
  │ Course               │ Credit Hours  │  Grade Points  │
  ├──────────────────────┼───────────────┼────────────────┤
  │ Mathematics          │             4 │          16.00 │
  │ Physics              │             3 │           9.90 │
  │ English              │             2 │           8.00 │
  ├──────────────────────┴───────────────┴────────────────┤
  │  Semester GPA : 3.7444                                 │
  └────────────────────────────────────────────────────────┘

╔══════════════════════════════════════════╗
║            FINAL CGPA REPORT             ║
╠══════════════════════════════════════════╣
║  Student : Alice                         ║
╠══════════════════════════════════════════╣
║  Semester 1 GPA : 3.7444                 ║
╠══════════════════════════════════════════╣
║  Total Credit Hours :     9              ║
║  Total Grade Points :  33.90             ║
╠══════════════════════════════════════════╣
║  OVERALL CGPA       :   3.7667           ║
╚══════════════════════════════════════════╝

  Academic Standing : Distinction / Summa Cum Laude
```

---

## Code Structure

| Component              | Description                                              |
|------------------------|----------------------------------------------------------|
| `struct Course`        | Holds course name, grade, credit hours, and grade points |
| `letterToPoints()`     | Converts letter grade string to numeric grade points     |
| `getDouble()`          | Validated floating-point input within a given range      |
| `getInt()`             | Validated integer input within a given range             |
| `calculateGPA()`       | Computes GPA from a list of `Course` objects             |
| `displaySemesterSummary()` | Prints formatted table for a semester               |
| `main()`               | Orchestrates input flow, loops over semesters, prints final report |

---

## Limitations

- No file persistence — results are not saved between runs
- Maximum of 20 semesters and 15 courses per semester
- Password-style hidden input is not applicable here (console display only)
- XOR obfuscation for credentials is not part of this task (see Task 2)
