#include <iostream> 
#include <fstream>
#include <forward_list>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

class TaskNode {
    private:
        string title;
        string description;
        string dueDate;
    public:
        TaskNode() {}
        TaskNode(string &title, string &description, string &dueDate) {
            this->title = title;
            this->description = description;
            this->dueDate = dueDate;
        }
        string getTitle() { return title;}
        string getDescription() { return description; }
        string getDueDate() { return dueDate; }
        void printTask() {
            cout << endl;
            cout << " Task title: " << title << endl;
            cout << " Task Description: " << description << endl;
            cout << " Task Due Date: " << dueDate << endl;
        }
};
// singleton obj 
class TaskManager {
    private: 
        forward_list<TaskNode> tasks;
        TaskManager() { loadTasks();}
        static bool isObjectcreated;
    protected:
        void loadTasks();
        void unloadTasks();
    public:
        static TaskManager* getTaskManager() {
            if (isObjectcreated == false) {
                isObjectcreated = true;
                return new TaskManager();
            }
            else 
                cout << "You can get only one task manager"<< endl;
            return nullptr;
        }
        int menu();
        void addTask();
        void deleteTask(string&);
        void viewAllTasks();
        ~TaskManager();
};
bool TaskManager:: isObjectcreated = false;

int TaskManager:: menu() {
    int choice;
    cout << endl;
    cout << "1. Add New Task" << endl;
    cout << "2. Delete Task" << endl;
    cout << "3. View All Task" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter Your Choice: ";
    cin >> choice;
    return choice;
}

void TaskManager:: addTask() {
    string t, d, dd;
    cout << "\nEnter task title: " << endl;
    cin.ignore();
    getline(cin, t);
    cout << "Enter task descripton: " << endl;
    cin.ignore();
    getline(cin, d);
    cout <<"Enter due date: " << endl;
    cin.ignore();
    getline(cin, dd);
    TaskNode newTask(t, d, dd);
    tasks.push_front(newTask);
    cout << "Task Added Successfully" << endl;
}

void TaskManager:: deleteTask(string &t) {
    forward_list<TaskNode>::iterator it, it1;
    bool flag = true;
    it = tasks.begin();
    it1 = tasks.before_begin();

    while (it != tasks.end()) {
        if (it->getTitle() == t) {
            tasks.erase_after(it1);
            cout << "\nTask deleted Successfully" << endl;
            flag = false;
            break;
        }
        it1 = it++;
        if (flag) 
            cout << "\nTask not found";
    }
}

void TaskManager:: viewAllTasks() {
    if (tasks.empty())
        cout <<"\nNo task in the task manager";
    else {
    for (auto t : tasks) 
        t.printTask();
    }
    char x;
    cout << "\n Enter any key to exit";
    cin >> x;
}

TaskManager:: ~TaskManager() {
    unloadTasks();
    isObjectcreated = false;
}

void TaskManager:: loadTasks() {
    tasks.clear();
    ifstream fin;
    string t, d, dd;
    fin.open("mytasks.txt", ios:: in | ios:: binary);
    if (!fin)
        cout <<"task manager is empty";
    else {
        fin.ignore();
        while(!fin.eof()) {
            getline(fin, t);
            getline(fin, d);
            getline(fin, dd);
            tasks.push_front(*new TaskNode(t, d, dd));
        }
    }
    fin.close();
}

void TaskManager:: unloadTasks() {
    TaskNode temp;
    ofstream fout;
    if(!tasks.empty()) {
        fout.open("mytasks.txt", ios::out | ios::binary);
        while (!tasks.empty())
        {
            temp = tasks.front();
            fout << '\n' << temp.getTitle();
            fout << '\n' << temp.getDescription();
            fout << '\n' << temp.getDueDate();
            tasks.pop_front();
        }
        fout.close();
    }
    else 
        remove("mytasks.txt");
}

int main() {
    TaskManager *tm;
    tm = TaskManager :: getTaskManager();

    while (true) {
        system("clear");
        switch (tm->menu())
        {
        case 1:
            tm->addTask();
            break;
        case 2: {
            cout <<"\nEnter title of the task to delete: ";
            string temp = "";
            getline(cin, temp);
            tm->deleteTask(temp);
            break;
        }
        case 3: 
            tm->viewAllTasks();
            break;
        case 4: 
            delete tm;
            exit(0);
        default:
            cout << "\nInvalid choice";
        }
        getchar();
        fflush(stdin);
    }
}