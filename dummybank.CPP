#include <iostream>
#include <fstream>
#include <string>

class Account {
private:
    // Keep account number, password, name, and balance private for security
    unsigned int acc;
    std::string pass;
    std::string name;
    float balance;

public:
    // Setup default values when an account object is created
    Account() : acc(0), pass(""), name(""), balance(0.0f) {}

    // Quick functions to grab balance and account details safely
    float getBalance() const { return balance; }
    unsigned int getAccountNo() const { return acc; }

    // Core banking options
    void withdraw();
    void deposit();
    void displayDetails() const;
    void signup();
    bool login();
    
    // Save and load stuff to a text file
    void saveData() const;
    void loadData();
};

void Account::withdraw() {
    float amount;
    std::cout << "\nEnter the Withdraw Amount: ";
    std::cin >> amount;
    
    if (balance >= amount) {
        balance -= amount;
        std::cout << "\n$" << amount << " taken out. Current balance: $" << balance << "\n";
        saveData(); // Save changes right away
    } else {
        std::cout << "\nError: Not enough money in your account.\n";
    }
}

void Account::deposit() {
    float amount;
    std::cout << "\nEnter Amount to be deposited: ";
    std::cin >> amount;
    
    if (amount > 0) {
        balance += amount;
        std::cout << "\n$" << amount << " added to balance successfully.\n";
        saveData(); // Save changes right away
    } else {
        std::cout << "\nError: Deposit must be more than 0.\n";
    }
}

void Account::displayDetails() const {
    std::cout << "\n====================================";
    std::cout << "\n\t\tSASTA BANK";
    std::cout << "\n====================================";
    std::cout << "\nAccount No:\t" << acc;
    std::cout << "\nName:\t\t" << name;
    std::cout << "\nBalance:\t$" << balance;
    std::cout << "\n====================================\n";
}

void Account::signup() {
    std::cout << "\n--- Welcome to SASTA BANK Portal ---\n";
    std::cout << "Enter your Name: ";
    std::cin.ignore(); // Clear any leftover spaces/newlines
    std::getline(std::cin, name); // Let user type full name with spaces
    
    std::cout << "Enter your 9-digit account code: ";
    std::cin >> acc;

    // Use a loop so the program doesn't crash if they mess up the password
    while (true) {
        std::cout << "Enter your password (at least 9 characters): ";
        std::cin >> pass;
        if (pass.length() >= 9) {
            break; // Password is long enough, move on
        }
        std::cout << "Error: Password is too short. Try again.\n\n";
    }

    std::cout << "\nNote: A minimum balance of $5000 is recommended.\n";
    std::cout << "Deposit minimum balance now? (1 = Yes, 2 = No): ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        deposit();
    } else {
        std::cout << "\nAccount created. Don't forget to add funds later.\n";
        saveData();
    }
}

bool Account::login() {
    unsigned int inputAcc;
    std::string inputPass;

    // Give them 3 tries to get it right
    for (int attempts = 1; attempts <= 3; ++attempts) {
        std::cout << "\n--- Login (Attempt " << attempts << "/3) ---";
        std::cout << "\nAccount No: ";
        std::cin >> inputAcc;
        std::cout << "Password: ";
        std::cin >> inputPass;

        if (inputAcc == acc && inputPass == pass) {
            std::cout << "\nLogin Successful!\n";
            return true;
        } else {
            std::cout << "Wrong account number or password.\n";
        }
    }

    std::cout << "\nFailed 3 times. Reset account details? (1 = Yes, 2 = No): ";
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        signup();
        return true; 
    }
    return false;
}

void Account::saveData() const {
    std::ofstream outFile("account_data.txt");
    if (outFile.is_open()) {
        outFile << acc << "\n" << pass << "\n" << balance << "\n" << name << "\n";
        outFile.close();
    } else {
        std::cerr << "File Error: Could not save data.\n";
    }
}

void Account::loadData() {
    std::ifstream inFile("account_data.txt");
    if (inFile.is_open()) {
        inFile >> acc >> pass >> balance;
        inFile.ignore(); // Clean up formatting boundary
        std::getline(inFile, name); // Read the full name line
        inFile.close();
    }
}

int main() {
    Account userAccount;
    userAccount.loadData(); // Load saved user info if the file exists

    std::cout << "\n====================================";
    std::cout << "\n\tWelcome to Sasta Bank";
    std::cout << "\n====================================";

    int choice;
    bool systemRunning = true;

    while (systemRunning) {
        std::cout << "\nMAIN MENU:\n1: Register Account\n2: Login\n3: Exit\nChoice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                userAccount.signup();
                break;
            case 2:
                if (userAccount.login()) {
                    bool sessionActive = true;
                    while (sessionActive) {
                        std::cout << "\nSERVICES:\n1: Withdraw\n2: Deposit\n3: View Details\n4: Logout\nChoice: ";
                        std::cin >> choice;
                        switch (choice) {
                            case 1: userAccount.withdraw(); break;
                            case 2: userAccount.deposit(); break;
                            case 3: userAccount.displayDetails(); break;
                            case 4: sessionActive = false; break; // Logs out to main menu
                            default: std::cout << "Invalid choice.\n";
                        }
                    }
                }
                break;
            case 3:
                systemRunning = false;
                std::cout << "\nThanks for using Sasta Bank. Bye!\n";
                break;
            default:
                std::cout << "Invalid menu choice.\n";
        }
    }
    return 0;
}
