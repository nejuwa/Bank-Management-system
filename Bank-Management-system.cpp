#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <filesystem> //file
#include <cstdlib>//for clearing console
#include <thread>//for sleeping time
#include <chrono>//to set site bound

using namespace std;

//data base/filessystem in our case/ path
std::filesystem::path account_path = std::filesystem::path("database")/"accounts.txt";
std::filesystem::path accountNo_path = std::filesystem::path("database")/"account_numbers.txt";

//graphics-------------------------------------------------------------------------------------

//colors decorations
string END="\033[0m";
string RED="\033[31m";
string GREEN="\033[32m";
string YELLOW="\033[33m";
string BLUE="\033[34m";
string CYAN="\033[36m";

//clearing the console command
void clear_console() {
    #ifdef _WIN32
        system("cls"); // For Windows
    #else
        system("clear"); // For UNIX-like systems and mac os
    #endif
};

//delaying time for real system simulation
void delay_time(int second){
    chrono::seconds duration(second);
    this_thread::sleep_for(duration);
}
//navgating options
void navigation_option(){
    int choose;
    cout << RED+"\n[1]"+BLUE+" back to menu\t";
    cout << RED+"[2]"+BLUE+" exit\n";
    cout << "choose(1-2): ";
    cin >> choose;
    if(choose == 2){
        exit(0);
    }
}

//dashboard banner
void dash_board() {
    const int width = 50;//give value for banner width
    const string border(width, '*');
    const string title = "bank management system";
    const string copyright = "© 2024,Gemini";

    int padding = (width - 2 - title.length()) / 2;
    int padding2 = (width - 2 - copyright.length()) / 2;

    clear_console();//always clear console  before printing banner
    cout<<GREEN + border<<endl;//top border with '*'
    cout<<'*';

    cout << string(padding, ' ') <<YELLOW + title + END << string(padding, ' ');//dispalay title at center
    if ((width - 2 - title.length()) % 2 != 0 ) {
        cout << ' ';
    }
    cout << GREEN + '*' << endl;
    cout<<'*'<<' ';

    cout << string(padding2, ' ') <<CYAN + copyright + END<< string(padding2, ' ');//diaplay copyright at center
    if ((width - 2 - copyright.length()) % 2 != 0 ) {
        cout << ' ';
    }

    cout <<GREEN + '*' << endl;
    cout << border << endl; // Bottom border with '*'
};
//contributers as linked list
struct member{
    string name;
    string Id_num;
    int section;
    int group;
    member *next;
    //constructor
    member(string name,string id_no,int sec,int _group):name(name),Id_num(id_no),section(sec),group(_group),next(nullptr){};
};
// member* readfile(){
//     member* _member;
//     string _name,_id_no;
//     int _sec,group;

// }
//account class---------------------------------------------------------------------------------
class BankAccount {
public:
    long long accountNumber;
    std::string accountHolderName;
    double balance;

    //constructor
    BankAccount(int number, std::string name, double initialBalance) {
        accountNumber = number;
        accountHolderName = name;
        balance = initialBalance;
    }
};

//tree node class to store accounts ------------------------------------------------------------------
class Node {
public:
    BankAccount* account;
    Node* left;
    Node* right;

    Node(BankAccount* acc) {
        account = acc;
        left = NULL;
        right = NULL;
    }
};

//binary tree implimentaion---------------------------------------------------------------------
class BinaryTree {
public:
    Node* root;
    BinaryTree() {
        root = NULL;
    };

    void insertNode(BankAccount* account){
        Node* newNode = new Node(account);
        if (root == NULL){
            root = newNode;
        }
        else {
            Node* current = root;
            Node* parent;
            while (true) {
                parent = current;
                if (account->accountNumber < current->account->accountNumber) {
                    current = current->left;
                    if (current == NULL) {
                        parent->left = newNode;
                        return;
                    }
                }
                else {
                    current = current->right;
                    if (current == NULL) {
                        parent->right = newNode;
                        return;
                    }
                }
            }
        }
    };

