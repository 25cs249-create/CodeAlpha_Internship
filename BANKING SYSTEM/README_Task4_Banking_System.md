# 🏦 Task 4 – Banking System

A console-based C++ banking application built with **object-oriented design**. It models real-world banking with `Customer`, `Account`, and `Transaction` classes, supporting deposits, withdrawals, fund transfers, transaction history, and persistent file storage.

---

## 📋 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Class Design](#class-design)
- [How It Works](#how-it-works)
- [Menu Reference](#menu-reference)
- [Runtime Files](#runtime-files)
- [Getting Started](#getting-started)
- [Sample Output](#sample-output)
- [Code Structure](#code-structure)
- [Limitations](#limitations)

---

## Overview

This program simulates a basic banking system where a teller (the user) can register new customers, open accounts for them, and perform financial transactions. All customer data, account balances, and transaction histories are automatically saved to disk and reloaded on the next run — so data persists between sessions.

---

## Features

### Customer Management
- ✅ Create new customers with auto-generated IDs (`CUST1001`, `CUST1002`, …)
- ✅ Store name, email, phone, and join date per customer
- ✅ Display full customer profile

### Account Management
- ✅ Open **Savings** or **Checking** accounts linked to a customer ID
- ✅ Auto-generated account numbers (`ACC1001`, `ACC1002`, …)
- ✅ Set an optional initial deposit when opening an account
- ✅ Display account type, owner, and current balance

### Transactions
- ✅ **Deposit** — add funds with optional note
- ✅ **Withdraw** — deduct funds with insufficient-balance guard
- ✅ **Fund Transfer** — move money between any two accounts
- ✅ Every transaction is recorded with type, amount, running balance, description, and timestamp

### Reporting
- ✅ View transaction history for any account (last 20 entries, tabular format)
- ✅ View full customer summary (profile + all linked accounts)
- ✅ List all customers or all accounts

### Persistence
- ✅ Customer records saved to `bank_customers.dat`
- ✅ Account records and full transaction history saved to `bank_accounts.dat`
- ✅ Data reloaded automatically on startup

---

## Class Design

```
┌─────────────────────────────────────────────────────────┐
│                        Bank                             │
│  - manages customers and accounts                       │
│  - handles all transaction operations                   │
│  - loads/saves data to disk                             │
└───────────┬─────────────────────┬───────────────────────┘
            │                     │
            ▼                     ▼
  ┌──────────────────┐   ┌──────────────────────────────┐
  │    Customer      │   │           Account             │
  │  - id            │   │  - accountNo                  │
  │  - name          │   │  - ownerID (→ Customer.id)    │
  │  - email         │   │  - type (Savings / Checking)  │
  │  - phone         │   │  - balance                    │
  │  - joinDate      │   │  - transactions[ ]            │
  └──────────────────┘   └──────────────┬───────────────┘
                                        │
                                        ▼
                            ┌──────────────────────┐
                            │     Transaction       │
                            │  - type (enum)        │
                            │  - amount             │
                            │  - balanceAfter       │
                            │  - description        │
                            │  - timestamp          │
                            └──────────────────────┘
```

### Transaction Types (enum)

| Enum Value      | Description                    |
|-----------------|--------------------------------|
| `DEPOSIT`       | Money added to account         |
| `WITHDRAWAL`    | Money removed from account     |
| `TRANSFER_OUT`  | Debit leg of a transfer        |
| `TRANSFER_IN`   | Credit leg of a transfer       |

---

## How It Works

### Deposit
```
User provides: Account No + Amount
  └─> Balance guard: amount must be > 0
  └─> balance += amount
  └─> Transaction(DEPOSIT) appended to account history
```

### Withdrawal
```
User provides: Account No + Amount
  └─> Guard: amount > 0
  └─> Guard: amount ≤ current balance  (prints error if not)
  └─> balance -= amount
  └─> Transaction(WITHDRAWAL) appended
```

### Fund Transfer
```
User provides: From Account + To Account + Amount
  └─> Guard: from ≠ to
  └─> Guard: from account has sufficient balance
  └─> from.balance -= amount  → Transaction(TRANSFER_OUT)
  └─> to.balance   += amount  → Transaction(TRANSFER_IN)
```

### Persistence (save / load)
```
On exit  → write customers to bank_customers.dat
         → write accounts + transactions to bank_accounts.dat

On start → read bank_customers.dat → populate customers list
         → read bank_accounts.dat  → populate accounts + replay transactions
```

---

## Menu Reference

```
MAIN MENU
  1. Create New Customer          → enter name, email, phone
  2. Open New Account             → choose customer, type, initial deposit
  3. Deposit                      → choose account, enter amount
  4. Withdraw                     → choose account, enter amount
  5. Fund Transfer                → choose source & destination accounts, amount
  6. View Account Details         → shows balance + last 20 transactions
  7. View Customer Summary        → shows profile + all linked accounts
  8. List All Customers           → prints ID, name, email for all customers
  9. List All Accounts            → prints account no, owner, type, balance
  0. Exit                         → saves data and quits
```

---

## Runtime Files

### `bank_customers.dat`
One customer per line, pipe-delimited:
```
CUST1001|Alice Smith|alice@email.com|0987654321|2025-03-11
CUST1002|Bob Jones|bob@email.com|1234567890|2025-03-11
```

### `bank_accounts.dat`
Account header followed by transaction records, terminated by `END`:
```
1003
ACC1001|CUST1001|0|5000.000000
TX|0,500.000000,500.000000,Initial deposit,2025-03-11 10:00:00
TX|0,4500.000000,5000.000000,Deposit,2025-03-11 10:05:00
TX|2,200.000000,4800.000000,Transfer to ACC1002,2025-03-11 10:10:00
END
ACC1002|CUST1002|1|200.000000
TX|3,200.000000,200.000000,Transfer from ACC1001,2025-03-11 10:10:00
END
```

> The first line is the next account ID counter to avoid duplicates across sessions.

---

## Getting Started

### Prerequisites
- C++17-compatible compiler (`g++`, `clang++`, or MSVC)

### Compile

```bash
g++ -std=c++17 -o bank task4_banking_system.cpp
```

### Run

**Linux / macOS:**
```bash
./bank
```

**Windows:**
```cmd
bank.exe
```

> `bank_customers.dat` and `bank_accounts.dat` are created automatically in the working directory.

---

## Sample Output

```
╔══════════════════════════════════════════╗
║     National C++ Bank                    ║
╚══════════════════════════════════════════╝

  Choice : 1
  Full name  : Alice Smith
  Email      : alice@email.com
  Phone      : 0987654321
  ✔ Customer created. ID: CUST1001

  Choice : 2
  Customer ID   : CUST1001
  Account type: [1] Savings  [2] Checking
  Choice         : 1
  Initial deposit: $1000
  ✔ Account opened. Account No: ACC1001

  Choice : 3
  Account No : ACC1001
  Amount     : $500
  ✔ Deposited $500.00. New balance: $1500.00

  Choice : 6
  Account No : ACC1001

  ┌──────────────────────────────────────────┐
  │  Account No : ACC1001                    │
  │  Type       : Savings                    │
  │  Owner ID   : CUST1001                   │
  │  Balance    : $               1500.00    │
  └──────────────────────────────────────────┘

  Transaction History (last 20 entries):
  ┌────────────────┬──────────────┬──────────────┬────────────────────┬──────────────────────┐
  │ Date/Time      │ Type         │ Amount       │ Balance After      │ Description          │
  ├────────────────┼──────────────┼──────────────┼────────────────────┼──────────────────────┤
  │ 2025-03-11     │ DEPOSIT      │     +1000.00 │ $            1000.00 │ Initial deposit      │
  │ 2025-03-11     │ DEPOSIT      │      +500.00 │ $            1500.00 │ Deposit              │
  └────────────────┴──────────────┴──────────────┴────────────────────┴──────────────────────┘
```

---

## Code Structure

| Class / Component          | Responsibility                                                        |
|----------------------------|-----------------------------------------------------------------------|
| `Transaction`              | Immutable record of one financial event; serializes/deserializes itself |
| `Transaction::Type`        | Enum: `DEPOSIT`, `WITHDRAWAL`, `TRANSFER_OUT`, `TRANSFER_IN`          |
| `Account`                  | Holds balance and transaction log; enforces business rules (guards)   |
| `Account::deposit()`       | Validates amount > 0, updates balance, appends transaction            |
| `Account::withdraw()`      | Validates amount > 0 and ≤ balance, updates balance, appends transaction |
| `Account::transferOut()`   | Debit side of a transfer with balance guard                           |
| `Account::transferIn()`    | Credit side of a transfer (no guard needed)                           |
| `Account::displayTransactions()` | Renders last N transactions in a bordered table               |
| `Customer`                 | Stores profile data; serializes/deserializes itself                   |
| `Bank`                     | Central controller — creates customers and accounts, routes all transactions, handles persistence |
| `Bank::saveData()`         | Writes all customers and accounts to `.dat` files                     |
| `Bank::loadData()`         | Reads and reconstructs all objects from `.dat` files on startup       |
| `main()`                   | Presents the numbered menu and dispatches user choices to `Bank`      |

---

## Limitations

- No user authentication — anyone who runs the program has full teller access
- No support for account closure or customer deletion
- No interest calculation (Savings accounts do not accrue interest)
- Concurrent access to `.dat` files is not safe (single-user, single-process only)
- Transaction history display is limited to the last 20 entries on screen (all are stored in memory and file)
- No currency support beyond a default single currency
