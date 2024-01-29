#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <map>
#include <ctime>
#include <algorithm> // Include the algorithm header for std::transform




struct Transaction {
    std::string type;
    double amount;
    std::time_t timestamp;

    // Constructor to initialize timestamp
    Transaction() : timestamp(std::time(nullptr)) {}
};

class Bank {
private:
    std::vector<std::pair<std::string, std::string>> users;
    const std::string filename = "info.txt";
    std::map<std::string, double> userBalances;
    const std::string balanceFilename = "balances.txt";
    std::map<std::string, std::vector<Transaction>> userTransactions;
    std::map<std::string, std::time_t> lastUsernameChange;

    // Function to format a std::time_t timestamp as a string
    std::string formatTimestamp(std::time_t timestamp) {
        char buffer[26];
        struct tm timeinfo;

        if (localtime_s(&timeinfo, &timestamp) == 0) {
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
            return buffer;
        }
        else {
            return "Invalid Timestamp";
        }
    }

    void loadBalancesFromFile() {
        std::ifstream inputFile(balanceFilename);
        if (inputFile.is_open()) {
            std::string username;
            double balance;
            while (inputFile >> username >> balance) {
                userBalances[username] = balance;
            }
            inputFile.close();
        }
    }

    void saveBalancesToFile() {
        std::ofstream outputFile(balanceFilename);
        if (outputFile.is_open()) {
            for (const auto& pair : userBalances) {
                outputFile << pair.first << " " << pair.second << std::endl;
            }
            outputFile.close();
        }
    }

    void loadTransactionsFromFile() {
        std::ifstream inputFile("transactions.txt");
        if (inputFile.is_open()) {
            std::string username;
            std::string type;
            double amount;
            std::time_t timestamp;

            while (inputFile >> username >> type >> amount >> timestamp) {
                Transaction transaction;
                transaction.type = type;
                transaction.amount = amount;
                transaction.timestamp = timestamp;
                userTransactions[username].push_back(transaction);
            }
            inputFile.close();
        }
    }

    void saveTransactionsToFile() {
        std::ofstream outputFile("transactions.txt");
        if (outputFile.is_open()) {
            for (const auto& pair : userTransactions) {
                for (const Transaction& transaction : pair.second) {
                    outputFile << pair.first << " " << transaction.type << " " << transaction.amount << " " << transaction.timestamp << std::endl;
                }
            }
            outputFile.close();
        }
    }

public:
    Bank() {
        loadUsersFromFile();
        loadBalancesFromFile();
        loadTransactionsFromFile(); // Load transactions when the program starts
    }

    ~Bank() {
        saveUsersToFile();
        saveBalancesToFile();
        saveTransactionsToFile(); // Save transactions when the program exits
    }

    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Function to deposit money
   // Function to deposit money
    void deposit(const std::string& username, double amount) {
        if (userBalances.find(username) != userBalances.end()) {
            if (amount > 0) {
                if (amount <= 2000) { // Check if the deposit is within the limit
                    userBalances[username] += amount;
                    saveBalancesToFile();
                    // Add deposit transaction record
                    Transaction depositTransaction;
                    depositTransaction.type = "Deposit";
                    depositTransaction.amount = amount;
                    userTransactions[username].push_back(depositTransaction);
                    saveTransactionsToFile(); // Save transactions after adding a new one

                    std::cout << "Deposit successful. Your new balance is: " << userBalances[username] << std::endl;
                }
                else {
                    std::cout << "We appreciate your trust in Savvy Bank. However, for deposits exceeding $10,000, we kindly request you to visit one of our branches and let our experienced tellers handle this transaction." << std::endl;
                }
            }
            else {
                std::cout << "Invalid deposit amount. Deposit failed." << std::endl;
            }
        }
        else {
            std::cout << "User not found. Deposit failed." << std::endl;
        }
    }


    // Function to withdraw money
    void withdraw(const std::string& username, double amount) {
        if (userBalances.find(username) != userBalances.end()) {
            if (amount > 0 && userBalances[username] >= amount) {
                if (amount < 2000) { // Check if the withdrawal is within the limit
                    userBalances[username] -= amount;
                    saveBalancesToFile();
                    // Add withdrawal transaction record
                    Transaction withdrawTransaction;
                    withdrawTransaction.type = "Withdrawal";
                    withdrawTransaction.amount = amount;
                    userTransactions[username].push_back(withdrawTransaction);
                    saveTransactionsToFile(); // Save transactions after adding a new one

                    std::cout << "Withdrawal successful. Your new balance is: " << userBalances[username] << std::endl;
                }
                else {
                    std::cout << "We appreciate your trust in Savvy Bank. However, for withdrawals exceeding $10,000, we kindly request you to visit one of our branches and let our experienced tellers handle this transaction." << std::endl;
                }
            }
            else {
                std::cout << "Invalid withdrawal amount or insufficient balance. Withdrawal failed." << std::endl;
            }
        }
        else {
            std::cout << "User not found. Withdrawal failed." << std::endl;
        }
    }

