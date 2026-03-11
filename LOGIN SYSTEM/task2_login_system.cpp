/*
 * TASK 2: Login & Registration System
 * Description: File-based user authentication system with secure
 *              password hashing, duplicate-username detection, and
 *              session management.
 * Author: Student Project
 * Language: C++
 *
 * NOTE: Passwords are stored using a simple XOR + base64-like
 *       obfuscation for portability (no external libraries needed).
 *       For production use, replace with bcrypt / Argon2.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <ctime>
#include<iomanip>

using namespace std;

// ─────────────────────────────────────────────
// Constants
// ─────────────────────────────────────────────
const string USER_DB_FILE  = "users.dat";
const string LOG_FILE      = "login_log.txt";
const int    MAX_LOGIN_ATTEMPTS = 3;
const char   XOR_KEY            = 0x5A; // obfuscation key

// ─────────────────────────────────────────────
// Simple XOR obfuscation (NOT cryptographic)
// ─────────────────────────────────────────────
string xorObfuscate(const string& input) {
    string output = input;
    for (char& c : output) c ^= XOR_KEY;
    return output;
}

// ─────────────────────────────────────────────
// Encode to hex string for safe file storage
// ─────────────────────────────────────────────
string toHex(const string& input) {
    static const char hex[] = "0123456789ABCDEF";
    string out;
    out.reserve(input.size() * 2);
    for (unsigned char c : input) {
        out += hex[c >> 4];
        out += hex[c & 0x0F];
    }
    return out;
}

string fromHex(const string& hex) {
    string out;
    out.reserve(hex.size() / 2);
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        auto hexVal = [](char c) -> unsigned char {
            if (c >= '0' && c <= '9') return c - '0';
            if (c >= 'A' && c <= 'F') return c - 'A' + 10;
            if (c >= 'a' && c <= 'f') return c - 'a' + 10;
            return 0;
        };
        out += (char)((hexVal(hex[i]) << 4) | hexVal(hex[i + 1]));
    }
    return out;
}

// Hash = hex(xor(password))
string hashPassword(const string& password) {
    return toHex(xorObfuscate(password));
}

bool verifyPassword(const string& plaintext, const string& stored) {
    return hashPassword(plaintext) == stored;
}

// ─────────────────────────────────────────────
// User record
// ─────────────────────────────────────────────
struct User {
    string username;
    string hashedPassword;
    string email;
    string createdAt;
};

// ─────────────────────────────────────────────
// Serialize / Deserialize user to/from line
// Format: username|hashedPassword|email|createdAt
// ─────────────────────────────────────────────
string serializeUser(const User& u) {
    return u.username + "|" + u.hashedPassword + "|"
         + u.email    + "|" + u.createdAt + "\n";
}

bool deserializeUser(const string& line, User& u) {
    stringstream ss(line);
    if (!getline(ss, u.username,        '|')) return false;
    if (!getline(ss, u.hashedPassword,  '|')) return false;
    if (!getline(ss, u.email,           '|')) return false;
    if (!getline(ss, u.createdAt))            return false;
    return !u.username.empty();
}

// ─────────────────────────────────────────────
// Load all users from file
// ─────────────────────────────────────────────
vector<User> loadUsers() {
    vector<User> users;
    ifstream file(USER_DB_FILE);
    if (!file.is_open()) return users;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        User u;
        if (deserializeUser(line, u))
            users.push_back(u);
    }
    return users;
}

// ─────────────────────────────────────────────
// Save all users to file (overwrite)
// ─────────────────────────────────────────────
void saveUsers(const vector<User>& users) {
    ofstream file(USER_DB_FILE, ios::trunc);
    for (const auto& u : users)
        file << serializeUser(u);
}

// ─────────────────────────────────────────────
// Append a single user (append mode – faster)
// ─────────────────────────────────────────────
void appendUser(const User& u) {
    ofstream file(USER_DB_FILE, ios::app);
    file << serializeUser(u);
}

// ─────────────────────────────────────────────
// Check if username already exists
// ─────────────────────────────────────────────
bool usernameExists(const string& username) {
    vector<User> users = loadUsers();
    for (const auto& u : users) {
        if (u.username == username) return true;
    }
    return false;
}

// ─────────────────────────────────────────────
// Current timestamp string
// ─────────────────────────────────────────────
string currentTimestamp() {
    time_t now = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buf);
}

// ─────────────────────────────────────────────
// Write to activity log
// ─────────────────────────────────────────────
void writeLog(const string& entry) {
    ofstream log(LOG_FILE, ios::app);
    log << "[" << currentTimestamp() << "] " << entry << "\n";
}

// ─────────────────────────────────────────────
// Input helpers
// ─────────────────────────────────────────────
string getInput(const string& prompt) {
    cout << prompt;
    string val;
    getline(cin, val);
    return val;
}

// Read password without echoing (platform-portable fallback)
string getPassword(const string& prompt) {
    cout << prompt;
    string password;
    // On most terminals this will echo — for a real app use
    // termios (Linux) or SetConsoleMode (Windows) to hide input.
    getline(cin, password);
    return password;
}

// ─────────────────────────────────────────────
// Validate username: 3-20 chars, alphanumeric + _
// ─────────────────────────────────────────────
bool isValidUsername(const string& username, string& errMsg) {
    if (username.size() < 3) {
        errMsg = "Username must be at least 3 characters.";
        return false;
    }
    if (username.size() > 20) {
        errMsg = "Username must be at most 20 characters.";
        return false;
    }
    for (char c : username) {
        if (!isalnum(c) && c != '_') {
            errMsg = "Username may only contain letters, digits, and underscores.";
            return false;
        }
    }
    return true;
}

// ─────────────────────────────────────────────
// Validate password: min 6 chars, at least 1 digit
// ─────────────────────────────────────────────
bool isValidPassword(const string& password, string& errMsg) {
    if (password.size() < 6) {
        errMsg = "Password must be at least 6 characters.";
        return false;
    }
    bool hasDigit = false;
    for (char c : password) if (isdigit(c)) { hasDigit = true; break; }
    if (!hasDigit) {
        errMsg = "Password must contain at least one digit.";
        return false;
    }
    return true;
}

// ─────────────────────────────────────────────
// REGISTRATION
// ─────────────────────────────────────────────
void registerUser() {
    cout << "\n  ── REGISTRATION ──────────────────────────\n";

    // Username
    string username;
    while (true) {
        username = getInput("  Username : ");
        string err;
        if (!isValidUsername(username, err)) {
            cout << "  [!] " << err << "\n";
            continue;
        }
        if (usernameExists(username)) {
            cout << "  [!] Username '" << username << "' is already taken.\n";
            continue;
        }
        break;
    }

    // Password
    string password;
    while (true) {
        password = getPassword("  Password : ");
        string err;
        if (!isValidPassword(password, err)) {
            cout << "  [!] " << err << "\n";
            continue;
        }
        string confirm = getPassword("  Confirm  : ");
        if (confirm != password) {
            cout << "  [!] Passwords do not match. Try again.\n";
            continue;
        }
        break;
    }

    // Email (optional but stored)
    string email = getInput("  Email    : ");

    User newUser;
    newUser.username       = username;
    newUser.hashedPassword = hashPassword(password);
    newUser.email          = email.empty() ? "N/A" : email;
    newUser.createdAt      = currentTimestamp();

    appendUser(newUser);
    writeLog("REGISTER success | user=" + username);

    cout << "\n  ✔ Registration successful! You can now log in.\n";
}

// ─────────────────────────────────────────────
// LOGIN
// ─────────────────────────────────────────────
bool loginUser(string& loggedInUser) {
    cout << "\n  ── LOGIN ─────────────────────────────────\n";

    string username = getInput("  Username : ");

    // Find user
    vector<User> users = loadUsers();
    User* target = nullptr;
    for (auto& u : users) {
        if (u.username == username) { target = &u; break; }
    }

    if (!target) {
        cout << "  [!] Username not found.\n";
        writeLog("LOGIN failed | user=" + username + " (not found)");
        return false;
    }

    // Attempt password up to MAX_LOGIN_ATTEMPTS
    for (int attempt = 1; attempt <= MAX_LOGIN_ATTEMPTS; attempt++) {
        string password = getPassword("  Password (attempt " + to_string(attempt)
                                      + "/" + to_string(MAX_LOGIN_ATTEMPTS) + ") : ");
        if (verifyPassword(password, target->hashedPassword)) {
            loggedInUser = username;
            writeLog("LOGIN success | user=" + username);
            cout << "\n  ✔ Login successful! Welcome, " << username << "!\n";
            cout << "    Account created : " << target->createdAt << "\n";
            cout << "    Email           : " << target->email << "\n";
            return true;
        }
        cout << "  [!] Incorrect password.\n";
    }

    writeLog("LOGIN failed | user=" + username + " (too many attempts)");
    cout << "  [✗] Too many failed attempts. Access denied.\n";
    return false;
}

// ─────────────────────────────────────────────
// Change Password (post-login feature)
// ─────────────────────────────────────────────
void changePassword(const string& loggedInUser) {
    cout << "\n  ── CHANGE PASSWORD ───────────────────────\n";
    string oldPwd  = getPassword("  Current password : ");

    vector<User> users = loadUsers();
    for (auto& u : users) {
        if (u.username == loggedInUser) {
            if (!verifyPassword(oldPwd, u.hashedPassword)) {
                cout << "  [!] Incorrect current password.\n";
                return;
            }
            string newPwd;
            while (true) {
                newPwd = getPassword("  New password     : ");
                string err;
                if (!isValidPassword(newPwd, err)) {
                    cout << "  [!] " << err << "\n"; continue;
                }
                string confirm = getPassword("  Confirm new      : ");
                if (confirm != newPwd) {
                    cout << "  [!] Passwords do not match.\n"; continue;
                }
                break;
            }
            u.hashedPassword = hashPassword(newPwd);
            saveUsers(users);
            writeLog("PASSWORD CHANGE | user=" + loggedInUser);
            cout << "  ✔ Password changed successfully.\n";
            return;
        }
    }
    cout << "  [!] User not found.\n";
}

// ─────────────────────────────────────────────
// MAIN MENU
// ─────────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════════════════╗\n";
    cout << "║   LOGIN & REGISTRATION SYSTEM  (C++)     ║\n";
    cout << "╚══════════════════════════════════════════╝\n";

    string loggedInUser = "";
    bool   running      = true;

    while (running) {
        if (loggedInUser.empty()) {
            // Guest menu
            cout << "\n  ┌─────────────────────────────┐\n";
            cout << "  │  1. Register                │\n";
            cout << "  │  2. Login                   │\n";
            cout << "  │  3. Exit                    │\n";
            cout << "  └─────────────────────────────┘\n";
            cout << "  Choice : ";
            string choice; getline(cin, choice);

            if      (choice == "1") registerUser();
            else if (choice == "2") loginUser(loggedInUser);
            else if (choice == "3") { running = false; }
            else    cout << "  [!] Invalid choice.\n";
        } else {
            // Authenticated menu
            cout << "\n  ┌─────────────────────────────┐\n";
            cout << "  │  Logged in as: "
                 << left << setw(14) << loggedInUser << "│\n";
            cout << "  ├─────────────────────────────┤\n";
            cout << "  │  1. Change Password         │\n";
            cout << "  │  2. Logout                  │\n";
            cout << "  │  3. Exit                    │\n";
            cout << "  └─────────────────────────────┘\n";
            cout << "  Choice : ";
            string choice; getline(cin, choice);

            if      (choice == "1") changePassword(loggedInUser);
            else if (choice == "2") {
                writeLog("LOGOUT | user=" + loggedInUser);
                cout << "  ✔ Logged out.\n";
                loggedInUser = "";
            }
            else if (choice == "3") { running = false; }
            else    cout << "  [!] Invalid choice.\n";
        }
    }

    cout << "\n  Goodbye!\n";
    return 0;
}
