# C++ Programming Tasks

A collection of C++ console applications covering CGPA calculation, user authentication, and a banking system.

---

## 📁 Project Structure

```
├── task1_cgpa_calculator.cpp    # CGPA Calculator
├── task2_login_system.cpp       # Login & Registration System
├── task4_banking_system.cpp     # Banking System
└── README.md
```

---

## Task 1 – CGPA Calculator

### Features
- Input number of courses per semester
- Accept letter grades (A, B+, C-, F …) **or** numeric grade points (0.0–4.0)
- Calculate grade points per course (`grade × credit hours`)
- Compute semester GPA and cumulative CGPA across multiple semesters
- Formatted tabular output with academic standing classification

### Compile & Run
```bash
g++ -std=c++17 -o cgpa task1_cgpa_calculator.cpp
./cgpa
```

### Grade Scale Used
| Letter | Points |
|--------|--------|
| A / A+ | 4.0    |
| A-     | 3.7    |
| B+     | 3.3    |
| B      | 3.0    |
| B-     | 2.7    |
| C+     | 2.3    |
| C      | 2.0    |
| D      | 1.0    |
| F      | 0.0    |

---

## Task 2 – Login & Registration System

### Features
- **Register** with username, password, and email
- Username validation (3–20 chars, alphanumeric + `_`)
- Password validation (min 6 chars, must contain a digit)
- Duplicate username detection
- Password stored as XOR-obfuscated hex in `users.dat`
- **Login** with up to 3 attempts before lockout
- **Change password** when logged in
- Activity log written to `login_log.txt`

### Compile & Run
```bash
g++ -std=c++17 -o login task2_login_system.cpp
./login
```

### Files Created at Runtime
| File | Purpose |
|------|---------|
| `users.dat` | User credentials database |
| `login_log.txt` | Timestamped activity log |

> **Security note:** The XOR obfuscation is for portability only.  
> For production, replace with a proper hashing library (bcrypt / Argon2).

---

## Task 4 – Banking System

### Classes
| Class | Responsibility |
|-------|----------------|
| `Customer` | Stores customer profile (ID, name, email, phone) |
| `Account` | Savings or Checking; tracks balance and transactions |
| `Transaction` | Immutable record of each financial event |
| `Bank` | Central controller; manages customers and accounts |

### Features
- Create customers with auto-generated IDs (`CUST1001`, …)
- Open Savings or Checking accounts with optional initial deposit
- **Deposit**, **Withdraw**, **Fund Transfer** between any two accounts
- Insufficient-funds guard on withdrawals and transfers
- Full transaction history display (type, amount, running balance, timestamp, description)
- Data persisted to `bank_customers.dat` and `bank_accounts.dat` between sessions

### Compile & Run
```bash
g++ -std=c++17 -o bank task4_banking_system.cpp
./bank
```

### Files Created at Runtime
| File | Purpose |
|------|---------|
| `bank_customers.dat` | Customer records |
| `bank_accounts.dat`  | Account records + full transaction history |

---

## Requirements
- C++17 or later
- Any standard C++ compiler: `g++`, `clang++`, or MSVC

---

## Build All at Once
```bash
g++ -std=c++17 -o cgpa  task1_cgpa_calculator.cpp
g++ -std=c++17 -o login task2_login_system.cpp
g++ -std=c++17 -o bank  task4_banking_system.cpp
```
