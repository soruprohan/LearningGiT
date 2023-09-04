#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Transaction {
private:
    int transactionId;
    string transactionType;
    double amount;
public:
    Transaction(int id, const string& type, double amt)
        : transactionId(id), transactionType(type), amount(amt) {}
    int getId() {
        return transactionId;
    }
    string getType() {
        return transactionType;
    }
    double getAmount() {
        return amount;
    }
};

class Account {
private:
    string name;
    int accountNumber;
    string accountType;
    double balance;
    vector<Transaction> transactions;
    bool isLoanTaker;
    double loanAmount;
    int monthsPaid;
    int totalMonths;
public:
    Account(const string& n, int number, const string& type, double initialBalance)
        : name(n), accountNumber(number), accountType(type), balance(initialBalance),
          isLoanTaker(false), loanAmount(0), monthsPaid(0), totalMonths(12) {}
    void deposit(double amount);
    bool withdraw(double amount);
    void addTransaction(const Transaction& transaction);
    void displayInfo();
    void applyLoan(double amount);
    void payLoan();
    int getAccountNumber();
    vector<Transaction>& getTransactions() {
        return transactions;
    }
    void makeLoanPayment();
    double getRemainingLoan();
    bool getIsLoanTaker();
    string getName();
    int getMonthsPaid();
    int getTotalMonths();
};

class Bank {
private:
    vector<Account> accounts;
public:
    void addAccount(const string& name, int number, const string& type, double initialBalance);
    Account* findAccount(int accountNumber);
    void transfer(Account& fromAccount, Account& toAccount, double amount);
    void displayAllAccounts();
    void displayAccountDetails(int accountNumber);
    void displayLoanTakers();
};

void Account::deposit(double amount) {
    balance += amount;
    Transaction transaction(transactions.size() + 1, "Deposit", amount);
    transactions.push_back(transaction);
    cout << "Deposit successful." << endl;
}
bool Account::withdraw(double amount) {
    if (balance >= amount) {
        balance -= amount;
        Transaction transaction(transactions.size() + 1, "Withdrawal", amount);
        transactions.push_back(transaction);
        cout << "Withdrawal successful." << endl;
        return true;
    }
    cout << "Insufficient balance." << endl;
    return false;
}
void Account::addTransaction(const Transaction& transaction) {
    transactions.push_back(transaction);
}
void Account::displayInfo() {
    cout << "Account Holder: " << name << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Account Type: " << accountType << endl;
    cout << "Balance: " << balance << endl;
}
void Account::applyLoan(double amount) {
    if (!isLoanTaker && balance >= amount) {
        balance -= amount;
        isLoanTaker = true;
        loanAmount = amount;
        Transaction transaction(transactions.size() + 1, "Loan", amount);
        transactions.push_back(transaction);
        cout << "Loan approved. Loan amount: " << amount << endl;
        cout << "Loan will be paid in 12 months with 5% interest each month." << endl;
    } else {
        cout << "Cannot apply for a loan." << endl;
    }
}
void Account::payLoan() {
    double monthlyPayment = loanAmount * 0.05;
    if (monthsPaid < totalMonths) {
        if (balance >= monthlyPayment) {
            balance -= monthlyPayment;
            Transaction transaction(transactions.size() + 1, "Loan Payment", monthlyPayment);
            transactions.push_back(transaction);
            cout << "Loan payment successful. Remaining balance: " << balance << endl;
            monthsPaid++;
}
        else
            {
            cout << "Insufficient balance for loan payment." << endl;
}
}   else
{
        cout << "Loan paid off." << endl;
}
}
int Account::getAccountNumber() {
    return accountNumber;
}
void Account::makeLoanPayment() {
    if (isLoanTaker) {
        double monthlyPayment = loanAmount * 0.05;
        if (monthsPaid < totalMonths) {
            if (balance >= monthlyPayment) {
                balance -= monthlyPayment;
                monthsPaid++;
}
}
}
}
double Account::getRemainingLoan() {
    double remainingAmount = loanAmount - (loanAmount * 0.05 * monthsPaid);
    return remainingAmount > 0 ? remainingAmount : 0;
}

bool Account::getIsLoanTaker() {
    return isLoanTaker;
}

string Account::getName() {
    return name;
}

int Account::getMonthsPaid() {
    return monthsPaid;
}

int Account::getTotalMonths() {
    return totalMonths;
}

void Bank::addAccount(const string& name, int number, const string& type, double initialBalance) {
    Account account(name, number, type, initialBalance);
    accounts.push_back(account);
    cout << "Account created successfully." << endl;
}

Account* Bank::findAccount(int accountNumber) {
    for (Account& account : accounts) {
        if (account.getAccountNumber() == accountNumber) {
            return &account;
        }
    }
    return nullptr;
}

void Bank::transfer(Account& fromAccount, Account& toAccount, double amount) {
    if (fromAccount.withdraw(amount)) {
        toAccount.deposit(amount);
        Transaction transaction(fromAccount.getTransactions().size() + 1, "Transfer", amount);
        fromAccount.addTransaction(transaction);
        transaction = Transaction(toAccount.getTransactions().size() + 1, "Transfer", amount);
        toAccount.addTransaction(transaction);
        cout << "Transfer successful." << endl;
    } else {
        cout << "Transfer failed." << endl;
    }
}

