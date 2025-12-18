#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Account{
    string Name, ID, IFSC, status, username, password;
    long int Balance;
    struct dob {
        int year, month, day;
    };
    vector<Transaction> transactionHistory;
    //TransactionsHistory <vector> - all past transactions
public:

    bool passwordCheck(string pwd){
        if(this->password == pwd)
            return true;
        else
            return false;
    }
};

class Bank{
    //Constructor(IFSC) - loadData()
    unordered_map<int, Account> accounts;
    //IFSC - (string)
    //Bank Name

    //*static features*
    //deposit
    //withdraw
    //store all Accounts in files

    //store all transactionslog for all accounts separate(text based, .log file) 
    // Format: AccountID | Type | Amount | Time..etc

    //retrieve accounts from file
    //getMonthlyStatement(pdf/csv) (e.g. Statement_Oct2023.csv)
    //create account
    //close account
    //modify account
    //getAccountByID
    //searchByName
    //priorityQueue
    //showTopTransactions

    //loadData() - (runs the startup for Bank - accounts.dat to map<>) 
    //              + refill priority queue using transaction***.log file new trans. objects.

    //saveData() - (updates accounts.dat with updated map<>)
};

class Transaction{
    string ID, accountID, type;
    long int Ammount;
    struct timestamp{
        int day, month, year, hours, minutes, seconds;
    };

public:
    
    friend bool operator> (Transaction& t1, Transaction& t2){
        return t1.Ammount > t2.Ammount;
    }
    friend bool operator< (Transaction& t1, Transaction& t2){
        return t1.Ammount < t2.Ammount;
    }
};

int main(){
    
    return 0;
}