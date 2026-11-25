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

struct Task {
    string studentID;
    string title;
    string dueDate;
    string priority;
    string status;
};

// Arrays that hold the logged-in student's data while the program runs.
// loadData() fills them from the text files, saveData() writes them back.
const int MAX_EXPENSES = 100;
const int MAX_TASKS = 50;

Expense expenses[MAX_EXPENSES];
Task tasks[MAX_TASKS];

int expenseCount = 0;
int taskCount = 0;

double weeklyBudget = 150.00; // replaced by the student's own budget in loadData()

// File names used for storage
const string EXPENSE_FILE = "expense.txt";
const string TASK_FILE = "tasks.txt";

//Function prototypes
void createAccount();
bool login(Student &currentStudent);
void mainMenu(Student currentStudent);
void addExpense(const Student &currentStudent);
void displayExpense(const Student &currentStudent);
void calculateBudget();
void addTask(const Student &currentStudent);
void viewTasks(const Student &currentStudent);
void saveData(const Student &currentStudent);
void loadData(const Student &currentStudent);

// Called when cin fails (e.g. user typed a letter where a number was expected).
// Resets cin and throws away the bad input so the program doesn't loop forever.
void clearBadInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

// Returns true if the text contains a comma.
// Commas are used to separate fields in the text files, so they are not allowed.
bool hasComma(string text) {
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == ',') {
            return true;
        }
    }
    return false;
}

// Returns true if the username is already used in accounts.txt
bool usernameExists(string username) {
    ifstream accFile("accounts.txt");
    string line, savedUsername;

    while (getline(accFile, line)) {
        stringstream ss(line);
        getline(ss, savedUsername, ',');

        if (savedUsername == username) {
            accFile.close();
            return true;
        }
    }

    accFile.close();
    return false;
}