void Bank::displayAllAccounts() {
    cout << "---- Account List ----" << endl;
    for (Account& account : accounts) {
        account.displayInfo();
        cout << "----------------------" << endl;
    }
}

void Bank::displayAccountDetails(int accountNumber) {
    Account* account = findAccount(accountNumber);
    if (account) {
        cout << "---- Account Details ----" << endl;
        account->displayInfo();
        cout << "Account Number: " << account->getAccountNumber() << endl;
        if (account->getIsLoanTaker()) {
            cout << "Loan Taken: " << account->getRemainingLoan() << endl;
            cout << "Months Paid: " << account->getMonthsPaid() << "/" << account->getTotalMonths() << endl;
        }
        cout << "-------------------------" << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

void Bank::displayLoanTakers() {
    bool loanTakerFound = false;
    cout << "---- Loan Takers ----" << endl;
    for (Account& account : accounts) {
        if (account.getIsLoanTaker()) {
            cout << "Name: " << account.getName() << endl;
            cout << "Loan Taken: " << account.getRemainingLoan() << endl;
            cout << "Months Paid: " << account.getMonthsPaid() << "/" << account.getTotalMonths() << endl;
            cout << "----------------------" << endl;
            loanTakerFound = true;
        }
    }
    if (!loanTakerFound) {
        cout << "Nobody has taken a loan yet. You are welcome to take a loan from us." << endl;
    }
}

class BankManagementSystem {
private:
    Bank bank;

public:
    void run();
};

void BankManagementSystem::run() {
    int choice;
    do {
        cout << "\n---- Agrani Bank Limited ----" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer" << endl;
        cout << "5. Apply Loan" << endl;
        cout << "6. Pay Loan" << endl;
        cout << "7. Display All Accounts" << endl;
        cout << "8. Display Account Details" << endl;
        cout << "9. Display Loan Takers and Loan Status" << endl;
        cout << "10. Make Loan Payment" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, type;
                int number;
                double initialBalance;
                cout << "Enter account holder's name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter account number: ";
                cin >> number;
                cout << "Enter account type (e.g., Savings, Checking): ";
                cin.ignore();
                getline(cin, type);
                cout << "Enter initial balance: ";
                cin >> initialBalance;
                bank.addAccount(name, number, type, initialBalance);
                break;
            }
            case 2: {
                int accountNumber;
                double amount;
                cout << "Enter account number: ";
                cin >> accountNumber;
                Account* account = bank.findAccount(accountNumber);
                if (account) {
                    cout << "Enter deposit amount: ";
                    cin >> amount;
                    account->deposit(amount);
                } else {
                    cout << "Account not found." << endl;
                }
                break;
            }
            case 3: {
                int accountNumber;
                double amount;
                cout << "Enter account number: ";
                cin >> accountNumber;
                Account* account = bank.findAccount(accountNumber);
                if (account) {
                    cout << "Enter withdrawal amount: ";
                    cin >> amount;
                    account->withdraw(amount);
                } else {
                    cout << "Account not found." << endl;
                }
                break;
            }
            case 4: {
                int fromAccountNumber, toAccountNumber;
                double amount;
                cout << "Enter source account number: ";
                cin >> fromAccountNumber;
                Account* fromAccount = bank.findAccount(fromAccountNumber);
                cout << "Enter target account number: ";
                cin >> toAccountNumber;
                Account* toAccount = bank.findAccount(toAccountNumber);
                if (fromAccount && toAccount) {
                    cout << "Enter transfer amount: ";
                    cin >> amount;
                    bank.transfer(*fromAccount, *toAccount, amount);
                } else {
                    cout << "One or both accounts not found." << endl;
                }
                break;
            }
            case 5: {
                int accountNumber;
                double loanAmount;
                cout << "Enter account number: ";
                cin >> accountNumber;
                Account* account = bank.findAccount(accountNumber);
                if (account) {
                    cout << "Enter loan amount: ";
                    cin >> loanAmount;
                    account->applyLoan(loanAmount);
                } else {
                    cout << "Account not found." << endl;
                }
                break;
            }
            case 6: {
                int accountNumber;
                cout << "Enter account number: ";
                cin >> accountNumber;
                Account* account = bank.findAccount(accountNumber);
                if (account) {
                    account->payLoan();
                } else {
                    cout << "Account not found." << endl;
                }
                break;
            }
            case 7:
                bank.displayAllAccounts();
                break;
            case 8: {
                int accountNumber;
                cout << "Enter account number: ";
                cin >> accountNumber;
                bank.displayAccountDetails(accountNumber);
                break;
            }
            case 9:
                bank.displayLoanTakers();
                break;
            case 10: {
                int accountNumber;
                cout << "Enter account number: ";
                cin >> accountNumber;
                Account* account = bank.findAccount(accountNumber);
                if (account && account->getIsLoanTaker()) {
                    account->makeLoanPayment();
                    cout << "Loan payment successful." << endl;
                } else {
                    cout << "Account not found or is not a loan taker." << endl;
                }
                break;
            }
            case 11:
                cout << "Thanks for being with us!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

    } while (choice != 11);
}

int main() {
    BankManagementSystem system;
    system.run();

    return 0;
}
