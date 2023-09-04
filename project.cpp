#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Customer; // Forward declaration

class BankAccount {
private:
    int accountNumber;
    double balance;
    static int nextAccountNumber;

public:
    BankAccount(double initialBalance = 0.0);
    void deposit(double amount);
    void withdraw(double amount);
    double getBalance() ;
    int getAccountNumber() ;
    friend class BankSystem;
    friend class Transaction;
};

class Transaction {
private:
    int transactionId;
    double amount;
    static int nextTransactionId;

public:
    Transaction(double amount);
    int getTransactionId() ;
    double getAmount() ;
    friend class BankSystem;
};

class Customer {
private:
    string name;
    int customerId;
    vector<BankAccount> accounts;

public:
    Customer(string& name, int customerId);
    void addAccount(BankAccount account);
    void listAccounts() ;
    friend class BankSystem;
};

class BankSystem {
private:
    vector<Customer> customers;
    vector<Transaction> transactions;
    int nextCustomerId;
    int findCustomerIndex(int customerId);

public:
    BankSystem(){nextCustomerId=1;}
    void addCustomer(string& name);
    void addAccount(int customerId, double initialBalance = 0.0);
    void listCustomers() ;
    void listCustomerAccounts(int customerId) ;
    void performTransaction(int fromAccountId, int toAccountId, double amount);
    void listTransactions() ;
};

int BankAccount::nextAccountNumber = 1;
int Transaction::nextTransactionId = 1;

// BankAccount class member functions
BankAccount::BankAccount(double initialBalance){
    balance=initialBalance;
    accountNumber = nextAccountNumber++;
}

void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposit of $" << amount << " successful. New balance: $" << balance << endl;
    } else {
        cout << "Invalid deposit amount." << endl;
    }
}

void BankAccount::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        cout << "Withdrawal of $" << amount << " successful. New balance: $" << balance << endl;
    } else {
        cout << "Insufficient balance or invalid withdrawal amount." << endl;
    }
}

double BankAccount::getBalance()  {
    return balance;
}

int BankAccount::getAccountNumber()  {
    return accountNumber;
}

// Transaction class member functions
Transaction::Transaction(double amount) {
    this ->amount=amount;
    transactionId = nextTransactionId++;
}

int Transaction::getTransactionId()  {
    return transactionId;
}

double Transaction::getAmount()  {
    return amount;
}

// Customer class member functions
Customer::Customer( string& name, int customerId) : name(name), customerId(customerId) {}

void Customer::addAccount(BankAccount account) {
    accounts.push_back(account);
}

void Customer::listAccounts()  {
    cout << "Accounts for customer " << name << " (ID: " << customerId << "):" << endl;
    for ( BankAccount& account : accounts) {
        cout << "Account " << account.getAccountNumber() << ", Balance: $" << account.getBalance() << endl;
    }
}

// BankSystem class member functions
int BankSystem::findCustomerIndex(int customerId)  {
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].customerId == customerId) {
            return i;
        }
    }
    return -1; // Customer not found
}

void BankSystem::addCustomer(string& name) {
    customers.emplace_back(name, nextCustomerId++);
}

void BankSystem::addAccount(int customerId, double initialBalance) {
    int customerIndex = findCustomerIndex(customerId);
    if (customerIndex != -1) {
        customers[customerIndex].addAccount(BankAccount(initialBalance));
        cout << "Account added for customer with ID: " << customerId << endl;
    } else {
        cout << "Customer with ID " << customerId << " not found." << endl;
    }
}

void BankSystem::listCustomers()  {
    cout << "Customers list:" << endl;
    for ( Customer& customer : customers) {
        cout << "Customer ID: " << customer.customerId << ", Name: " << customer.name << endl;
    }
}

void BankSystem::listCustomerAccounts(int customerId)  {
    int customerIndex = findCustomerIndex(customerId);
    if (customerIndex != -1) {
        customers[customerIndex].listAccounts();
    } else {
        cout << "Customer with ID " << customerId << " not found." << endl;
    }
}

void BankSystem::performTransaction(int fromAccountId, int toAccountId, double amount) {
    int fromCustomerIndex = -1;
    int toCustomerIndex = -1;
    int fromAccountIndex = -1;
    int toAccountIndex = -1;

    // Find the indexes of the accounts and customers
    for (int i = 0; i < customers.size(); ++i) {
        for (int j = 0; j < customers[i].accounts.size(); ++j) {
            if (customers[i].accounts[j].getAccountNumber() == fromAccountId) {
                fromCustomerIndex = i;
                fromAccountIndex = j;
            }
            if (customers[i].accounts[j].getAccountNumber() == toAccountId) {
                toCustomerIndex = i;
                toAccountIndex = j;
            }
        }
    }

    if (fromCustomerIndex == -1 || toCustomerIndex == -1 || fromAccountIndex == -1 || toAccountIndex == -1) {
        cout << "Account(s) not found." << endl;
        return;
    }

    // Perform the transaction
    if (amount > 0 && customers[fromCustomerIndex].accounts[fromAccountIndex].getBalance() >= amount) {
        customers[fromCustomerIndex].accounts[fromAccountIndex].withdraw(amount);
        customers[toCustomerIndex].accounts[toAccountIndex].deposit(amount);
        transactions.emplace_back(amount);
        cout << "Transaction successful." << endl;
    } else {
        cout << "Invalid transaction or insufficient balance." << endl;
    }
}

void BankSystem::listTransactions()  {
    cout << "Transactions list:" << endl;
    for ( Transaction& transaction : transactions) {
        cout << "Transaction ID: " << transaction.getTransactionId() << ", Amount: $" << transaction.getAmount() << endl;
    }
}

int main() {
    BankSystem bankSystem;
    int choice;
    int customerId;

    do {
        cout << "----- Bank Account Management System -----" << endl;
        cout << "1. Add Customer\n2. Add Account\n3. List Customers\n4. List Customer Accounts"
                  << "\n5. Perform Transaction\n6. List Transactions\n7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, name);
                bankSystem.addCustomer(name);
                break;
            }
            case 2: {
                double initialBalance;
                cout << "Enter customer ID: ";
                cin >> customerId;
                cout << "Enter initial account balance: ";
                cin >> initialBalance;
                bankSystem.addAccount(customerId, initialBalance);
                break;
            }
            case 3:
                bankSystem.listCustomers();
                break;
            case 4: {
                cout << "Enter customer ID: ";
                cin >> customerId;
                bankSystem.listCustomerAccounts(customerId);
                break;
            }
            case 5: {
                int fromAccountId, toAccountId;
                double amount;
                cout << "Enter source account ID: ";
                cin >> fromAccountId;
                cout << "Enter destination account ID: ";
                cin >> toAccountId;
                cout << "Enter transaction amount: ";
                cin >> amount;
                bankSystem.performTransaction(fromAccountId, toAccountId, amount);
                break;
            }
            case 6:
                bankSystem.listTransactions();
                break;
            case 7:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << "-----------------------------------------" << endl;

    } while (choice != 7);

    return 0;
}