void createAccount() {
    Student student;

    system("cls");
    cout << "\n========== CREATE ACCOUNT ==========\n";
    cout << "Username: ";
    cin >> student.username;

    while (hasComma(student.username) || usernameExists(student.username)) {
        if (hasComma(student.username)) {
            cout << "Commas are not allowed. Please enter another username: ";
        }
        else {
            cout << "Username already taken. Please choose another: ";
        }
        cin >> student.username;
    }

    cout << "Password: ";
    cin >> student.password;

    while (hasComma(student.password)) {
        cout << "Commas are not allowed. Please enter another password: ";
        cin >> student.password;
    }

    cin.ignore();

    cout << "Full Name: ";
    getline(cin, student.name);

    while (hasComma(student.name)) {
        cout << "Commas are not allowed. Please enter your full name again: ";
        getline(cin, student.name);
    }

    cout << "Student ID: ";
    getline(cin, student.studentID);

    while (hasComma(student.studentID)) {
        cout << "Commas are not allowed. Please enter your student ID again: ";
        getline(cin, student.studentID);
    }

    cout << "Programme: ";
    getline(cin, student.programme);

    while (hasComma(student.programme)) {
        cout << "Commas are not allowed. Please enter your programme again: ";
        getline(cin, student.programme);
    }

    cout << "Semester: ";
    cin >> student.semester;

    while (cin.fail() || student.semester < 1) {
        clearBadInput();
        cout << "Invalid input! Please enter a valid semester number: ";
        cin >> student.semester;
    }

    cout << "Weekly Budget (RM): ";
    cin >> student.weeklyBudget;

    while (cin.fail() || student.weeklyBudget < 0) {
        clearBadInput();
        cout << "Invalid input! Please enter a positive amount (RM): ";
        cin >> student.weeklyBudget;
    }

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

        // only use the line if all 7 fields were read (blank lines are skipped)
        if (getline(ss, student.username, ',') &&
            getline(ss, student.password, ',') &&
            getline(ss, student.name, ',') &&
            getline(ss, student.studentID, ',') &&
            getline(ss, student.programme, ',') &&
            getline(ss, semester, ',') &&
            getline(ss, budget)) {

            //convert back to original (stringstream works on all compilers, incl. Dev-C++)
            stringstream semStream(semester);
            semStream >> student.semester;

            stringstream budgetStream(budget);
            budgetStream >> student.weeklyBudget;

            if (username == student.username && password == student.password) {
                currentStudent = student; //copy specific student info to currentStudent
                createAcc.close();
                cout << "\nLogin successful!\n";
                return true;
            }
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
	cin >> exp.amount;
	while(cin.fail() || exp.amount<0){   // cin.fail() is true when letters are typed
		cout << "ERROR! Invalid Input! Please Enter a Positive Number (RM): ";
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> exp.amount;
	}
	
	cout << "\nEnter Category\n";
	cout << "1. Food\n";
	cout << "2. Transportation\n";
	cout << "3. Entertainment\n";
	cout << "4. Study\n";
	cout << "5. Others\n";
	
	int categoryChoice;
	cout << "Enter Choice (1-5): ";
	cin >> categoryChoice;
	while(cin.fail() || categoryChoice < 1 || categoryChoice > 5){
		cout << "ERROR! Invalid Input! Please enter a number within the range of 1-5: ";
		cin.clear();
		cin.ignore(1000, '\n');
		cin >> categoryChoice;
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
			break;
		
		default:
			exp.category = "Others";
			break;
	}
	
	cin.ignore();
	cout << "Enter Date (DD/MM/YYYY): ";
	getline(cin, exp.date);

	// Store the expense in the array, then save it to file
	if (expenseCount >= MAX_EXPENSES){
		cout << "\nExpense storage is full! Expense not saved.\n";
	}
	else{
		expenses[expenseCount] = exp;
		expenseCount++;
		saveData(currentStudent);
		cout << "\nExpense successfully added!\n";
	}

	cout << "\nPress Enter to return to the main menu...";
	cin.get();
}


void displayExpense(const Student &currentStudent){
	system("cls");
	cout << "\n=========================================================================\n";
	cout << "                              EXPENSE LIST                                 \n";
	cout << "=========================================================================\n";
	
	ifstream file("expense.txt");
	string line;
	bool found = false;
	double totalExpense = 0.0;
	int count = 1;
	
	if (!file.is_open()){
		cout << "\nNo expense recorded yet.\n";
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
        
        while (getline(file, line)){
        	stringstream ss(line);
        
        	if (getline(ss, exp.studentID, ',') &&
			  	getline(ss, exp.description, ',') &&
			  	getline(ss, exp.category, ',') &&
			  	getline(ss, amtStr, ',') &&
			  	getline(ss, exp.date)){
			  	
				if (exp.studentID == currentStudent.studentID){
					stringstream amtStream(amtStr); //change amount string to double
					amtStream >> exp.amount;
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

// MODUL : calculateBudget()
void calculateBudget()
{
    double totalSpent = 0;
    double remainingBudget;
    double percentage = 0;

    for (int i = 0; i < expenseCount; i++)
    {
        totalSpent += expenses[i].amount;
    }

    remainingBudget = weeklyBudget - totalSpent;

    if (weeklyBudget > 0)   // avoid dividing by zero
    {
        percentage = (totalSpent / weeklyBudget) * 100;
    }

    cout << "\n========== BUDGET ANALYSIS ==========\n";

    cout << fixed << setprecision(2);

    cout << "Weekly Budget   : RM" << weeklyBudget << endl;
    cout << "Total Spent     : RM" << totalSpent << endl;
    cout << "Remaining Budget: RM" << remainingBudget << endl;

    cout << "Budget Used     : " << percentage << "%" << endl;

    if (totalSpent > weeklyBudget)
    {
        cout << "Status: OVER BUDGET!" << endl;
        cout << "You have exceeded your budget by RM"
             << totalSpent - weeklyBudget << endl;
    }
    else if (totalSpent == weeklyBudget)
    {
        cout << "Status: BUDGET FULLY USED!" << endl;
    }
    else
    {
        cout << "Status: WITHIN BUDGET" << endl;
    }

    cout << "=====================================\n";
}

// MODUL : addTask()
void addTask(const Student &currentStudent)
{
    if (taskCount >= MAX_TASKS)
    {
        cout << "\nTask storage is full!\n";
        cin.ignore(1000, '\n');   // remove the leftover Enter from the menu choice
        return;
    }

    cout << "\n========== ADD NEW TASK ==========\n";

    cin.ignore();

    tasks[taskCount].studentID = currentStudent.studentID;

    cout << "Enter task title: ";
    getline(cin, tasks[taskCount].title);

    cout << "Enter due date (DD/MM/YYYY): ";
    getline(cin, tasks[taskCount].dueDate);

    cout << "\nSelect Priority:\n";
    cout << "1. High\n";
    cout << "2. Medium\n";
    cout << "3. Low\n";

    int choice;

    cout << "Enter priority: ";
    cin >> choice;

    if (cin.fail())   // letter typed -> treat as invalid choice
    {
        cin.clear();
        choice = 0;
    }
    cin.ignore(1000, '\n');   // remove the rest of the line so the "Press Enter" pause works

    switch (choice)
    {
        case 1:
            tasks[taskCount].priority = "High";
            break;

        case 2:
            tasks[taskCount].priority = "Medium";
            break;

        case 3:
            tasks[taskCount].priority = "Low";
            break;

        default:
            cout << "Invalid choice! Default priority: Medium\n";
            tasks[taskCount].priority = "Medium";
    }

    tasks[taskCount].status = "Pending";

    taskCount++;

    saveData(currentStudent);   // keep the new task after the program closes

    cout << "\nTask successfully added!\n";

    cout << "==================================\n";
}

// MODUL : viewTasks() - shows the student's assignments and lets them
// mark one as Completed
void viewTasks(const Student &currentStudent)
{
    cout << "\n=========================== ASSIGNMENT LIST ===========================\n";

    if (taskCount == 0)
    {
        cout << "\nNo assignments recorded yet.\n";
        cout << "=======================================================================\n";
        cin.ignore(1000, '\n');   // remove the leftover Enter from the menu choice
        return;
    }

    cout << left << setw(5) << "No."
         << setw(30) << "Title"
         << setw(14) << "Due Date"
         << setw(10) << "Priority"
         << setw(10) << "Status" << endl;
    cout << "-----------------------------------------------------------------------\n";

    int pendingCount = 0;

    for (int i = 0; i < taskCount; i++)
    {
        cout << left << setw(5) << i + 1
             << setw(30) << tasks[i].title
             << setw(14) << tasks[i].dueDate
             << setw(10) << tasks[i].priority
             << setw(10) << tasks[i].status << endl;

        if (tasks[i].status == "Pending")
        {
            pendingCount++;
        }
    }

    cout << "-----------------------------------------------------------------------\n";
    cout << "Total Assignments: " << taskCount << endl;
    cout << "Pending          : " << pendingCount << endl;
    cout << "Completed        : " << taskCount - pendingCount << endl;
    cout << "=======================================================================\n";

    int taskNumber;

    cout << "\nEnter assignment number to mark as Completed (0 to go back): ";
    cin >> taskNumber;

    if (cin.fail())   // letter typed
    {
        cin.clear();
        taskNumber = -1;
    }
    cin.ignore(1000, '\n');   // remove the rest of the line so the "Press Enter" pause works

    if (taskNumber == 0)
    {
        return;
    }
    else if (taskNumber < 1 || taskNumber > taskCount)
    {
        cout << "Invalid assignment number.\n";
    }
    else if (tasks[taskNumber - 1].status == "Completed")
    {
        cout << "This assignment is already completed.\n";
    }
    else
    {
        tasks[taskNumber - 1].status = "Completed";
        saveData(currentStudent);
        cout << "\"" << tasks[taskNumber - 1].title << "\" marked as Completed!\n";
    }
}

// Helper for saveData(): reads a file and returns every line that does NOT
// belong to the given student, so other students' records are not erased.
string keepOtherStudents(string fileName, string studentID)
{
    ifstream inFile(fileName.c_str());   // .c_str() is needed by older compilers (Dev-C++)
    string line, id;
    string others = "";

    while (getline(inFile, line))
    {
        stringstream ss(line);
        getline(ss, id, ',');   // first field is always the student ID

        if (id != studentID && line != "")
        {
            others += line + "\n";
        }
    }

    inFile.close();
    return others;
}

// Helper for saveData(): makes a text field safe to store in the file.
// - a comma would split the field in two, so it is replaced with ';'
// - an empty field is stored as "-" so the line can always be read back
string cleanField(string text)
{
    for (int i = 0; i < text.length(); i++)
    {
        if (text[i] == ',')
        {
            text[i] = ';';
        }
    }

    if (text == "")
    {
        text = "-";
    }

    return text;
}

// MODUL 7 : File Storage - saveData()
// Writes the current student's expenses and tasks into the text files.
// Format (one record per line, comma separated):
//   expense.txt : studentID,description,category,amount,date
//   tasks.txt   : studentID,title,dueDate,priority,status
void saveData(const Student &currentStudent)
{
    // 1. Save expenses
    string otherExpenses = keepOtherStudents(EXPENSE_FILE, currentStudent.studentID);

    ofstream expFile(EXPENSE_FILE.c_str());   // overwrite with updated data

    if (!expFile.is_open())
    {
        cout << "\nError: unable to open " << EXPENSE_FILE << " for saving.\n";
    }
    else
    {
        expFile << otherExpenses;

        for (int i = 0; i < expenseCount; i++)
        {
            expFile << cleanField(expenses[i].studentID) << ","
                    << cleanField(expenses[i].description) << ","
                    << cleanField(expenses[i].category) << ","
                    << fixed << setprecision(2) << expenses[i].amount << ","
                    << cleanField(expenses[i].date) << "\n";
        }

        expFile.close();
    }

    // 2. Save tasks
    string otherTasks = keepOtherStudents(TASK_FILE, currentStudent.studentID);

    ofstream taskFile(TASK_FILE.c_str());

    if (!taskFile.is_open())
    {
        cout << "\nError: unable to open " << TASK_FILE << " for saving.\n";
    }
    else
    {
        taskFile << otherTasks;

        for (int i = 0; i < taskCount; i++)
        {
            taskFile << cleanField(tasks[i].studentID) << ","
                     << cleanField(tasks[i].title) << ","
                     << cleanField(tasks[i].dueDate) << ","
                     << cleanField(tasks[i].priority) << ","
                     << cleanField(tasks[i].status) << "\n";
        }

        taskFile.close();
    }
}

// MODUL 8 : File Handling - loadData()
// Reads the text files and loads only the logged-in student's records
// into the expenses[] and tasks[] arrays.
void loadData(const Student &currentStudent)
{
    string line;
    int skipped = 0;   // records that did not fit in the arrays

    // Start fresh so data from a previous login is not mixed in
    expenseCount = 0;
    taskCount = 0;
    weeklyBudget = currentStudent.weeklyBudget;

    // 1. Load expenses
    ifstream expFile(EXPENSE_FILE.c_str());

    if (expFile.is_open())   // if the file doesn't exist yet, there is simply no data
    {
        while (getline(expFile, line))
        {
            stringstream ss(line);
            Expense exp;
            string amtStr;

            if (getline(ss, exp.studentID, ',') &&
                getline(ss, exp.description, ',') &&
                getline(ss, exp.category, ',') &&
                getline(ss, amtStr, ','))
            {
                getline(ss, exp.date);   // last field, may be empty in old files

                if (exp.studentID == currentStudent.studentID)
                {
                    if (expenseCount < MAX_EXPENSES)
                    {
                        stringstream amtStream(amtStr);   // string -> double
                        amtStream >> exp.amount;
                        expenses[expenseCount] = exp;
                        expenseCount++;
                    }
                    else
                    {
                        skipped++;
                    }
                }
            }
        }

        expFile.close();
    }

    // 2. Load tasks
    ifstream taskFile(TASK_FILE.c_str());

    if (taskFile.is_open())
    {
        while (getline(taskFile, line))
        {
            stringstream ss(line);
            Task t;

            if (getline(ss, t.studentID, ',') &&
                getline(ss, t.title, ',') &&
                getline(ss, t.dueDate, ',') &&
                getline(ss, t.priority, ','))
            {
                getline(ss, t.status);   // last field, may be empty in old files

                if (t.studentID == currentStudent.studentID)
                {
                    if (taskCount < MAX_TASKS)
                    {
                        tasks[taskCount] = t;
                        taskCount++;
                    }
                    else
                    {
                        skipped++;
                    }
                }
            }
        }

        taskFile.close();
    }

    if (skipped > 0)
    {
        cout << "\nWarning: " << skipped << " saved record(s) could not be loaded "
             << "because storage is full. They will be removed on the next save.\n";
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

void mainMenu(Student currentStudent) {
    int choice;

    loadData(currentStudent);   // bring back this student's saved expenses & tasks

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

        if (cin.fail()) { // letter typed -> show "Invalid choice" instead of looping forever
            clearBadInput();
            choice = 0;
        }

        switch (choice) {
            case 1:
                system("cls");
                cout << "\n========== STUDENT PROFILE ==========\n";
                cout<< "\nStudent Name : " << currentStudent.name << endl;
                cout << "Student ID: " << currentStudent.studentID << endl;
                cout << "Programme: " << currentStudent.programme << endl;
                cout << "Semester: " << currentStudent.semester << endl;
                cout << "Weekly Budget: RM" << fixed << setprecision(2) << currentStudent.weeklyBudget << endl;

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
                calculateBudget();
                cout << "\nPress Enter to return to the main menu...";
                cin.ignore();
                cin.get();
                break;

            case 5:
                system("cls");
                addTask(currentStudent);
                cout << "\nPress Enter to return to the main menu...";
                cin.get();   // addTask() already cleared the input line
                break;

            case 6:
                system("cls");
                viewTasks(currentStudent);
                cout << "\nPress Enter to return to the main menu...";
                cin.get();   // viewTasks() already cleared the input line
                break;

            case 7:
                system("cls");
                saveData(currentStudent);   // make sure everything is saved before leaving
                cout << "\nData saved. Logging out...\n";
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

        if (cin.fail()) { // letter typed -> show "Invalid choice" instead of looping forever
            clearBadInput();
            choice = 0;
        }

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
