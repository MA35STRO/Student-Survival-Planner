#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

struct Student {
    string username;
    string password;
    string name;
    string studentID;
    string programme;
    int semester;
    double weeklyBudget;
};
struct Expense{
	string studentID;
	string description;
	string category;
	double amount;
	string date;
};
// tambah untuk yang lain ii @ buat struct baru

//Function prototypes
void createAccount();
bool login(Student &currentStudent);
void mainMenu(Student currentStudent);
void addExpense(const Student &currentStudent);
void dailyExpense (const Student &currentStudent);

void createAccount() {
    Student student;

    system("cls");
    cout << "\n========== CREATE ACCOUNT ==========\n";
    cout << "Username: ";
    cin >> student.username;

    cout << "Password: ";
    cin >> student.password;

    cin.ignore();

    cout << "Full Name: ";
    getline(cin, student.name);

    cout << "Student ID: ";
    getline(cin, student.studentID);

    cout << "Programme: ";
    getline(cin, student.programme);

    cout << "Semester: ";
    cin >> student.semester;

    cout << "Weekly Budget (RM): ";
    cin >> student.weeklyBudget;

    ofstream createAcc("accounts.txt", ios::app); // ios :: app - so it add new data & does not overwrite existing data

    if (createAcc.is_open()) {
        createAcc << student.username << ","
                  << student.password << ","
                  << student.name << ","
                  << student.studentID << ","
                  << student.programme << ","
                  << student.semester << ","
                  << student.weeklyBudget << "\n";

        createAcc.close();

        cout << "\nAccount created successfully!\n";
    }
    else {
        cout << "\nUnable to open accounts.txt\n";
    }
}

bool login(Student &currentStudent) {
    string username, password, line;

    system("cls");
    cout << "\n=============== LOGIN ===============\n";
    cout << "Username: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    ifstream createAcc("accounts.txt");

    while (getline(createAcc, line)) { //open file and store each line of data in "line" and repeat
                                       //return true until the end of the line
        stringstream ss(line); //seperate long string in "line"
        Student student; //hold student info from file
        string semester, budget; //getline read as string

        getline(ss, student.username, ',');
        getline(ss, student.password, ',');
        getline(ss, student.name, ',');
        getline(ss, student.studentID, ',');
        getline(ss, student.programme, ',');
        getline(ss, semester, ',');
        getline(ss, budget);

        student.semester = stoi(semester); //convert back to original
        student.weeklyBudget = stod(budget);

        if (username == student.username && password == student.password) {
            currentStudent = student; //copy specific student info to currentStudent
            createAcc.close();
            cout << "\nLogin successful!\n";
            return true;
        }
    }

    createAcc.close();

    cout << "\nInvalid username or password.\n";
    return false;
}

void addExpense(const Student &currentStudent){
	Expense exp;
	exp.studentID = currentStudent.studentID;
	
	system("cls");
	cout << "\n========== ADD NEW EXPENSE ==========\n";
	
	cin.ignore();
	
	cout << "Enter Expense Description (e.g, Lunch, Bus Fare): ";
	getline(cin, exp.description);
	
	cout << "Enter amount(RM): ";
	while(!(cin >> exp.amount) || exp.amount<0){
		cout << "ERROR! Invalid Input! Please Enter a Positive Number (RM): ";
		cin.ignore();
		cin.clear(1000, '\n');
	}
	
	cout << "\nEnter Category\n";
	cout << "1. Food\n";
	cout << "2. Transportation\n";
	cout << "3. Entertainment\n";
	cout << "4. Study\n";
	cout << "5. Others\n";
	
	int categoryChoice;
	cout << "Enter Choice (1-5): ";
	while(!(cin >> categoryChoice) || categoryChoice < 1 || categoryChoice > 5){
		cout << "ERROR! Invalid Input! Please enter a number within the range of 1-5";
		cin.ignore();
		cin.clear(1000, '\n');
	}
	
	switch(categoryChoice){
		case 1:
			exp.category = "Food";
			break;
		
		case 2:
			exp.category = "Transportation";
			break;
		
		case 3:
			exp.category = "Entertainment";
			break;
			
		case 4:
			exp.category = "Study";
			break
		
		default:
			exp.category = "Others";
			break;
	}
	
	cin.ignore();
	cout << "Enter Date (DD/MM/YYYY): ";
	getline(cin, exp.date);
	
	//sambung file untuk addexpense
}


