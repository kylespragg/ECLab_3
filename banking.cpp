// EC Lab 3
#include<iostream>
#include <iomanip>

using namespace std;

class Account {

    protected:

        double balance;
        string accountHolder;
        string accountNumber;

    public:

        Account(const string& accountNumber, const string& accountHolder, double balance = 0.0) : accountNumber(accountNumber), balance(balance), accountHolder(accountHolder) {}

        virtual void deposit(double amount){
            
            balance += amount;
            cout << "Deposited $" << amount << "." << endl;
 
        }

        virtual void withdraw(double amount){

            if (balance >= amount){

                balance -= amount;

                cout << "Withdrawn $" << amount << "." << endl;
            }

            else{

                cout << "Error. Insufficient funds." << endl;
            }
        }

        virtual void displayDetails() const {

            cout << "Account Details for Savings Account (ID: " << accountNumber << "):" << endl;
            cout << "Holder: " << accountHolder << endl;
            cout << "Balance: $" << setprecision(2) << fixed << balance << endl;
        }
        double getBalance() const{

            return balance;

        }

        void setBalance(double newBalance){

            balance = newBalance;
        }

        virtual string getType() const {
            
            return "Generic";
        }
        Account& operator=(const Account& other) {
        if (this != &other) {
            balance = other.balance;
            accountHolder = other.accountHolder;
            accountNumber = other.accountNumber;
        }
        return *this;
    }


    friend ostream& operator <<(ostream& os, const Account& account);
};

ostream& operator<<(ostream& os, const Account& account){

    os << "Account Details for " << account.getType() << "Account (ID: " << account.accountNumber << ")" << endl;
    os << "Holder: " << account.accountHolder << endl;
    os << "Balance: $" << setprecision(2) << fixed << account.balance << endl;

    return os;
} 
class SavingsAccount : public Account {

    protected:

        double interestRate;

    public:
        
        SavingsAccount(const string& accountNumber, const string& accountHolder, double balance, double interestRate) : Account(accountNumber, accountHolder, balance), interestRate(interestRate) {}
        
        void displayDetails() const override {
            Account::displayDetails();
            cout << "Interest Rate:" << setprecision(2) << fixed << (interestRate*100) << "%" << endl;
        }

        double getBalance() const{

            return balance;
            
        }

        virtual string getType() const {
            return "Savings";
        }

        void withdraw(double amount) override{

            if (balance - amount >= 50){
                balance -= amount; 
                cout << "Withdrawn $" << setprecision(2) << fixed << amount << "." << endl;
            }

            else{

                cout << "Error. You cannot withdraw past the minimum balance." << endl;
            }
        }
        SavingsAccount& operator=(const SavingsAccount& other) {
            if (this != &other) {
                Account::operator=(other);  // Call the base class assignment operator
                interestRate = other.interestRate;
            }
            return *this;
}

};

class CurrentAccount : public Account {

    
    protected:
        
        double overdraftLimit;
        
    public:
        CurrentAccount(const string& accountNumber, const string& accountHolder, double balance, double overdraftLimit ) : Account(accountNumber, accountHolder, balance), overdraftLimit(overdraftLimit) {}
        
        void displayDetails() const override {
            Account::displayDetails();
            cout << "Overdraft Limit: $" << setprecision(2) << fixed << overdraftLimit << endl;
        }

        double getBalance() const{
            return balance;   
        }

        virtual string getType() const {

            return "Current";
        }

        void withdraw(double amount) override {

            if (balance >= amount) {
                balance -= amount;
                cout << "Withdrawn $" << setprecision(2) << fixed << amount << "." << endl;
            } else {
                double overdraftAmount = amount - balance;
                if (overdraftAmount <= overdraftLimit) {
                    balance = 0;
                    overdraftLimit -= overdraftAmount;
                    cout << "Withdrawn $" << setprecision(2) << fixed << amount << " (including overdraft)." << endl;
                } else {
                    cout << "Error. Withdrawal exceeds overdraft limit." << endl;
                }
            }
    }

        CurrentAccount& operator=(const CurrentAccount& other) {
            if (this != &other) {
                Account::operator=(other);  // Call the base class assignment operator
                overdraftLimit = other.overdraftLimit;
            }
            return *this;
}

        

};

CurrentAccount operator+(const CurrentAccount& current, const SavingsAccount& savings) {

    CurrentAccount result = current;
    result.setBalance(result.getBalance() + savings.getBalance());
    const_cast<SavingsAccount&>(savings).setBalance(0);
    return result;
}

SavingsAccount operator+(const SavingsAccount& savings, const CurrentAccount& current) {

    SavingsAccount result = savings;
    result.setBalance(result.getBalance() + current.getBalance());
    const_cast<CurrentAccount&>(current).setBalance(0);  // Assuming you want to set the current balance to 0
    return result;
}

int main() {

    SavingsAccount savings("S123", "John Doe", double(1000), double(0.02));
    CurrentAccount current("C456", "Jane Doe", double(2000), double(500));

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);
    
    savings.displayDetails();
    current.displayDetails();

    // Transfer 300 from savings to current
    current = current + savings;

    savings.displayDetails();
    current.displayDetails();

    return 0; 
}
