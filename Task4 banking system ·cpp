/*
 * TASK 4: Banking System
 * Description: OOP-based banking system with Customer, Account, and
 *              Transaction classes. Supports deposits, withdrawals,
 *              fund transfers, and transaction history.
 * Author: Student Project
 * Language: C++
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
// Forward declarations
// ─────────────────────────────────────────────
class Account;

// ─────────────────────────────────────────────
// Transaction class
// ─────────────────────────────────────────────
class Transaction {
public:
    enum Type { DEPOSIT, WITHDRAWAL, TRANSFER_OUT, TRANSFER_IN };

    Transaction(Type type, double amount, double balanceAfter,
                const string& description, const string& timestamp = "")
        : type_(type), amount_(amount), balanceAfter_(balanceAfter),
          description_(description)
    {
        if (timestamp.empty()) {
            time_t now = time(nullptr);
            char buf[32];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
            timestamp_ = string(buf);
        } else {
            timestamp_ = timestamp;
        }
    }

    // Getters
    Type        getType()         const { return type_;         }
    double      getAmount()       const { return amount_;       }
    double      getBalanceAfter() const { return balanceAfter_; }
    string      getDescription()  const { return description_;  }
    string      getTimestamp()    const { return timestamp_;    }

    string typeToString() const {
        switch (type_) {
            case DEPOSIT:      return "DEPOSIT     ";
            case WITHDRAWAL:   return "WITHDRAWAL  ";
            case TRANSFER_OUT: return "TRANSFER OUT";
            case TRANSFER_IN:  return "TRANSFER IN ";
        }
        return "UNKNOWN";
    }

    // Serialize to CSV line
    string serialize() const {
        return to_string((int)type_) + "," +
               to_string(amount_)   + "," +
               to_string(balanceAfter_) + "," +
               description_ + "," +
               timestamp_;
    }

    // Deserialize from CSV line
    static Transaction deserialize(const string& line) {
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ',')) tokens.push_back(token);
        if (tokens.size() < 5)
            return Transaction(DEPOSIT, 0, 0, "error");
        Type   t   = (Type)stoi(tokens[0]);
        double amt = stod(tokens[1]);
        double bal = stod(tokens[2]);
        string desc = tokens[3];
        string ts   = tokens[4];
        return Transaction(t, amt, bal, desc, ts);
    }

private:
    Type   type_;
    double amount_;
    double balanceAfter_;
    string description_;
    string timestamp_;
};

// ─────────────────────────────────────────────
// Account class
// ─────────────────────────────────────────────
class Account {
public:
    enum AccountType { SAVINGS, CHECKING };

    Account(const string& accountNo, const string& ownerID,
            AccountType type, double initialBalance = 0.0)
        : accountNo_(accountNo), ownerID_(ownerID),
          type_(type), balance_(initialBalance)
    {}

    // Getters
    string      getAccountNo()  const { return accountNo_; }
    string      getOwnerID()    const { return ownerID_;   }
    AccountType getType()       const { return type_;      }
    double      getBalance()    const { return balance_;   }
    const vector<Transaction>& getTransactions() const { return transactions_; }

    string typeString() const {
        return type_ == SAVINGS ? "Savings" : "Checking";
    }

    // ── Deposit ───────────────────────────────
    bool deposit(double amount, const string& note = "") {
        if (amount <= 0) {
            cout << "  [!] Deposit amount must be positive.\n";
            return false;
        }
        balance_ += amount;
        string desc = "Deposit" + (note.empty() ? "" : " (" + note + ")");
        transactions_.emplace_back(Transaction::DEPOSIT, amount, balance_, desc);
        return true;
    }

    // ── Withdrawal ────────────────────────────
    bool withdraw(double amount, const string& note = "") {
        if (amount <= 0) {
            cout << "  [!] Withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance_) {
            cout << "  [!] Insufficient funds. Available: $"
                 << fixed << setprecision(2) << balance_ << "\n";
            return false;
        }
        balance_ -= amount;
        string desc = "Withdrawal" + (note.empty() ? "" : " (" + note + ")");
        transactions_.emplace_back(Transaction::WITHDRAWAL, amount, balance_, desc);
        return true;
    }

    // ── Transfer Out (debit this account) ─────
    bool transferOut(double amount, const string& toAccNo) {
        if (amount <= 0) {
            cout << "  [!] Transfer amount must be positive.\n";
            return false;
        }
        if (amount > balance_) {
            cout << "  [!] Insufficient funds. Available: $"
                 << fixed << setprecision(2) << balance_ << "\n";
            return false;
        }
        balance_ -= amount;
        transactions_.emplace_back(Transaction::TRANSFER_OUT, amount, balance_,
                                   "Transfer to " + toAccNo);
        return true;
    }

    // ── Transfer In (credit this account) ─────
    void transferIn(double amount, const string& fromAccNo) {
        balance_ += amount;
        transactions_.emplace_back(Transaction::TRANSFER_IN, amount, balance_,
                                   "Transfer from " + fromAccNo);
    }

    // ── Display account info ──────────────────
    void displayInfo() const {
        cout << "\n  ┌──────────────────────────────────────────┐\n";
        cout << "  │  Account No : " << left << setw(27) << accountNo_ << "│\n";
        cout << "  │  Type       : " << left << setw(27) << typeString() << "│\n";
        cout << "  │  Owner ID   : " << left << setw(27) << ownerID_    << "│\n";
        cout << "  │  Balance    : $"<< right<< setw(26) << fixed << setprecision(2)
             << balance_ << "│\n";
        cout << "  └──────────────────────────────────────────┘\n";
    }

    // ── Display transaction history ───────────
    void displayTransactions(int limit = 10) const {
        cout << "\n  Transaction History (last " << limit << " entries):\n";
        cout << "  ┌────────────────┬──────────────┬──────────────┬────────────────────┬──────────────────────┐\n";
        cout << "  │ Date/Time      │ Type         │ Amount       │ Balance After      │ Description          │\n";
        cout << "  ├────────────────┼──────────────┼──────────────┼────────────────────┼──────────────────────┤\n";

        int start = max(0, (int)transactions_.size() - limit);
        for (int i = start; i < (int)transactions_.size(); i++) {
            const auto& t = transactions_[i];
            string sign = (t.getType() == Transaction::DEPOSIT ||
                           t.getType() == Transaction::TRANSFER_IN) ? "+" : "-";
            cout << "  │ " << left  << setw(14) << t.getTimestamp().substr(0,10)
                 << " │ " << left  << setw(12) << t.typeToString()
                 << " │ " << sign  << right << setw(11) << fixed << setprecision(2) << t.getAmount()
                 << " │ $"<< right << setw(17) << fixed << setprecision(2) << t.getBalanceAfter()
                 << " │ " << left  << setw(20) << t.getDescription().substr(0, 20)
                 << " │\n";
        }
        if (transactions_.empty())
            cout << "  │                      No transactions yet.                                          │\n";

        cout << "  └────────────────┴──────────────┴──────────────┴────────────────────┴──────────────────────┘\n";
    }

    // ── Persistence helpers ───────────────────
    string serialize() const {
        string line = accountNo_ + "|" + ownerID_ + "|"
                    + to_string((int)type_) + "|"
                    + to_string(balance_) + "|"
                    + to_string(transactions_.size()) + "\n";
        for (const auto& t : transactions_)
            line += "  TX:" + t.serialize() + "\n";
        return line;
    }

    void addTransaction(const Transaction& t) {
        transactions_.push_back(t);
    }

    void setBalance(double b) { balance_ = b; }

private:
    string      accountNo_;
    string      ownerID_;
    AccountType type_;
    double      balance_;
    vector<Transaction> transactions_;
};

// ─────────────────────────────────────────────
// Customer class
// ─────────────────────────────────────────────
class Customer {
public:
    Customer(const string& id, const string& name,
             const string& email, const string& phone)
        : id_(id), name_(name), email_(email), phone_(phone)
    {
        time_t now = time(nullptr);
        char buf[32];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
        joinDate_ = string(buf);
    }

    Customer(const string& id, const string& name,
             const string& email, const string& phone,
             const string& joinDate)
        : id_(id), name_(name), email_(email),
          phone_(phone), joinDate_(joinDate)
    {}

    // Getters
    string getID()       const { return id_;       }
    string getName()     const { return name_;     }
    string getEmail()    const { return email_;    }
    string getPhone()    const { return phone_;    }
    string getJoinDate() const { return joinDate_; }

    void displayInfo() const {
        cout << "\n  ┌──────────────────────────────────────────┐\n";
        cout << "  │  Customer ID  : " << left << setw(25) << id_       << "│\n";
        cout << "  │  Name         : " << left << setw(25) << name_     << "│\n";
        cout << "  │  Email        : " << left << setw(25) << email_    << "│\n";
        cout << "  │  Phone        : " << left << setw(25) << phone_    << "│\n";
        cout << "  │  Member since : " << left << setw(25) << joinDate_ << "│\n";
        cout << "  └──────────────────────────────────────────┘\n";
    }

    string serialize() const {
        return id_ + "|" + name_ + "|" + email_ + "|" + phone_ + "|" + joinDate_;
    }

    static Customer deserialize(const string& line) {
        stringstream ss(line);
        string id, name, email, phone, joinDate;
        getline(ss, id,       '|');
        getline(ss, name,     '|');
        getline(ss, email,    '|');
        getline(ss, phone,    '|');
        getline(ss, joinDate);
        return Customer(id, name, email, phone, joinDate);
    }

private:
    string id_, name_, email_, phone_, joinDate_;
};

// ─────────────────────────────────────────────
// Bank class – central controller
// ─────────────────────────────────────────────
class Bank {
public:
    const string DB_CUSTOMERS = "bank_customers.dat";
    const string DB_ACCOUNTS  = "bank_accounts.dat";

    Bank(const string& bankName) : bankName_(bankName), nextAccID_(1001) {
        loadData();
    }

    ~Bank() { saveData(); }

    // ── ID generation ─────────────────────────
    string generateCustomerID() {
        return "CUST" + to_string(customers_.size() + 1001);
    }

    string generateAccountNo() {
        string accNo = "ACC" + to_string(nextAccID_++);
        return accNo;
    }

    // ── Create customer ───────────────────────
    Customer* createCustomer(const string& name,
                              const string& email,
                              const string& phone)
    {
        string id = generateCustomerID();
        customers_.emplace_back(id, name, email, phone);
        cout << "  ✔ Customer created. ID: " << id << "\n";
        return &customers_.back();
    }

    // ── Open account ──────────────────────────
    Account* openAccount(const string& customerID,
                          Account::AccountType type,
                          double initialDeposit)
    {
        if (!findCustomer(customerID)) {
            cout << "  [!] Customer ID not found.\n";
            return nullptr;
        }
        if (initialDeposit < 0) {
            cout << "  [!] Initial deposit cannot be negative.\n";
            return nullptr;
        }
        string accNo = generateAccountNo();
        accounts_.emplace_back(accNo, customerID, type, 0.0);
        Account* acc = &accounts_.back();
        if (initialDeposit > 0)
            acc->deposit(initialDeposit, "Initial deposit");
        cout << "  ✔ Account opened. Account No: " << accNo << "\n";
        return acc;
    }

    // ── Find helpers ──────────────────────────
    Customer* findCustomer(const string& id) {
        for (auto& c : customers_)
            if (c.getID() == id) return &c;
        return nullptr;
    }

    Account* findAccount(const string& accNo) {
        for (auto& a : accounts_)
            if (a.getAccountNo() == accNo) return &a;
        return nullptr;
    }

    // ── Deposit ───────────────────────────────
    bool deposit(const string& accNo, double amount) {
        Account* acc = findAccount(accNo);
        if (!acc) { cout << "  [!] Account not found.\n"; return false; }
        bool ok = acc->deposit(amount);
        if (ok) cout << "  ✔ Deposited $" << fixed << setprecision(2) << amount
                     << ". New balance: $" << acc->getBalance() << "\n";
        return ok;
    }

    // ── Withdraw ──────────────────────────────
    bool withdraw(const string& accNo, double amount) {
        Account* acc = findAccount(accNo);
        if (!acc) { cout << "  [!] Account not found.\n"; return false; }
        bool ok = acc->withdraw(amount);
        if (ok) cout << "  ✔ Withdrew $" << fixed << setprecision(2) << amount
                     << ". New balance: $" << acc->getBalance() << "\n";
        return ok;
    }

    // ── Transfer ──────────────────────────────
    bool transfer(const string& fromAccNo,
                  const string& toAccNo,
                  double amount)
    {
        if (fromAccNo == toAccNo) {
            cout << "  [!] Cannot transfer to the same account.\n";
            return false;
        }
        Account* from = findAccount(fromAccNo);
        Account* to   = findAccount(toAccNo);
        if (!from) { cout << "  [!] Source account not found.\n";      return false; }
        if (!to)   { cout << "  [!] Destination account not found.\n"; return false; }

        if (!from->transferOut(amount, toAccNo)) return false;
        to->transferIn(amount, fromAccNo);
        cout << "  ✔ Transferred $" << fixed << setprecision(2) << amount
             << " from " << fromAccNo << " to " << toAccNo << ".\n";
        return true;
    }

    // ── Display customer with all accounts ────
    void displayCustomerSummary(const string& customerID) {
        Customer* cust = findCustomer(customerID);
        if (!cust) { cout << "  [!] Customer not found.\n"; return; }
        cust->displayInfo();

        cout << "\n  Accounts:\n";
        bool found = false;
        for (auto& acc : accounts_) {
            if (acc.getOwnerID() == customerID) {
                acc.displayInfo();
                found = true;
            }
        }
        if (!found) cout << "  (No accounts)\n";
    }

    // ── List all customers ────────────────────
    void listCustomers() const {
        cout << "\n  ── All Customers ─────────────────────────\n";
        if (customers_.empty()) { cout << "  (None)\n"; return; }
        for (const auto& c : customers_)
            cout << "  " << c.getID() << "  " << c.getName()
                 << "  (" << c.getEmail() << ")\n";
    }

    // ── List all accounts ─────────────────────
    void listAccounts() const {
        cout << "\n  ── All Accounts ──────────────────────────\n";
        if (accounts_.empty()) { cout << "  (None)\n"; return; }
        for (const auto& a : accounts_)
            cout << "  " << a.getAccountNo()
                 << "  Owner:" << a.getOwnerID()
                 << "  Type:" << a.typeString()
                 << "  Balance:$" << fixed << setprecision(2) << a.getBalance() << "\n";
    }

    // ── Persistence ───────────────────────────
    void saveData() {
        // Save customers
        ofstream cf(DB_CUSTOMERS, ios::trunc);
        for (const auto& c : customers_)
            cf << c.serialize() << "\n";

        // Save accounts
        ofstream af(DB_ACCOUNTS, ios::trunc);
        af << nextAccID_ << "\n";
        for (const auto& a : accounts_) {
            af << a.getAccountNo() << "|"
               << a.getOwnerID()   << "|"
               << (int)a.getType() << "|"
               << a.getBalance()   << "\n";
            for (const auto& t : a.getTransactions())
                af << "TX|" << t.serialize() << "\n";
            af << "END\n";
        }
    }

    void loadData() {
        // Load customers
        ifstream cf(DB_CUSTOMERS);
        if (cf.is_open()) {
            string line;
            while (getline(cf, line))
                if (!line.empty())
                    customers_.push_back(Customer::deserialize(line));
        }

        // Load accounts
        ifstream af(DB_ACCOUNTS);
        if (!af.is_open()) return;

        string firstLine;
        getline(af, firstLine);
        if (!firstLine.empty()) nextAccID_ = stoi(firstLine);

        string line;
        Account* current = nullptr;
        while (getline(af, line)) {
            if (line.empty()) continue;
            if (line == "END") { current = nullptr; continue; }
            if (line.substr(0,3) == "TX|") {
                if (current)
                    current->addTransaction(
                        Transaction::deserialize(line.substr(3)));
                continue;
            }
            // Account header
            stringstream ss(line);
            string accNo, ownerID, typeStr, balStr;
            getline(ss, accNo,   '|');
            getline(ss, ownerID, '|');
            getline(ss, typeStr, '|');
            getline(ss, balStr);
            Account::AccountType t = (Account::AccountType)stoi(typeStr);
            double bal = stod(balStr);
            accounts_.emplace_back(accNo, ownerID, t, 0.0);
            current = &accounts_.back();
            current->setBalance(bal);
        }
    }

    string getBankName() const { return bankName_; }

private:
    string           bankName_;
    int              nextAccID_;
    vector<Customer> customers_;
    vector<Account>  accounts_;
};

// ─────────────────────────────────────────────
// Input helpers
// ─────────────────────────────────────────────
string getLine(const string& prompt) {
    cout << prompt;
    string s; getline(cin, s); return s;
}

double getDouble(const string& prompt) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= 0) { cin.ignore(); return val; }
        cout << "  [!] Invalid amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ─────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────
int main() {
    Bank bank("National C++ Bank");

    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║     " << left << setw(37) << bank.getBankName() << "║\n";
    cout << "╚══════════════════════════════════════════╝\n";

    bool running = true;
    while (running) {
        cout << "\n  ┌─────────────────────────────────────────┐\n";
        cout << "  │  MAIN MENU                              │\n";
        cout << "  ├─────────────────────────────────────────┤\n";
        cout << "  │  1. Create New Customer                 │\n";
        cout << "  │  2. Open New Account                    │\n";
        cout << "  │  3. Deposit                             │\n";
        cout << "  │  4. Withdraw                            │\n";
        cout << "  │  5. Fund Transfer                       │\n";
        cout << "  │  6. View Account Details & Transactions │\n";
        cout << "  │  7. View Customer Summary               │\n";
        cout << "  │  8. List All Customers                  │\n";
        cout << "  │  9. List All Accounts                   │\n";
        cout << "  │  0. Exit                                │\n";
        cout << "  └─────────────────────────────────────────┘\n";
        cout << "  Choice : ";
        string choice; getline(cin, choice);

        if (choice == "1") {
            // Create customer
            string name  = getLine("  Full name  : ");
            string email = getLine("  Email      : ");
            string phone = getLine("  Phone      : ");
            bank.createCustomer(name, email, phone);

        } else if (choice == "2") {
            // Open account
            bank.listCustomers();
            string cid  = getLine("  Customer ID   : ");
            cout << "  Account type: [1] Savings  [2] Checking\n";
            string tChoice = getLine("  Choice         : ");
            Account::AccountType aType =
                (tChoice == "2") ? Account::CHECKING : Account::SAVINGS;
            double init = getDouble("  Initial deposit: $");
            bank.openAccount(cid, aType, init);

        } else if (choice == "3") {
            // Deposit
            bank.listAccounts();
            string accNo = getLine("  Account No : ");
            double amt   = getDouble("  Amount     : $");
            bank.deposit(accNo, amt);

        } else if (choice == "4") {
            // Withdraw
            bank.listAccounts();
            string accNo = getLine("  Account No : ");
            double amt   = getDouble("  Amount     : $");
            bank.withdraw(accNo, amt);

        } else if (choice == "5") {
            // Transfer
            bank.listAccounts();
            string from = getLine("  From Account No : ");
            string to   = getLine("  To   Account No : ");
            double amt  = getDouble("  Amount          : $");
            bank.transfer(from, to, amt);

        } else if (choice == "6") {
            // Account transactions
            bank.listAccounts();
            string accNo = getLine("  Account No : ");
            Account* acc = bank.findAccount(accNo);
            if (!acc) { cout << "  [!] Account not found.\n"; }
            else {
                acc->displayInfo();
                acc->displayTransactions(20);
            }

        } else if (choice == "7") {
            // Customer summary
            bank.listCustomers();
            string cid = getLine("  Customer ID : ");
            bank.displayCustomerSummary(cid);

        } else if (choice == "8") {
            bank.listCustomers();

        } else if (choice == "9") {
            bank.listAccounts();

        } else if (choice == "0") {
            running = false;
        } else {
            cout << "  [!] Invalid choice.\n";
        }
    }

    cout << "\n  Data saved. Goodbye!\n";
    return 0;
}