    void inorderTraversal(Node* node, std::ofstream& file) {
        if (node == NULL) {
            return;
        }
        inorderTraversal(node->left, file);
        file <<std::setw(20)<<std::left<<node->account->accountNumber<<std::setw(30)<<std::left
        <<node->account->accountHolderName<<std::setw(30)<<std::left
        <<node->account->balance<<std::endl;
        inorderTraversal(node->right, file);
    }


    void inorderTraversalHead(std::ofstream& file) {
        file <<std::setw(20)<<std::left<<"Account Number" <<std::setw(30) <<std::left << "Name" <<std::setw(30) <<std::left<< "Balance" <<std::endl;
        inorderTraversal(root, file);
    }

    Node* searchNode(int accountNumber) {
        Node* current = root;
        while (current != NULL) {
            if (current->account->accountNumber == accountNumber) {
                return current;
            }
            if (accountNumber < current->account->accountNumber) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }
        return NULL;
    }

    void removeNode(float accountNumber) {
        root = removeNodeHelper(root, accountNumber);
    }

    Node* removeNodeHelper(Node* node, int accountNumber) {
        if (node == NULL) {
            return node;
        }

        if (accountNumber < node->account->accountNumber) {
            node->left = removeNodeHelper(node->left, accountNumber);
        }
        else if (accountNumber > node->account->accountNumber) {
            node->right = removeNodeHelper(node->right, accountNumber);
        }
        else {
            if (node->left == NULL) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == NULL) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = findMinNode(node->right);
            if (temp==NULL){
                temp= findMaxNode(node->left);
            }
            node->account = temp->account;
            delete temp;
        }
        return node;
    }

    Node* findMinNode(Node* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    Node* findMaxNode(Node* node){
        while(node->right!= NULL){
            node=node->right;
        }
        return node;
    }

};

//stack implimetation for stack------------------------------------------------------------------
struct Transaction {

    std::string type;
    double amount;
    std::string time;
    Transaction* next;
};

//linked list for stroring each account transaction
struct stack_node{
    long long acc_number;
    Transaction* transaction;
    stack_node* next;
};

std::string setDate(){
    time_t now=time(0);
    tm* current_time = localtime(&now);
    int year =  1900 + current_time->tm_year;
    int month = 1 + current_time->tm_mon;
    int day = current_time->tm_mday;
    int hour = current_time->tm_hour;
    int minute = current_time->tm_min;
    int second = current_time->tm_sec;

    std::string Date = "Date:  "+std::to_string(year)+"/"+std::to_string(month)+"/"+std::to_string(day);
    std::string Time = "Time:  "+std::to_string(hour)+":"+std::to_string(minute)+":"+std::to_string(second);

    return Date+"   "+Time;
}

stack_node* start = nullptr; // Initialize to nullptr

// Function to create a new transaction and push it onto the stack.
Transaction* record_transaction(const std::string& type, double amount) {
    Transaction* newTransaction = new Transaction{type, amount,setDate(), nullptr};
    return newTransaction;
}

// Function to push a transaction onto the stack for a specific account.
void append_history(long long acc_number, const std::string& type, double amount) {
    Transaction* newTransaction = record_transaction(type, amount);
    stack_node* current = start;
    stack_node* previous = nullptr;

    while (current != nullptr && current->acc_number != acc_number) {
        previous = current;
        current = current->next;
    }

    if (current != nullptr && current->acc_number == acc_number) {
        // Account found, push onto existing stack
        newTransaction->next = current->transaction;
        current->transaction = newTransaction;
    }
    else {
        // Account not found, create a new node
        stack_node* newNode = new stack_node{acc_number, newTransaction, current};
        if (previous == nullptr) {
            start = newNode;
        } else {
            previous->next = newNode;
        }
    }
}


// Function to print the transaction history for a specific account.
void print_transaction_history(long long acc_number) {
    stack_node* current = start;
    while (current != nullptr) {
        if (current->acc_number == acc_number) {
            Transaction* transaction = current->transaction;
            if (transaction == nullptr) {
                std::cout << "No transactions for this account.\n";
                return;
            }
            while (transaction != nullptr) {
                std::cout << "Type: " <<std::left<<std::setw(15)<<transaction->type<<"\t|   Amount: $" <<std::left<<std::setw(10)<< transaction->amount << "\t"<< transaction->time << "\n";
                Transaction* next = transaction->next;
                transaction = next;
            }
            return;
        }
        current = current->next;
    }
    std::cout << "No transactions available for this account.\n";
};

