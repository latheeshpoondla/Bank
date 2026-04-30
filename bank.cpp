#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <limits>
#include <string>
#include <filesystem>

using namespace std;
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
        const string path = "data/LastTransactionID.dat";
        long long id=0LL;
        ifstream in(path, ios::binary);

        if (!in) {
            // File doesn't exist → create and initialize
            id = 100000000000LL;

            ofstream out(path, ios::binary);
            out.write(reinterpret_cast<char*>(&id), sizeof(id));
            out.close();

            return to_string(id);
        }
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        in.close();
        // If file empty or corrupted
        if (id == 0) {
            id = 100000000000LL;
        } else {
            id++;
        }

        // Write updated ID back
        ofstream out(path, ios::binary | ios::trunc);
        out.write(reinterpret_cast<char*>(&id), sizeof(id));
        out.close();
        return to_string(id);
    }

    friend bool operator> (Transaction& t1, Transaction& t2){
        return t1.Amount > t2.Amount;
    }
    friend bool operator< (Transaction& t1, Transaction& t2){
        return t1.Amount < t2.Amount;
    }
};

class Account{
    string Name, ID, IFSC, status, password;
    long int Balance = 0;
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
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer
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

string generateID() {
    const string path = "data/lastAccountID.dat";

    // Create directory if it doesn't exist
    std::filesystem::create_directories("data");

    long long ID = 0;

    // Try to open file for reading
    ifstream in(path, ios::binary);

    if (!in) {
        // File doesn't exist → create and initialize
        ID = 10000000000LL;

        ofstream out(path, ios::binary);
        out.write(reinterpret_cast<char*>(&ID), sizeof(ID));
        out.close();

        return to_string(ID);
    }

    // File exists → read current ID
    in.read(reinterpret_cast<char*>(&ID), sizeof(ID));
    in.close();

    // If file empty or corrupted
    if (ID == 0) {
        ID = 10000000000LL;
    } else {
        ID++;
    }

    // Write updated ID back
    ofstream out(path, ios::binary | ios::trunc);
    out.write(reinterpret_cast<char*>(&ID), sizeof(ID));
    out.close();

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
    map<string, Account> accounts;
    map<string, bool> update_ids;
public:
    string IFSC, Name;
    Bank(){
        cerr<<"Invalid Bank Details..!";
        exit(1);
    }
    Bank(string IFSC, string Name):IFSC(IFSC), Name(Name){}

    //*static features*
    bool deposit(string toID){
        Account *toA;
        if(!getAccountByID(toA, toID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(toA->status != "Open"){
            cout<<"Account Closed or Invalid"<<endl;
            return false;
        }
        long int amm;
        cout<<"Enter Amount:";
        cin>>amm;
        toA->Balance+=amm;
        Transaction t(toID, "Deposit", amm);
        toA->transactionHistory.push_back(t);
        this->update_ids[toA->ID] = true;
        return true;
    }

    bool withdraw(string ID){
        Account *a;
        if(!getAccountByID(a, ID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(a->status != "Open"){
            cout<<"Account Closed or Invalid"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password";
        cin>>pwd;
        long int amm=0;
        if(a->passwordCheck(pwd)){
            cout<<"Enter Amount";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cin>>amm;
            if(a->Balance>=amm)
                a->Balance -= amm;
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
        a->transactionHistory.push_back(t);
        this->update_ids[a->ID] = true;
        return true;
    }
    
    bool checkBalance(string ID){
        Account *a;
        if(!getAccountByID(a, ID)){
            cout<<"Account Not Found...!"<<endl;
            return false;
        }
        if(a->status != "Open"){
            cout<<"Account Closed or Invalid"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password: ";
        cin>>pwd;
        if(a->passwordCheck(pwd))
            cout<<"Balance: "<<a->Balance<<endl;
        else{
            cout<<"Incorrect Password..!"<<endl;
            return false;
        }
        return true;
    }
    
    bool transfer(string toID, string fromID){
        Account *toA, *fromA;
        if(!getAccountByID(toA, toID)){
            cout<<"Receiver Account Not Found...!"<<endl;
            return false;
        }
        if(!getAccountByID(fromA, fromID)){
            cout<<"Sender Account Not Found...!"<<endl;
            return false;
        }
        if(toA->status != "Open"){
            cout<<"Receiver-Account Closed or Invalid"<<endl;
            return false;
        }
        if(fromA->status != "Open"){
            cout<<"Sender-Account Closed or Invalid"<<endl;
            return false;
        }
        string pwd;
        cout<<"Enter Password: ";
        cin>>pwd;
        long int amm;
        if(fromA->passwordCheck(pwd)){
            cout<<"Enter Amount: ";
            cin>>amm;
            if(fromA->Balance>=amm){
                fromA->Balance -= amm;
                toA->Balance += amm;
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
        toA->transactionHistory.push_back(t);
        this->update_ids[toA->ID] = true;
        this->update_ids[fromA->ID] = true;
        return true;
    }
    //store all Accounts in files
    //saveData() - (updates accounts.dat with updated map<>)

void saveData() {
    std::filesystem::create_directories("data");

    // Open in append mode (only write updates)
    ofstream file("data/accounts.txt", ios::app);

    if (!file) {
        cout << "Error opening file\n";
        return;
    }

    for (const auto &p : update_ids) {
        const string &id = p.first;
        const Account &a = accounts.at(id);

        file << "ACCOUNT_BEGIN\n";

        file << a.ID << "\n";
        file << a.Name << "\n";
        file << a.IFSC << "\n";
        file << a.Balance << "\n";
        file << a.status << "\n";
        file << a.password << "\n";

        file << a.dob.day << " "
             << a.dob.month << " "
             << a.dob.year << "\n";

        file << "ACCOUNT_END\n";
    }

    file.close();

    // Clear updates after saving
    update_ids.clear();
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

    
    string createAccount(){
        Account a(IFSC);
        accounts[a.ID] = a;
        this->update_ids[a.ID] = true;
        return a.ID;
    }
    // //close account
    // bool closeAccount(){

    // }
    // //modify account
    // bool modifyAccount(){

    // }

    bool getAccountByID(Account*& a, string ID) {
        if (accounts.find(ID) == accounts.end())
            return false;

        a = &accounts[ID];
        return true;
    }

    //priorityQueue + showTopTransactions

    // ~Bank(){
    //     saveData();
    // }
};

int main(){
    Bank SBI("SBIN130045", "State Bank of India");
    int r;
    while(r!=6){
        cout<<"Select Option:\n"<<endl;
        cout<<"1. Create new account"<<endl;
        cout<<"2. Deposit"<<endl;
        cout<<"3. Withdraw"<<endl;
        cout<<"4. Check Balance"<<endl;
        cout<<"5. Transfer"<<endl;
        cout<<"6. Quit\n"<<endl;
        cin>>r;
        string a;
        if(r ==1){
            a = SBI.createAccount();
            cout<<"Your Account ID: "<<a<<endl;
        }
        if(r == 2){
            cout<<"Enter Account ID";
            cin>>a;
            SBI.deposit(a);
        }
        if(r==3){
            cout<<"Enter Account ID"; 
            cin>>a;
            SBI.withdraw(a);
        }
        if(r==4){
            cout<<"Enter Account ID"; 
            cin>>a;
            SBI.checkBalance(a);
        }
        if(r==5){
            cout<<"Enter toID:"<<endl;
            cin>>a;
            string b;
            cout<<"Enter fromID"<<endl;
            cin>>b;
            SBI.transfer(a, b);
        }
    }
    return 0;
}