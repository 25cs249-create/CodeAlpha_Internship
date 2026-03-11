# 🔐 Task 2 – Login & Registration System

A console-based C++ application implementing a complete **user authentication system** with file-based credential storage, input validation, duplicate detection, login attempt limiting, and an activity log.

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Validation Rules](#validation-rules)
- [Password Storage](#password-storage)
- [Runtime Files](#runtime-files)
- [Getting Started](#getting-started)
- [Sample Output](#sample-output)
- [Code Structure](#code-structure)
- [Security Notes](#security-notes)
- [Limitations](#limitations)

---

## Overview

This program provides a full authentication workflow: new users can **register** an account with a validated username, secure password, and email. Returning users can **log in** and, once authenticated, manage their account (e.g., change password). All data is persisted in flat files, and every significant event is timestamped in an activity log.

---

## Features

### Registration
- ✅ Input username, password, confirm password, and email
- ✅ Validates username format (length, allowed characters)
- ✅ Validates password strength (minimum length, must contain a digit)
- ✅ Checks for duplicate usernames before creating an account
- ✅ Confirms password match before saving
- ✅ Stores credentials in `users.dat` using XOR-obfuscated hex encoding
- ✅ Records registration event in `login_log.txt` with timestamp

### Login
- ✅ Looks up username in stored credentials
- ✅ Verifies entered password against stored hash
- ✅ Allows up to **3 attempts** before denying access
- ✅ Displays account info (creation date, email) on successful login
- ✅ Logs success and failure events with timestamps

### Post-Login
- ✅ Change password (requires current password verification)
- ✅ Logout with log entry
- ✅ Clean exit saves all data

---

## How It Works

### Execution Flow

```
Start
 └─> Main Menu (Guest)
      ├─> [1] Register
      │    ├─> Validate username (format + duplicate check)
      │    ├─> Validate password (strength + confirmation)
      │    ├─> Collect email
      │    ├─> Hash password → store in users.dat
      │    └─> Log event
      │
      ├─> [2] Login
      │    ├─> Find username in users.dat
      │    ├─> Attempt password (up to 3 tries)
      │    └─> On success → Authenticated Menu
      │         ├─> [1] Change Password
      │         ├─> [2] Logout
      │         └─> [3] Exit
      │
      └─> [3] Exit
End
```

---

## Validation Rules

### Username
| Rule | Detail |
|------|--------|
| Minimum length | 3 characters |
| Maximum length | 20 characters |
| Allowed characters | Letters (`a-z`, `A-Z`), digits (`0-9`), underscore (`_`) |
| Uniqueness | Must not already exist in `users.dat` |

### Password
| Rule | Detail |
|------|--------|
| Minimum length | 6 characters |
| Must contain digit | At least one `0–9` character required |
| Confirmation | Must be re-entered identically |

---

## Password Storage

Passwords are **never stored in plaintext**. The storage pipeline is:

```
plaintext password
      │
      ▼
XOR each byte with key 0x5A
      │
      ▼
Encode result as uppercase hexadecimal string
      │
      ▼
Store in users.dat
```

Verification reverses the process and compares hashes:
```
entered password → hash → compare with stored hash
```

> See [Security Notes](#security-notes) for important caveats.

---

## Runtime Files

Two files are created automatically when the program runs:

### `users.dat`
Stores one user record per line in pipe-delimited format:

```
username|hashedPassword|email|createdAt
```

**Example:**
```
alice|3E2B1F4A...|alice@example.com|2025-03-11 10:22:45
bob|4F1C2E3D...|bob@mail.com|2025-03-11 11:05:12
```

> This file is created in the same directory as the executable.

### `login_log.txt`
Appends a timestamped entry for every significant event:

```
[2025-03-11 10:22:45] REGISTER success | user=alice
[2025-03-11 10:25:01] LOGIN success | user=alice
[2025-03-11 10:30:12] PASSWORD CHANGE | user=alice
[2025-03-11 10:31:00] LOGOUT | user=alice
[2025-03-11 10:35:44] LOGIN failed | user=bob (not found)
[2025-03-11 10:36:01] LOGIN failed | user=alice (too many attempts)
```

---

## Getting Started

### Prerequisites
- C++17-compatible compiler (`g++`, `clang++`, or MSVC)

### Compile

```bash
g++ -std=c++17 -o login task2_login_system.cpp
```

### Run

**Linux / macOS:**
```bash
./login
```

**Windows:**
```cmd
login.exe
```

> The `users.dat` and `login_log.txt` files will be created automatically in the working directory on first run.

---

## Sample Output

```
╔══════════════════════════════════════════╗
║   LOGIN & REGISTRATION SYSTEM  (C++)     ║
╚══════════════════════════════════════════╝

  ┌─────────────────────────────┐
  │  1. Register                │
  │  2. Login                   │
  │  3. Exit                    │
  └─────────────────────────────┘
  Choice : 1

  ── REGISTRATION ──────────────────────────
  Username : alice
  Password : ••••••••
  Confirm  : ••••••••
  Email    : alice@example.com

  ✔ Registration successful! You can now log in.

  Choice : 2

  ── LOGIN ─────────────────────────────────
  Username : alice
  Password (attempt 1/3) : ••••••••

  ✔ Login successful! Welcome, alice!
    Account created : 2025-03-11 10:22:45
    Email           : alice@example.com
```

---

## Code Structure

| Component              | Description                                                     |
|------------------------|-----------------------------------------------------------------|
| `xorObfuscate()`       | XOR each character in a string with a fixed key byte            |
| `toHex()` / `fromHex()`| Convert string to/from uppercase hexadecimal representation     |
| `hashPassword()`       | Applies XOR obfuscation then hex-encodes for storage            |
| `verifyPassword()`     | Hashes plaintext and compares with stored hash                  |
| `struct User`          | Holds username, hashed password, email, creation timestamp      |
| `serializeUser()`      | Converts `User` to a pipe-delimited string for file storage     |
| `deserializeUser()`    | Parses a pipe-delimited line back into a `User` struct          |
| `loadUsers()`          | Reads all user records from `users.dat`                         |
| `saveUsers()`          | Overwrites `users.dat` with a full list of user records         |
| `appendUser()`         | Appends a single new user to `users.dat` (efficient)            |
| `usernameExists()`     | Checks if a username already exists in the database             |
| `writeLog()`           | Appends a timestamped entry to `login_log.txt`                  |
| `isValidUsername()`    | Validates username length and character constraints             |
| `isValidPassword()`    | Validates password length and digit requirement                 |
| `registerUser()`       | Full registration flow with validation loops                    |
| `loginUser()`          | Login flow with attempt counter and lockout                     |
| `changePassword()`     | Post-login password update with current-password verification   |
| `main()`               | Menu loop switching between guest and authenticated views       |

---

## Security Notes

> ⚠️ **Important:** This project is designed for **educational purposes**.

| Aspect | Implementation | Production Recommendation |
|--------|---------------|--------------------------|
| Password storage | XOR + hex encoding | Use **bcrypt**, **Argon2**, or **scrypt** |
| Password input | Echoes to console | Use `termios` (Linux) / `SetConsoleMode` (Windows) to hide input |
| File permissions | No restrictions | Restrict `users.dat` to owner read/write only (`chmod 600`) |
| Brute force protection | 3-attempt lockout per session | Add persistent lockout + exponential backoff |
| Salt | Not implemented | Add per-user random salt before hashing |

---

## Limitations

- No password recovery or "forgot password" functionality
- Login lockout resets if the program is restarted (not persistent)
- Password input is visible while typing (terminal echo not suppressed)
- No encryption of the `users.dat` file itself
- Single-user session only (no multi-user concurrent access)
