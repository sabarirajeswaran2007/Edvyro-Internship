#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct Expense {
    std::string description;
    std::string category;
    double amount;
};

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nInput closed. Exiting.\n";
        std::exit(0);
    }
    return trim(line);
}

std::string readNonEmpty(const std::string& prompt) {
    while (true) {
        std::string value = readLine(prompt);
        if (!value.empty()) return value;
        std::cout << "Input cannot be empty. Try again.\n";
    }
}

int readMenuChoice(const std::string& prompt, int low, int high) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            size_t pos = 0;
            int value = std::stoi(line, &pos);
            if (pos == line.size() && value >= low && value <= high) return value;
        } catch (const std::exception&) {
        }
        std::cout << "Enter a number between " << low << " and " << high << ".\n";
    }
}

double readAmount(const std::string& prompt) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            size_t pos = 0;
            double value = std::stod(line, &pos);
            if (pos == line.size() && std::isfinite(value) && value > 0 && value <= 1e9)
                return value;
        } catch (const std::exception&) {
        }
        std::cout << "Enter a positive number (for example 249.50).\n";
    }
}

void printMenu() {
    std::cout << "\n===== Expense Tracker =====\n"
              << "1. Add expense\n"
              << "2. List expenses\n"
              << "3. Total by category\n"
              << "4. Overall total\n"
              << "5. Exit\n";
}

void addExpense(std::vector<Expense>& expenses) {
    Expense e;
    e.description = readNonEmpty("Description: ");
    e.category = readNonEmpty("Category: ");
    e.amount = readAmount("Amount: ");
    expenses.push_back(e);
    std::cout << "Expense added.\n";
}

void listExpenses(const std::vector<Expense>& expenses) {
    if (expenses.empty()) {
        std::cout << "No expenses recorded yet.\n";
        return;
    }
    std::cout << "\n" << std::left << std::setw(4) << "#" << std::setw(24) << "Description"
              << std::setw(16) << "Category" << std::right << std::setw(12) << "Amount" << "\n"
              << std::string(56, '-') << "\n";
    std::cout << std::fixed << std::setprecision(2);
    for (size_t i = 0; i < expenses.size(); ++i) {
        std::cout << std::left << std::setw(4) << i + 1 << std::setw(24)
                  << expenses[i].description.substr(0, 22) << std::setw(16)
                  << expenses[i].category.substr(0, 14) << std::right << std::setw(12)
                  << expenses[i].amount << "\n";
    }
}

double categoryTotal(const std::vector<Expense>& expenses, const std::string& category) {
    double total = 0;
    std::string key = toLower(category);
    for (const auto& e : expenses)
        if (toLower(e.category) == key) total += e.amount;
    return total;
}

double overallTotal(const std::vector<Expense>& expenses) {
    double total = 0;
    for (const auto& e : expenses) total += e.amount;
    return total;
}

void showCategoryTotal(const std::vector<Expense>& expenses) {
    if (expenses.empty()) {
        std::cout << "No expenses recorded yet.\n";
        return;
    }
    std::string category = readNonEmpty("Category: ");
    double total = categoryTotal(expenses, category);
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total for '" << category << "': " << total << "\n";
}

void showOverallTotal(const std::vector<Expense>& expenses) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Overall total: " << overallTotal(expenses) << " ("
              << expenses.size() << " expenses)\n";
}

int main() {
    std::vector<Expense> expenses;
    bool running = true;
    while (running) {
        printMenu();
        switch (readMenuChoice("Choose an option (1-5): ", 1, 5)) {
            case 1: addExpense(expenses); break;
            case 2: listExpenses(expenses); break;
            case 3: showCategoryTotal(expenses); break;
            case 4: showOverallTotal(expenses); break;
            case 5: running = false; break;
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}
