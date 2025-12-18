#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Account{
    string Name, ID, IFSC, status, password;
    long int Balance;
    struct dob {
        int year, month, day;
    };
    vector<Transaction> transactionHistory;
    //TransactionsHistory <vector> - all past transactions
public:
    //friend class Bank;
    bool passwordCheck(string pwd){
        if(this->password == pwd)
            return true;
        else
            return false;
    }
};

class Bank{
    unordered_map<string, Account> accounts;
public:
    string IFSC, Name;
    Bank(string IFSC, string Name):IFSC(IFSC), Name(Name){}

    //*static features*
    bool deposit(string toID){
        Account toA;
        try{
            toA = accounts[toID];
        }
        catch(exception& e){
            return false;
        }
        int amm;
        cout<<"Enter Amount:";
        cin>>amm;
        toA.Balance+=amm;
        return true;
    }

    bool withdraw(string ID){
        Account a = accounts[ID];
        try{
            a = accounts[ID];
        }
        catch(exception& e){
            return false;
        }
        int amm;
        string pwd;
        cout<<"Enter Password";
        cin>>pwd;
        if(a.passwordCheck(pwd)){
            cout<<"Enter Amount";
            cin>>amm;
            if(a.Balance>=amm)
                a.Balance -= amm;
            else
                return false;
        }
        return true;
    }
    //check balance
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
    //Transaction Id
    //Account Id
    //Ammount
    //Trans. Type
    //timestamp

    //*static features*
    //operator overloading for priority queue comparision
};

int main(){
    
    return 0;
}