//banking system functionality-------------------------------------------------------------------
class BankAccountManagementSystem {
    private:
        BinaryTree binaryTree;

    public:
        BankAccountManagementSystem(){
        binaryTree = BinaryTree();
        loadAccountsFromFile();
    };

    int countWordsInStream(std::istringstream &iss){
        std::streampos originalPos = iss.tellg();
        std::string word;
        int wordCount = 0;

        while (iss >> word)
        {
        wordCount++;
        }

        // Reset the stream position to the original position
        iss.clear();
        iss.seekg(originalPos);

        return wordCount;
    }

    void loadAccountsFromFile() {
        std::ifstream file(account_path);
        if (file.is_open()) {
            std::string line;
            std::getline(file,line);
            while (getline(file, line)){
                std::istringstream iss(line);
                int accountNumber;
                std::string hold;
                std::string accountHolderName,sbalance;
                double balance;
                iss>>hold;
                std::istringstream(hold) >> accountNumber;

                if(countWordsInStream(iss)>=3){
                    std::string firstName, lastName;
                    iss>>firstName;
                    iss>>lastName;
                    accountHolderName = firstName +" "+ lastName;
                    accountHolderName=accountHolderName;
                }
                else{
                    iss >> accountHolderName;
                }
                iss>> sbalance;
                std::istringstream(sbalance) >> balance;
                BankAccount* account = new BankAccount(accountNumber, accountHolderName, balance);
                binaryTree.insertNode(account);
            }
            file.close();
            std::cout << "Accounts loaded from file successfully.\n";
        } else {
            std::cout << "Error: Unable to open file.\n";
        }
    }


    void saveAccountsToFile() {
            std::ofstream file(account_path);
            if (file.is_open()){
                binaryTree.inorderTraversalHead(file);
                file.close();
            }
            else {
                std::cout << "Error: Unable to open file.\n";
            }
    }
    //................................new
    long long generateAccountNumber() {
    long long nextAccountNumber;
    std::ifstream inputFile(accountNo_path); // Read last account number from file

    if (inputFile.is_open()) {
        inputFile >> nextAccountNumber;
        inputFile.close();
    } else {
        nextAccountNumber = 10000; // Start at 1000 if the file doesn't exist
    }
    nextAccountNumber++;
    std::ofstream outputFile(accountNo_path); // Write the updated account number
    if (outputFile.is_open()) {
        outputFile << nextAccountNumber;
        outputFile.close();
    } else {
        std::cerr << "Error writing account number to file!" << std::endl;
    }

    return nextAccountNumber;
    }
    //................................

    void createAccount() {
        int accountNumber;
        std::string accountHolderName;
        double initialBalance;

        accountNumber = generateAccountNumber();
        std::cout << "Enter Account Holder Name: ";
        std::cin.ignore(100,'\n');
        std::getline(std::cin, accountHolderName);
        std::cout << "Enter Initial Balance: ";
        std::cin>>initialBalance;

        BankAccount* account = new BankAccount(accountNumber, accountHolderName, initialBalance);
        binaryTree.insertNode(account);
        append_history(accountNumber,"Initial deposit",initialBalance);

        clear_console();
        std::cout<< "Account created successfully.\n";
        delay_time(3);
        dash_board();
        std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
        std::cout<< "The account number is: "<< accountNumber<<std::endl;
        std::cout<< "The account holder name is: "<< accountHolderName<<std::endl;
        std::cout<< "The account initial balance is: "<< initialBalance<<std::endl;
        std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;

    }