    void displayTransactionHistory(const std::string& username) {
        if (userTransactions.find(username) != userTransactions.end()) {
            std::cout << "Transaction History for User: " << username << std::endl;
            for (const Transaction& transaction : userTransactions[username]) {
                std::cout << "Type: " << transaction.type << ", Amount: " << transaction.amount
                    << ", Timestamp: " << formatTimestamp(transaction.timestamp) << std::endl;
            }
        }
        else {
            std::cout << "User not found or no transaction history available." << std::endl;
        }
    }

    void printUsers() {
        std::cout << "User List:\n";
        for (const auto& user : users) {
            std::cout << "Username: " << user.first << ", Password: " << user.second << std::endl;
        }
    }

    void clearUsers() {
        users.clear();
        std::cout << "All users have been deleted." << std::endl;
    }

    void displayMenu() {
        int choice;
        do {
            std::cout << "\nWelcome to Savvy Bank!\n";
            std::cout << "Please choose from the following options: \n";
            std::cout << "1. Login\n";
            std::cout << "2. Register\n";
            std::cout << "3. Exit\n";

            if (!(std::cin >> choice)) {
                clearInputBuffer();
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                continue;
            }

            switch (choice) {
            case 1:
                loginUser();
                break;
            case 2:
                registerUser();
                break;
            case 3:
                return;
            default:
                std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                break;
            }
        } while (true);
    }

    void registerUser() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string username;
        bool validUsername = false;

        while (!validUsername) {
            std::cout << "Please enter a username (no spaces): ";
            std::getline(std::cin, username);

            if (usernameExists(username)) {
                std::cout << "Username already exists. Please enter a new username." << std::endl;
            }
            else if (username.find(' ') != std::string::npos) {
                std::cout << "Invalid username. Please enter a username without spaces." << std::endl;
            }
            else {
                validUsername = true;
            }
        }

        std::string password;
        std::cout << "Enter a password (no spaces): ";
        std::getline(std::cin, password);

        while (password.find(' ') != std::string::npos) {
            std::cout << "Your password contains spaces. Please enter a new valid password: ";
            std::getline(std::cin, password);
        }

        // Convert the username to lowercase for consistent storage
        std::string lowercaseUsername = username;
        std::transform(lowercaseUsername.begin(), lowercaseUsername.end(), lowercaseUsername.begin(), ::tolower);

        users.push_back(std::make_pair(lowercaseUsername, password));
        saveUsersToFile();

