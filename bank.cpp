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
    friend class Bank;

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
    unordered_map<string, Account> accounts;
public:
    string IFSC, Name;
    Bank(string IFSC, string Name):IFSC(IFSC), Name(Name){}

    //*static features*
    bool deposit(string toID){
        Account toA;
        if(!getAccountByID(toA, toID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(toA.status == "Closed"){
            cout<<"Account Closed"<<endl;
            return false;
        }
        int amm;
        cout<<"Enter Amount:";
        cin>>amm;
        toA.Balance+=amm;
        return true;
    }

    bool withdraw(string ID){
        Account a;
        if(!getAccountByID(a, ID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(a.status == "Closed"){
            cout<<"Account Closed"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password";
        cin>>pwd;
        if(a.passwordCheck(pwd)){
            int amm;
            cout<<"Enter Amount";
            cin>>amm;
            if(a.Balance>=amm)
                a.Balance -= amm;
            else{
                cout<<"Insuffient Balance..!"<<endl;
                return false;
            }
        }
        else{
            cout<<"Incorrect Password..!"<<endl;
            return false;
        }
        return true;
    }
    
    bool checkBalance(string ID){
        Account a;
        if(!getAccountByID(a, ID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(a.status == "Closed"){
            cout<<"Account Closed"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password: ";
        cin>>pwd;
        if(a.passwordCheck(pwd))
            cout<<"Balance: "<<a.Balance<<endl;
        else{
            cout<<"Incorrect Password..!"<<endl;
            return false;
        }
        return true;
    }
    
    bool transfer(string toID, string fromID){
        Account toA, fromA;
        if(!getAccountByID(toA, toID)){
            cout<<"Receiver Account Not Found...!"<<endl;
            return false;
        }
        if(!getAccountByID(fromA, fromID)){
            cout<<"Sender Account Not Found...!"<<endl;
            return false;
        }
        if(toA.status == "Closed"){
            cout<<"Receiver-Account Closed"<<endl;
            return false;
        }
        if(fromA.status == "Closed"){
            cout<<"Sender-Account Closed"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password: ";
        cin>>pwd;
        if(fromA.passwordCheck(pwd)){
            int amm;
            cout<<"Enter Amount: ";
            cin>>amm;
            if(fromA.Balance>=amm){
                fromA.Balance -= amm;
                toA.Balance += amm;
            }
            else{
                cout<<"Insuffient Balance..!"<<endl;
                return false;
            }
        }
        else{
            cout<<"Incorrect Password..!"<<endl;
            return false;
        }
        return true;
    }
    //store all Accounts in files

    //store all transactionslog for all accounts separate(text based, .log file) 
    // Format: AccountID | Type | Amount | Time..etc

    //retrieve accounts from file
    //getMonthlyStatement(pdf/csv) (e.g. Statement_Oct2023.csv)
    
    bool createAccount(){
        Account a(IFSC);
        accounts[a.ID] = a;
    }
    //close account
    //modify account
    bool getAccountByID(Account& a, string ID){
        try{
            a = accounts[ID];
        }
        catch (exception& e){
            return false;
        }
        return true;
    }
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