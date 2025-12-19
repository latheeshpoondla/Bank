#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

class Account{
    string Name, ID, IFSC, status, password;
    long int Balance;
    struct dob {
        int year, month, day;
    }dob;
    vector<Transaction> transactionHistory;
    //TransactionsHistory <vector> - all past transactions
public:

    Account(string IFSC):IFSC(IFSC){
        cout<<"Enter Full Name: ";
        cin>>this->Name;
        cout<<"Set Password: ";
        cin>>this->password;
        string DOB;
        cout<<"Enter Date of Birth(DD/MM/YYYY): ";
        cin>>DOB;
        char slash;
        stringstream ss(DOB);
        ss>>this->dob.day>>slash>>this->dob.month>>slash>>this->dob.year;
        this->status = "Open Savings";
        this->ID = generateID();
    }

    string generateID(){
        fstream file("/data/LastAccountID.dat", ios::in | ios::out | ios::binary);
        long int ID=0;
        file.read(reinterpret_cast<char*>(&ID), sizeof(ID));
        
        if(ID)
            ID++;
        else
            ID = 1000000000000000;
        file.write(reinterpret_cast<const char*>(&ID), sizeof(ID));
        return to_string(ID);
    }
    
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