void displayExpense(const Student &currentStudent){
	system("cls");
	cout << "\n=========================================================================\n";
	cout << "                              EXPENSE LIST                                 \n";
	cout << "\n=========================================================================\n";
	
	ifstream file("expense.txt");
	string line;
	bool found = false;
	double totalExpense = 0.0;
	int count = 1;
	
	if (!file.is_open()){
		cout << "\nNo expense recorded yet.";
		cout << "=========================================================================\n";
	}
	else{
		cout << left << setw(5) << "No." 
             << setw(20) << "Description" 
             << setw(18) << "Category" 
             << setw(15) << "Amount" 
             << setw(12) << "Date" << endl;
        cout << "-------------------------------------------------------------------------\n";
        
        string amtStr;
        Expense exp;
        
        while(getline(file, exp.studentID, ',') &&
			  getline(file, exp.description, ',') &&
			  getline(file, exp.category, ',') &&
			  getline(file, exp.amtStr, ',') &&
			  getline(file, exp.date, ',')){
			  	
			if (exp.studentID == currentStudent.studentID){
				exp.amount == stood(amdStr); //change amount string to double
				cout << left << setw(5) << count++
                     << setw(20) << exp.description
                     << setw(18) << exp.category
                     << "RM " << right << setw(10) << fixed << setprecision(2) << exp.amount
                     << left << setw(2) << "" 
                     << setw(12) << exp.date << endl;
                totalExpense += exp.amount;
                found = true;
			}
		}
	file.close();
	
	if(!found){
		system("cls");
		cout << "\n=========================================================================\n";
        cout << "                              EXPENSE LIST                                 \n";
        cout << "=========================================================================\n";
        cout << "\nNo expenses recorded for your account yet.\n";
	}
	else{
		cout << "-------------------------------------------------------------------------\n";
        cout << "Total Expenses: RM " << fixed << setprecision(2) << totalExpense << endl;
        cout << "Weekly Budget : RM " << fixed << setprecision(2) << currentStudent.weeklyBudget << endl;
        cout << "Remaining     : RM " << fixed << setprecision(2) << (currentStudent.weeklyBudget - totalExpense) << endl;
	}
	}
	cout << "=========================================================================\n";
    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

void mainMenu(Student currentStudent) {
    int choice;

    do {
        system("cls");
        cout << "\n========== MAIN MENU ==========\n";
        cout << "Welcome, " << currentStudent.name << "!\n";
        cout << "1. Student Profile\n";
        cout << "2. Add Expense\n";
        cout << "3. View Expenses\n";
        cout << "4. Budget Analysis\n";
        cout << "5. Add Assignment\n";
        cout << "6. View Assignments\n";
        cout << "7. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                system("cls");
                cout << "\n========== STUDENT PROFILE ==========\n";
                cout<< "\nStudent Name : " << currentStudent.name << endl;
                cout << "Student ID: " << currentStudent.studentID << endl;
                cout << "Programme: " << currentStudent.programme << endl;
                cout << "Semester: " << currentStudent.semester << endl;
                cout << "Weekly Budget: RM" << currentStudent.weeklyBudget << endl;

                cout<< "\nPress Enter to return to the main menu...";
                cin.ignore();
                cin.get();
                break;

            case 2:
                addExpense(currentStudent);
                break;

            case 3:
                displayExpense(currentStudent);
                break;

            case 4:
                system("cls");
                cout << "\nBudget Analysis selected.\n";
                //sambung @ buat function baru
                break;

            case 5:
                system("cls");
                cout << "\nAdd Assignment selected.\n";
                //sambung @ buat function baru
                break;

            case 6:
                system("cls");
                cout << "\nView Assignments selected.\n";
                //sambung @ buat function baru
                break;

            case 7:
                system("cls");
                cout << "\nLogging out...\n";
                //sambung @ buat function baru
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 7);
}

int main() {
    int choice;
    Student currentStudent;

    do {
        system("cls");
        cout << "\n================================\n";
        cout << "    UTP STUDENT SURVIVAL\n";
        cout << "           PLANNER\n";
        cout << "================================\n";
        cout << "1. Login\n";
        cout << "2. Create Account\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (login(currentStudent)) {
                    mainMenu(currentStudent);
                }
                break;

            case 2:
                createAccount();
                break;

            case 3:
                cout << "\nSee you later!\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 3);

    return 0;
}