        std::cout << "Registration successful!" << std::endl;
    }

    void changeUsername(const std::string& username) {
        // Check if the user has changed their username in the last 30 days
        std::time_t currentTime = std::time(nullptr);
        if (lastUsernameChange.find(username) != lastUsernameChange.end()) {
            std::time_t lastChangeTime = lastUsernameChange[username];
            std::time_t thirtyDaysAgo = currentTime - 30 * 24 * 60 * 60; // 30 days in seconds

            if (lastChangeTime > thirtyDaysAgo) {
                std::cout << "You can only change your username once every 30 days. You can change it again after " << formatTimestamp(lastChangeTime + 30 * 24 * 60 * 60) << "." << std::endl;
                return;
            }
        }

        std::string newUsername;
        bool validUsername = false;

        while (!validUsername) {
            std::cout << "Please enter a new username (no spaces): ";
            std::getline(std::cin, newUsername);

            if (usernameExists(newUsername)) {
                std::cout << "Username already exists. Please enter a new username." << std::endl;
            }
            else if (newUsername.find(' ') != std::string::npos) {
                std::cout << "Invalid username. Please enter a username without spaces." << std::endl;
            }
            else {
                validUsername = true;
            }
        }

        // Update the username and timestamp of the last change
        for (auto& user : users) {
            if (user.first == username) {
                user.first = newUsername;
                saveUsersToFile();
                lastUsernameChange[username] = currentTime;
                std::cout << "Username changed successfully to: " << newUsername << std::endl;
                return;
            }
        }
    }

   


    void changePassword(const std::string& username) {
        std::string currentPassword;
        std::cout << "Enter your current password: ";
        std::string enteredPassword;
        std::getline(std::cin, enteredPassword);

        for (const auto& user : users) {
            if (user.first == username && user.second == enteredPassword) {
                std::string newPassword;
                std::cout << "Enter your new password (no spaces): ";
                std::getline(std::cin, newPassword);

                while (newPassword.find(' ') != std::string::npos) {
                    std::cout << "Your new password contains spaces. Please enter a new valid password: ";
                    std::getline(std::cin, newPassword);
                }

                if (newPassword == enteredPassword) {
                    std::cout << "New password cannot be the same as the current password." << std::endl;
                    std::cout << "1. Enter a new password" << std::endl;
                    std::cout << "2. Return to the main menu" << std::endl;

                    int choice;
                    if (!(std::cin >> choice)) {
                        clearInputBuffer();
                        std::cout << "Invalid choice. Returning to the main menu." << std::endl;
                        return;
                    }

                    switch (choice) {
                    case 1:
                        std::cin.ignore(); // Clear the newline character
                        changePassword(username); // Retry entering a new password
                        return;
                    case 2:
                        std::cin.ignore(); // Clear the newline character
                        return; // Return to the main menu
                    default:
                        std::cout << "Invalid choice. Returning to the main menu." << std::endl;
                        return;
                    }
                }

                // Update the password
                for (auto& user : users) {
                    if (user.first == username) {
                        user.second = newPassword;
                        saveUsersToFile();
                        std::cout << "Password changed successfully!" << std::endl;
                        return;
                    }
                }
            }
        }

        std::cout << "Incorrect current password. Password change failed." << std::endl;
    }

    void loginUser() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string username, password;
        bool validLogin = false;

        std::cout << "Please enter your username: ";
        std::getline(std::cin, username);

        std::transform(username.begin(), username.end(), username.begin(), ::tolower);

        std::cout << "Please enter your password: ";
        std::getline(std::cin, password);

        for (const auto& user : users) {
            std::string storedUsername = user.first;

            // Convert stored usernames to lowercase for comparison
            std::transform(storedUsername.begin(), storedUsername.end(), storedUsername.begin(), ::tolower);

            if (storedUsername == username && user.second == password) {
                validLogin = true;
                break;
            }
        }

        if (validLogin) {
            double balance = userBalances[username];
            std::cout << "Login successful! Welcome, " << username << "! Your balance is: " << balance << std::endl;

            int choice;
            do {
                std::cout << "Please choose from the following options: \n";
                std::cout << "1. Deposit\n";
                std::cout << "2. Withdraw\n";
                std::cout << "3. View Balance\n";
                std::cout << "4. Transaction History\n";
                std::cout << "5. Change Password\n";
                std::cout << "6. Logout\n";

                if (!(std::cin >> choice)) {
                    clearInputBuffer();
                    std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                    continue;
                }

                switch (choice) {
                case 1:
                    double depositAmount;
                    std::cout << "Enter the amount to deposit: ";
                    if (!(std::cin >> depositAmount)) {
                        clearInputBuffer();
                        std::cout << "Invalid input. Deposit failed." << std::endl;
                    }
                    else {
                        deposit(username, depositAmount);
                    }
                    break;
                case 2:
                    double withdrawAmount;
                    std::cout << "Enter the amount to withdraw: ";
                    if (!(std::cin >> withdrawAmount)) {
                        clearInputBuffer();
                        std::cout << "Invalid input. Withdrawal failed." << std::endl;
                    }
                    else {
                        withdraw(username, withdrawAmount);
                    }
                    break;
                case 3:
                    balance = userBalances[username];
                    std::cout << "Your balance is: " << balance << std::endl;
                    break;
                case 4:
                    displayTransactionHistory(username);
                    break;
                case 5:
                    clearInputBuffer();
                    changePassword(username);
                    break;
                case 6:
                    return;
                default:
                    clearInputBuffer();
                    std::cout << "Invalid choice. Please enter a valid option." << std::endl;
                    break;
                }
            } while (true);
        }
        else {
            std::cout << "Invalid username or password. Please try again." << std::endl;
        }
    }

    bool usernameExists(const std::string& username) {
        for (const auto& user : users) {
            if (user.first == username) {
                return true;
            }
        }
        return false;
    }

    void loadUsersFromFile() {
        users.clear();
        std::ifstream inputFile(filename);
        if (inputFile.is_open()) {
            std::string username, password;
            while (inputFile >> username >> password) {
                users.push_back(std::make_pair(username, password));
            }
            inputFile.close();
        }
    }

    void saveUsersToFile() {
        std::ofstream outputFile(filename);
        if (outputFile.is_open()) {
            for (const auto& user : users) {
                outputFile << user.first << " " << user.second << std::endl;
            }
            outputFile.close();
        }
    }
};

int main() {
    Bank user;
    user.displayMenu();
    return 0;
}
