
# Savvy Bank Management System
This C++ program implements a simple banking system called Savvy Bank. It allows users to register, login, deposit money, withdraw money, view balance, view transaction history, change password, and logout.

# Features:
User Registration:
Users can register with a unique username and password. Usernames are case-insensitive.
Passwords must not contain any spaces.

User Login:
Registered users can log in using their username and password.
Upon successful login, users can perform various banking operations.

Deposit:
Users can deposit money into their account.
Deposits are capped at $2000. Deposits exceeding this limit require a visit to the bank branch.

Withdrawal:
Users can withdraw money from their account, provided they have sufficient balance.
Withdrawals are capped at $2000. Amounts exceeding this limit require a visit to the bank branch.

View Balance:
Users can view their current account balance.

Transaction History:
Users can view their transaction history, including transaction type, amount, and timestamp.

Change Password:
Users can change their password after successful login.

Logout:
Users can logout from their account.

# Implementation Details:
 Data Storage:
User credentials (username and password) are stored in a text file (info.txt).
User balances are stored in a text file (balances.txt).
User transaction history is stored in a text file (transactions.txt).
Classes and Structures:

Transaction: Represents a banking transaction with attributes like type, amount, and timestamp.
Bank: Manages banking operations and user interactions.
File Operations:

Functions are provided to load and save user data, balances, and transaction history from/to files.

# How to Use:
Compile:
Compile the code using a C++ compiler (g++ or clang++).

Run:
Execute the compiled binary.

Menu Navigation:
Choose options from the main menu to perform various banking operations.
Follow on-screen instructions for inputting data and navigating the menu.

# Note:
This is a basic implementation intended for educational purposes.
Proper error handling and security measures are not implemented for production use.
For deposits or withdrawals exceeding $2000, users are advised to visit a bank branch.

# Author:
This program was authored by Hamzah Marie for educational purposes.

# License:
This program is released under the MIT License for educational and non-commercial use.
