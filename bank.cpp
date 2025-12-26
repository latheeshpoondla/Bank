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
    Account(){}

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
        this->status = "Open";
        this->ID = generateID();
    }

    string generateID(){
        fstream file("/data/LastAccountID.dat", ios::in | ios::out | ios::binary);
        long int ID=0;
        file.read(reinterpret_cast<char*>(&ID), sizeof(ID));
        
        if(ID)
            ID++;
        else
            ID = 10000000000;
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
    Bank(){
        cerr<<"Invalid Bank Details..!";
        exit(1);
    }
    Bank(string IFSC, string Name):IFSC(IFSC), Name(Name){}

    //*static features*
    bool deposit(string toID){
        Account toA;
        if(!getAccountByID(toA, toID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(toA.status != "Open"){
            cout<<"Account Closed or Invalid"<<endl;
            return false;
        }
        long int amm;
        cout<<"Enter Amount:";
        cin>>amm;
        toA.Balance+=amm;
        Transaction t(toID, "Deposit", amm);
        toA.transactionHistory.push_back(t);
        return true;
    }

    bool withdraw(string ID){
        Account a;
        if(!getAccountByID(a, ID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(a.status != "Open"){
            cout<<"Account Closed or Invalid"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password";
        cin>>pwd;
        long int amm;
        if(a.passwordCheck(pwd)){
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
        Transaction t(ID, "Withdraw", amm);
        a.transactionHistory.push_back(t);
        return true;
    }
    
    bool checkBalance(string ID){
        Account a;
        if(!getAccountByID(a, ID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(a.status != "Open"){
            cout<<"Account Closed or Invalid"<<endl;
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
        if(toA.status != "Open"){
            cout<<"Receiver-Account Closed or Invalid"<<endl;
            return false;
        }
        if(fromA.status != "Open"){
            cout<<"Sender-Account Closed or Invalid"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password: ";
        cin>>pwd;
        long int amm;
        if(fromA.passwordCheck(pwd)){
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
        Transaction t(fromID, toID, "Transfer", amm);
        toA.transactionHistory.push_back(t);
        return true;
    }
    //store all Accounts in files
    //saveData() - (updates accounts.dat with updated map<>)
    void saveData(){

    }
    //retrieve accounts from file
    //loadData() - (runs the startup for Bank - accounts.dat to map<>) 
    //              + refill priority queue using transaction***.log file new trans. objects.
    void loadData(){

    }

    //store all transactionslog for all accounts separate(text based, .log file) 
    // Format: AccountID | Type | Amount | Time..etc
    void transactionsLog(){

    }
    //getMonthlyStatement(pdf/csv) (e.g. Statement_Oct2023.csv)

    
    bool createAccount(){
        Account a(IFSC);
        accounts[a.ID] = a;
        return true;
    }
    //close account
    bool closeAccount(){

    }
    //modify account
    bool modifyAccount(){

    }

    bool getAccountByID(Account&a, string ID){
        try{
            a = accounts[ID];
        }
        catch (exception& e){
            return false;
        }

        return true;
    }

    //priorityQueue + showTopTransactions

    ~Bank(){
        saveData();
    }
};

class Transaction{
    string ID, accountID, type, toID, fromID;
    long int Amount;
    struct timestamp{
        int day, month, year, hours, minutes, seconds;
    };

public:
    Transaction(){
        cerr<<"Invalid Transaction..!"<<endl;
    }

    Transaction(string accountID, string type, long int amm){
        this->ID = this->generateID();
        this->accountID = accountID;
        this->type = type;
        this->Amount = amm;
    }

    Transaction(string fromID, string toID, string type, long int amm){
        this->ID = this->generateID();
        this->fromID = fromID;
        this->toID = toID;
        this->type = type;
        this->Amount = amm;
    }

    string generateID(){
        long int id=0;
        fstream file("/data/LastTransactionID.dat", ios::in|ios::out|ios::binary);
        file.read(reinterpret_cast<char*>(&id), sizeof(id));
        if(id)
            id++;
        else
        id = 100000000000;
        file.write(reinterpret_cast<char*>(&id), sizeof(id));
        return to_string(id);
    }

    friend bool operator> (Transaction& t1, Transaction& t2){
        return t1.Amount > t2.Amount;
    }
    friend bool operator< (Transaction& t1, Transaction& t2){
        return t1.Amount < t2.Amount;
    }
};

int main(){
    Bank SBI("SBIN130045", "State Bank of India");
    return 0;
}