/*
 * TASK 1: CGPA Calculator
 * Description: Calculates semester GPA and overall CGPA for a student
 *              based on course grades and credit hours.
 * Author: Student Project
 * Language: C++
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
// Structure to hold individual course data
// ─────────────────────────────────────────────
struct Course {
    string name;
    double grade;       // grade points (e.g., 4.0, 3.7, 3.3 ...)
    int    creditHours;
    double gradePoints; // grade * creditHours
};

// ─────────────────────────────────────────────
// Helper: convert letter grade to grade points
// ─────────────────────────────────────────────
double letterToPoints(const string& letter) {
    if (letter == "A+"  || letter == "a+")  return 4.0;
    if (letter == "A"   || letter == "a")   return 4.0;
    if (letter == "A-"  || letter == "a-")  return 3.7;
    if (letter == "B+"  || letter == "b+")  return 3.3;
    if (letter == "B"   || letter == "b")   return 3.0;
    if (letter == "B-"  || letter == "b-")  return 2.7;
    if (letter == "C+"  || letter == "c+")  return 2.3;
    if (letter == "C"   || letter == "c")   return 2.0;
    if (letter == "C-"  || letter == "c-")  return 1.7;
    if (letter == "D+"  || letter == "d+")  return 1.3;
    if (letter == "D"   || letter == "d")   return 1.0;
    if (letter == "F"   || letter == "f")   return 0.0;
    return -1.0; // invalid
}

// ─────────────────────────────────────────────
// Helper: get a valid double input
// ─────────────────────────────────────────────
double getDouble(const string& prompt, double minVal, double maxVal) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            return val;
        }
        cout << "  [!] Invalid input. Please enter a number between "
             << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ─────────────────────────────────────────────
// Helper: get a valid positive integer
// ─────────────────────────────────────────────
int getInt(const string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            return val;
        }
        cout << "  [!] Invalid input. Please enter an integer between "
             << minVal << " and " << maxVal << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ─────────────────────────────────────────────
// Calculate GPA for a list of courses
// ─────────────────────────────────────────────
double calculateGPA(const vector<Course>& courses) {
    double totalGradePoints = 0.0;
    int    totalCredits     = 0;
    for (const auto& c : courses) {
        totalGradePoints += c.gradePoints;
        totalCredits     += c.creditHours;
    }
    if (totalCredits == 0) return 0.0;
    return totalGradePoints / totalCredits;
}

// ─────────────────────────────────────────────
// Display a summary table for a semester
// ─────────────────────────────────────────────
void displaySemesterSummary(int semNum,
                             const vector<Course>& courses,
                             double semGPA)
{
    cout << "\n  ┌────────────────────────────────────────────────────────┐\n";
    cout << "  │            SEMESTER " << semNum << " GRADE REPORT                    │\n";
    cout << "  ├──────────────────────┬───────────────┬────────────────┤\n";
    cout << "  │ Course               │ Credit Hours  │  Grade Points  │\n";
    cout << "  ├──────────────────────┼───────────────┼────────────────┤\n";
    for (const auto& c : courses) {
        cout << "  │ " << left  << setw(20) << c.name
             << " │ " << right << setw(13) << c.creditHours
             << " │ " << right << setw(14) << fixed << setprecision(2) << c.gradePoints
             << " │\n";
    }
    cout << "  ├──────────────────────┴───────────────┴────────────────┤\n";
    cout << "  │  Semester GPA : " << fixed << setprecision(4) << semGPA
         << "                              │\n";
    cout << "  └────────────────────────────────────────────────────────┘\n";
}

// ─────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║         CGPA CALCULATOR  (C++)           ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    string studentName;
    cout << "Enter student name : ";
    cin.ignore();
    getline(cin, studentName);

    int numSemesters = getInt("Enter number of semesters : ", 1, 20);

    // Accumulate data across all semesters
    double totalGradePointsAll = 0.0;
    int    totalCreditsAll     = 0;
    vector<double> semesterGPAs;

    for (int s = 1; s <= numSemesters; s++) {
        cout << "\n──────────────────────────────────────────\n";
        cout << "  SEMESTER " << s << "\n";
        cout << "──────────────────────────────────────────\n";

        int numCourses = getInt("  Number of courses this semester : ", 1, 15);
        cin.ignore();

        vector<Course> semCourses;

        for (int i = 1; i <= numCourses; i++) {
            Course course;
            cout << "\n  Course " << i << ":\n";

            cout << "    Name        : ";
            getline(cin, course.name);
            if (course.name.empty()) course.name = "Course " + to_string(i);

            // Accept either letter grade or numeric grade
            cout << "    Grade input mode:\n"
                 << "      [1] Letter grade (A, B+, C-, etc.)\n"
                 << "      [2] Numeric grade points (0.0 – 4.0)\n";
            int mode = getInt("    Choose (1 or 2) : ", 1, 2);

            if (mode == 1) {
                string letter;
                double pts = -1.0;
                while (pts < 0) {
                    cout << "    Letter grade  : ";
                    cin >> letter;
                    pts = letterToPoints(letter);
                    if (pts < 0)
                        cout << "  [!] Unrecognized grade. Try again.\n";
                }
                course.grade = pts;
            } else {
                course.grade = getDouble("    Grade points  : ", 0.0, 4.0);
            }

            course.creditHours  = getInt("    Credit hours  : ", 1, 6);
            course.gradePoints  = course.grade * course.creditHours;

            cin.ignore();
            semCourses.push_back(course);

            totalGradePointsAll += course.gradePoints;
            totalCreditsAll     += course.creditHours;
        }

        double semGPA = calculateGPA(semCourses);
        semesterGPAs.push_back(semGPA);
        displaySemesterSummary(s, semCourses, semGPA);
    }

    // ── Final CGPA Report ──────────────────────────────────────
    double cgpa = (totalCreditsAll > 0)
                  ? totalGradePointsAll / totalCreditsAll
                  : 0.0;

    cout << "\n╔══════════════════════════════════════════╗\n";
    cout << "║            FINAL CGPA REPORT             ║\n";
    cout << "╠══════════════════════════════════════════╣\n";
    cout << "║  Student : " << left << setw(30) << studentName << "║\n";
    cout << "╠══════════════════════════════════════════╣\n";
    for (int s = 0; s < (int)semesterGPAs.size(); s++) {
        cout << "║  Semester " << (s + 1) << " GPA : "
             << right << fixed << setprecision(4) << semesterGPAs[s]
             << "                       ║\n";
    }
    cout << "╠══════════════════════════════════════════╣\n";
    cout << "║  Total Credit Hours : " << setw(5) << totalCreditsAll << "                 ║\n";
    cout << "║  Total Grade Points : " << setw(8) << fixed << setprecision(2)
         << totalGradePointsAll << "              ║\n";
    cout << "╠══════════════════════════════════════════╣\n";
    cout << "║  OVERALL CGPA       : " << setw(8) << fixed << setprecision(4)
         << cgpa << "              ║\n";
    cout << "╚══════════════════════════════════════════╝\n\n";

    // Grade classification
    cout << "  Academic Standing : ";
    if      (cgpa >= 3.7) cout << "Distinction / Summa Cum Laude\n";
    else if (cgpa >= 3.3) cout << "Merit / Magna Cum Laude\n";
    else if (cgpa >= 2.0) cout << "Pass\n";
    else                  cout << "Below Minimum – Academic Probation\n";

    cout << "\n  [Program ended. Press Enter to exit.]";
    cin.ignore();
    cin.get();
    return 0;
}