    //function to deposit to bank account
    void deposit() {
        int accountNumber;
        double amount;
        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        Node* node= binaryTree.searchNode(accountNumber);
        if (node != NULL)
            {
            std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
            std::cout << "The account holder name is: " << node->account->accountHolderName<<std::endl;
            std::cout << "Enter Amount to Deposit: ";
            std::cin >> amount;
            append_history(accountNumber,"deposit",amount);
            node->account->balance += amount;
            std::cout << "Deposit successful. New balance: " << node->account->balance<<std::endl;
            std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
        } else {
            std::cout << "Account not found.\n";
        }
    }

    //function to withdraw balace from account
    void withdraw() {
        int accountNumber;
        double amount;
        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;

        Node* node=binaryTree.searchNode(accountNumber);
        std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
        if (node != NULL) {
            std::cout<<"Account holder name is: "<< node->account->accountHolderName<<std::endl;
            std::cout<<"Account balance is: "<< node->account->balance<<std::endl;
            std::cout << "Enter Amount to Withdraw: ";
            std::cin >> amount;
            if (node->account->balance >= amount) {
                node->account->balance -= amount;
                std::cout << "Withdrawal successful. New balance: " << node->account->balance << std::endl;
                append_history(accountNumber,"withdraw",amount);
            }
            else {
                std::cout << "Insufficient balance.\n";
            }
        std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
        } else {
            std::cout << "Account not found.\n";
        }
    }

    void displayBalance() {
        int accountNumber;
        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        Node* node=binaryTree.searchNode(accountNumber);
        if (node != NULL) {
            std::cout<<YELLOW+"---------------------------------------------------------------------------------------------\n"+BLUE;
            std::cout << "Account Number: " << node->account->accountNumber << std::endl;
            std::cout << "Account Holder: " << node->account->accountHolderName << std::endl;
            std::cout << "Balance: " << node->account->balance << std::endl;
            std::cout << "Transaction history: \n"+GREEN;
            print_transaction_history(accountNumber);
            std::cout<<YELLOW+"---------------------------------------------------------------------------------------------\n"+BLUE;
        } else {
            std::cout << "Account not found." << std::endl;
        }
    }
    void removeAccount(){
        int accountNumber;
        std::cout << "Enter Account Number: ";
        std::cin >> accountNumber;
        Node *node = binaryTree.searchNode(accountNumber);
        std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
        if(node!=NULL){
            std::cout << "Account holder: " << node->account->accountHolderName;
            binaryTree.removeNode(accountNumber);
            std::cout<< " has been removed successfully." << std::endl;
        }
        else{
            std::cout<<"Account Number not Found"<<std::endl;
        }
        std::cout<<YELLOW+"--------------------------------------------------\n"+BLUE;
    }
};
//main ffunction-----------------------------------------------------------------------------------
int main(){
    char option;
    BankAccountManagementSystem system;

    while(true){
        dash_board();
        cout << RED+"[1]"+END+" Create Account\n";
        cout << RED+"[2]"+BLUE+" Deposit\n";
        cout << RED+"[3]"+BLUE+" Withdraw\n";
        cout << RED+"[4]"+BLUE+" Display Balance\n";
        cout << RED+"[5]"+BLUE+" Remove Account\n";
        cout << RED+"[6]"+BLUE+" Display contributers\n";
        cout << RED+"[7]"+BLUE+" Exit\n";

        cout << "\nEnter your choice "+RED+"(1-6)"+BLUE+": ";
        cin >> option;

        if(option == '1'){
            dash_board();
            system.createAccount();
            system.saveAccountsToFile();
            navigation_option();
        }
        else if (option == '2'){
            dash_board();
            system.deposit();
            system.saveAccountsToFile();
            navigation_option();

        }
        else if(option == '3'){
            dash_board();
            system.withdraw();
            system.saveAccountsToFile();
            navigation_option();
        }

        else if(option == '4'){
            dash_board();
            system.displayBalance();
            navigation_option();

        }
        else if(option == '5'){
            dash_board();
            system.removeAccount();
            system.saveAccountsToFile();
            navigation_option();
        }
        else if(option == '6'){

        }
        else if(option == '7'){
            cout << "Exiting..." << endl;
            exit(0);
        }
        else{
            cout << "Invalid choice. Please try again!" << endl;
        }
    }
    return 0;
}
