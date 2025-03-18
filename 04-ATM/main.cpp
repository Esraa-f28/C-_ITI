#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <memory>
#include <iomanip>
#include <openssl/sha.h>
#include <termios.h>
#include <unistd.h>
#include <limits>
#include <algorithm>
#include <fstream>
#include <string>



std::string getHiddenInput() {
    std::string input;
    struct termios oldt, newt;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO; 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    std::getline(std::cin, input); 
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    std::cout << std::string(input.length(), '*') << std::endl; 
    return input;
}

std::string hashPin(const std::string& pin) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(pin.c_str()), pin.length(), hash);

    std::ostringstream oss;
    for (unsigned char c : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return oss.str();
}

class Transaction {
public:
    std::string type;
    double amount;
    std::string timeStamp;

    Transaction(std::string type, double amount) : type(type), amount(amount) {
        std::ostringstream oss;
        std::time_t now = std::time(nullptr);
        oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
        timeStamp = oss.str();
    }   
};

class Account {
private:
    std::string accNumber;
    std::string accName;
    std::string accType;
    std::string hashedPin;
    double balance;
    std::vector<Transaction> transactions;

public:
    Account(std::string accNum, std::string accName, std::string accType, std::string pin, double initialBalance)
        : accNumber(accNum), accName(accName), accType(accType), hashedPin(hashPin(pin)), balance(initialBalance) {}

        void saveToFile() {
            std::ofstream file(accNumber + "_transactions.txt", std::ios::app);
            if (!file) {
                std::cerr << "Error opening file!" << std::endl;
                return;
            }
            // Save the latest transaction
            Transaction lastTransaction = transactions.back();
            file << lastTransaction.type << "  " << lastTransaction.amount << "  " <<lastTransaction.timeStamp<< std::endl;
            file.close();
        }
        
    void deposit(double amount) {
        balance += amount;
        transactions.emplace_back("Deposit", amount);
        saveToFile(); // Save to file
        std::cout << "\nYou deposited: " << amount << " | Your current balance: " << balance << std::endl;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            std::cout << "Insufficient funds!" << std::endl;
            return false;
        }
        balance -= amount;
        transactions.emplace_back("Withdrawal", amount);
        std::cout << "Withdrawn: " << amount << " | Your current balance: " << balance << std::endl;
        saveToFile();
        return true;
    }

    std::string getAccountNumber() const { 
        return accNumber; 
    }

    void showTransactionHistory() {
        std::ifstream file(accNumber + "_transactions.txt");
        if (!file.is_open()) {
            std::cout << "No transaction history found.\n";
            return;
        }
        std::cout << "\n--- Transaction History ---\n";
        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    }

    void loadTransactions() {
        std::ifstream file(accNumber + "_transactions.txt");
        if (!file.is_open()) return;
        
        std::string type, timeStamp;
        double amount;
        while (file >> type >> amount) {
            file.ignore();  // Ignore the space between amount and timestamp
            std::getline(file, timeStamp); // Read the full timestamp
            transactions.emplace_back(type, amount);
            transactions.back().timeStamp = timeStamp; // Assign correct timestamp
        }
        file.close();
    }

    void showBalance() {
        std::cout << "Your current balance: " << balance << std::endl;
    }

    bool authenticate(const std::string& enteredPin) {
        return hashPin(enteredPin) == hashedPin;
    }
};

class ATM {
private:
    std::vector<std::shared_ptr<Account>> accounts;

public:
    void addAccount(std::string accNum, std::string accName, std::string accType, std::string pin, double initialBalance) {
        accounts.push_back(std::make_shared<Account>(accNum, accName, accType, pin, initialBalance));
    }

    std::shared_ptr<Account> authenticate(std::string accNum) {
        auto it = std::find_if(accounts.begin(), accounts.end(), 
            [&accNum](const std::shared_ptr<Account>& acc) { return acc->getAccountNumber() == accNum; });
    
        if (it == accounts.end()) {
            std::cout << "Account not found. Exiting...\n";
            return nullptr;
        }
    
        std::shared_ptr<Account> account = *it;
        int attempts = 0;
    
        while (attempts < 3) {
            std::cout << "Enter PIN: ";
            std::string enteredPin = getHiddenInput();
    
            if (account->authenticate(enteredPin)) {
                std::cout << "Authentication successful!\n";
                return account;
            }
    
            std::cout << "Incorrect PIN. Attempts left: " << (2 - attempts) << "\n";
            attempts++;
        }
    
        std::cout << "Too many incorrect attempts. Exiting...\n";
        return nullptr;
    }
    
    void displayMenu(std::shared_ptr<Account> userAcc) {
        if (!userAcc) {
            std::cout << "No valid account found.\n";
            return;
        }

        int choice;
        do {
            std::cout << "\n--- ATM Menu ---\n";
            std::cout << "1. Deposit Money\n";
            std::cout << "2. Withdraw Money\n";
            std::cout << "3. Show Balance\n";
            std::cout << "4. Show Transaction History\n";
            std::cout << "5. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            double amount;
            switch (choice) {
                case 1:
                    std::cout << "Enter amount to deposit: ";
                    std::cin >> amount;
                    userAcc->deposit(amount);
                    break;

                case 2:
                    std::cout << "Enter amount to withdraw: ";
                    std::cin >> amount;
                    userAcc->withdraw(amount);
                    break;

                case 3:
                    userAcc->showBalance();
                    break;

                case 4:
                    userAcc->showTransactionHistory();
                    break;

                case 5:
                    std::cout << "Thank you for using the ATM!\n";
                    break;

                default:
                    std::cout << "Invalid choice, please try again!\n";
            }
        } while (choice != 5);
    }
};

int main() {
    ATM atm;
    atm.addAccount("12345", "John Doe", "Savings", "1234", 5000.0);
    atm.addAccount("67890", "Jane Smith", "Checking", "5678", 3000.0);

    std::string accNum;
    std::cout << "Enter Account Number: ";
    std::cin >> accNum;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::shared_ptr<Account> userAcc = atm.authenticate(accNum);
    if (!userAcc) return 0;

    atm.displayMenu(userAcc);
    return 0;
}
