
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>

using namespace std;

const int TABLE_SIZE = 100;
const int MAX_COURSES = 10;

struct Student {
    int studentID;
    string studentName;
    string department;
    int level;
    
    string courseNames[MAX_COURSES];
    double courseGrades[MAX_COURSES];
    int numCourses;
    
    double gpa;

    void calculateGPA() {
        if (numCourses == 0) {
            gpa = 0.0;
            return;
        }
        
        double totalGradePoints = 0.0;
        for (int i = 0; i < numCourses; i++) {
            double percentage = courseGrades[i];
            double gradePoint = 0.0;
            
            if (percentage >= 95) gradePoint = 5.0;
            else if (percentage >= 90) gradePoint = 4.75;
            else if (percentage >= 85) gradePoint = 4.5;
            else if (percentage >= 80) gradePoint = 4.0;
            else if (percentage >= 75) gradePoint = 3.5;
            else if (percentage >= 70) gradePoint = 3.0;
            else if (percentage >= 65) gradePoint = 2.5;
            else if (percentage >= 60) gradePoint = 2.0;
            else gradePoint = 0.0;
            
            totalGradePoints += gradePoint;
        }
        
        gpa = totalGradePoints / numCourses;
    }

    bool addCourse(string name, double grade) {
        if (numCourses >= MAX_COURSES) {
            cout << "Error: Cannot add more courses. Maximum is " << MAX_COURSES << "\n";
            return false;
        }
        if (grade < 0 || grade > 100) {
            cout << "Error: Grade must be between 0 and 100.\n";
            return false;
        }
        
        courseNames[numCourses] = name;
        courseGrades[numCourses] = grade;
        numCourses++;
        calculateGPA();
        return true;
    }
};

struct Node {
    Student data;
    Node* next;
};

class HashTable {
private:
    Node* table[TABLE_SIZE];
    int elementCount;

    int countRecursive(Node* ptr) {
        if (ptr == NULL) {
            return 0;
        }
        return 1 + countRecursive(ptr->next);
    }

    void displayRecursive(Node* ptr) {
        if (ptr == NULL) {
            return;
        }
        displayStudentInfo(ptr->data);
        displayRecursive(ptr->next);
    }

public:
    HashTable() {
        elementCount = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = NULL;
        }
    }

    int hashFunction(int studentID) {
        return studentID % TABLE_SIZE;
    }

    bool addStudent(int id, string name, string dept, int lvl, string courses[], double grades[], int courseCount) {
        if (findStudent(id) != NULL) {
            cout << "Error: Student with ID " << id << " already exists.\n";
            return false;
        }
        
        if (lvl < 1 || lvl > 10) {
            cout << "Error: Level must be between 1 and 10.\n";
            return false;
        }
        
        if (dept != "IT" && dept != "CS" && dept != "CE") {
            cout << "Error: Department must be IT, CS, or CE.\n";
            return false;
        }
        
        int index = hashFunction(id);
        
        Student newStudent;
        newStudent.studentID = id;
        newStudent.studentName = name;
        newStudent.department = dept;
        newStudent.level = lvl;
        newStudent.numCourses = 0;
        
        for (int i = 0; i < courseCount; i++) {
            newStudent.addCourse(courses[i], grades[i]);
        }
        
        newStudent.calculateGPA();
        
        Node* newNode = new Node;
        newNode->data = newStudent;
        newNode->next = table[index];
        table[index] = newNode;
        
        elementCount++;
        return true;
    }

    Student* findStudent(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        
        while (current != NULL) {
            if (current->data.studentID == id) {
                return &(current->data);
            }
            current = current->next;
        }
        
        return NULL;
    }

    void deleteStudent(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        Node* previous = NULL;
        
        while (current != NULL) {
            if (current->data.studentID == id) {
                if (previous == NULL) {
                    table[index] = current->next;
                } else {
                    previous->next = current->next;
                }
                delete current;
                elementCount--;
                cout << "Student deleted successfully!\n";
                return;
            }
            previous = current;
            current = current->next;
        }
        
        cout << "Student not found!\n";
    }

    void displayStudentInfo(const Student& student) {
        cout << "========================================\n";
        cout << "Student ID   : " << student.studentID << "\n";
        cout << "Name         : " << student.studentName << "\n";
        cout << "Department   : " << student.department << "\n";
        cout << "Level        : " << student.level << "\n";
        cout << "GPA (5.0)    : " << fixed << setprecision(2) << student.gpa << "\n";
        cout << "Courses      : ";
        
        if (student.numCourses == 0) {
            cout << "N/A";
        } else {
            for (int i = 0; i < student.numCourses; i++) {
                cout << student.courseNames[i] << " (" 
                     << fixed << setprecision(1) << student.courseGrades[i] << "%)";
                if (i < student.numCourses - 1) cout << ", ";
            }
        }
        cout << "\n========================================\n\n";
    }

    void viewAllStudents() {
        bool found = false;
        
        cout << "\n========== ALL STUDENTS ==========\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                found = true;
                displayStudentInfo(current->data);
                current = current->next;
            }
        }
        
        if (!found) {
            cout << "No students found!\n";
        }
    }

    Student* getAllStudents(int& count) {
        count = elementCount;
        if (count == 0) {
            return NULL;
        }

        Student* students = new Student[count];
        int k = 0;
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                students[k++] = current->data;
                current = current->next;
            }
        }
        return students;
    }

    void sortStudentsByGPA() {
        int count = 0;
        Student* students = getAllStudents(count);
        
        if (count == 0) {
            cout << "No students to sort.\n";
            return;
        }

        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (students[j].gpa < students[j + 1].gpa) {
                    Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        
        cout << "\n========== STUDENTS SORTED BY GPA ==========\n";
        for (int i = 0; i < count; i++) {
            displayStudentInfo(students[i]);
        }

        delete[] students;
    }

    void sortStudentsByName() {
        int count = 0;
        Student* students = getAllStudents(count);

        if (count == 0) {
            cout << "No students to sort.\n";
            return;
        }

        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (students[j].studentName > students[j + 1].studentName) {
                    Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        
        cout << "\n========== STUDENTS SORTED BY NAME ==========\n";
        for (int i = 0; i < count; i++) {
            displayStudentInfo(students[i]);
        }
        
        delete[] students;
    }

    void findStudentsByLevel(int level) {
        bool found = false;
        
        if (level < 1 || level > 10) {
            cout << "Error: Level must be between 1 and 10.\n";
            return;
        }
        
        cout << "\n========== Students in Level " << level << " ==========\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                if (current->data.level == level) {
                    found = true;
                    displayStudentInfo(current->data);
                }
                current = current->next;
            }
        }
        
        if (!found) {
            cout << "No students in level " << level << "\n";
        }
    }

    void findStudentsByDepartment(string dept) {
        bool found = false;
        
        if (dept != "IT" && dept != "CS" && dept != "CE") {
            cout << "Error: Department must be IT, CS, or CE.\n";
            return;
        }
        
        cout << "\n========== Students in Department " << dept << " ==========\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                if (current->data.department == dept) {
                    found = true;
                    displayStudentInfo(current->data);
                }
                current = current->next;
            }
        }
        
        if (!found) {
            cout << "No students in department " << dept << "\n";
        }
    }

    void findStudentsByCourse(string courseName) {
        bool found = false;
        
        cout << "\n========== Students Taking Course: " << courseName << " ==========\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                bool enrolled = false;
                for (int j = 0; j < current->data.numCourses; j++) {
                    if (current->data.courseNames[j] == courseName) {
                        enrolled = true;
                        break;
                    }
                }
                
                if (enrolled) {
                    found = true;
                    displayStudentInfo(current->data);
                }
                current = current->next;
            }
        }
        
        if (!found) {
            cout << "No students taking course: " << courseName << "\n";
        }
    }

    void displayStudentStatistics() {
        int count = 0;
        Student* students = getAllStudents(count);
        
        if (count == 0) {
            cout << "No student data available.\n";
            return;
        }

        cout << "\n========== STUDENT STATISTICS ==========\n";
        cout << "Total Students: " << count << "\n";

        double totalGPA = 0.0;
        for (int i = 0; i < count; i++) {
            totalGPA += students[i].gpa;
        }
        cout << "Overall Average GPA: " << fixed << setprecision(2)
             << (totalGPA / count) << "/5.0\n";
        
        map<string, int> deptCounts;
        map<string, double> deptGpaSums;
        
        for (int i = 0; i < count; i++) {
            deptCounts[students[i].department]++;
            deptGpaSums[students[i].department] += students[i].gpa;
        }

        cout << "\n--- By Department ---\n";
        map<string, int>::iterator it;
        for (it = deptCounts.begin(); it != deptCounts.end(); it++) {
            string dept = it->first;
            int deptCount = it->second;
            double avgGpa = deptGpaSums[dept] / deptCount;
            
            cout << dept << ": " << deptCount << " student(s), Avg GPA: " 
                 << fixed << setprecision(2) << avgGpa << "/5.0\n";
        }
        
        map<int, int> levelCounts;
        map<int, double> levelGpaSums;
        
        for (int i = 0; i < count; i++) {
            levelCounts[students[i].level]++;
            levelGpaSums[students[i].level] += students[i].gpa;
        }

        cout << "\n--- By Level ---\n";
        map<int, int>::iterator it2;
        for (it2 = levelCounts.begin(); it2 != levelCounts.end(); it2++) {
            int level = it2->first;
            int levelCount = it2->second;
            double avgGpa = levelGpaSums[level] / levelCount;
            
            cout << "Level " << level << ": " << levelCount << " student(s), Avg GPA: " 
                 << fixed << setprecision(2) << avgGpa << "/5.0\n";
        }
        cout << "========================================\n";

        delete[] students;
    }

    int countStudents() {
        int total = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            total += countRecursive(table[i]);
        }
        return total;
    }

    void displayRecursiveAll() {
        cout << "\n========== RECURSIVE DISPLAY ==========\n";
        bool found = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i] != NULL) {
                found = true;
                displayRecursive(table[i]);
            }
        }
        if (!found) {
            cout << "No students found!\n";
        }
    }

    void displayHashTableStatistics() {
        cout << "\n========== HASH TABLE STATISTICS ==========\n";
        cout << "Total Students : " << elementCount << "\n";
        cout << "Table Size     : " << TABLE_SIZE << "\n";
        cout << "Load Factor    : " << fixed << setprecision(2) 
             << (double)elementCount / TABLE_SIZE << "\n";
        
        int collisions = 0;
        int emptyBuckets = 0;
        int longestChain = 0;
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            int chainLength = 0;
            Node* current = table[i];
            
            while (current != NULL) {
                chainLength++;
                current = current->next;
            }
            
            if (chainLength > 1) {
                collisions += (chainLength - 1);
            }
            if (chainLength == 0) {
                emptyBuckets++;
            }
            if (chainLength > longestChain) {
                longestChain = chainLength;
            }
        }
        
        cout << "Collisions     : " << collisions << "\n";
        cout << "Empty Buckets  : " << emptyBuckets << "\n";
        cout << "Longest Chain  : " << longestChain << "\n";
        cout << "===========================================\n";
    }

    void saveToFile(string filename) {
        ofstream file(filename.c_str());
        
        if (!file.is_open()) {
            cout << "Error opening file!\n";
            return;
        }
        
        file << "========== STUDENT DATABASE ==========\n\n";
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                file << "Student ID   : " << current->data.studentID << "\n";
                file << "Name         : " << current->data.studentName << "\n";
                file << "Department   : " << current->data.department << "\n";
                file << "Level        : " << current->data.level << "\n";
                file << "GPA (5.0)    : " << fixed << setprecision(2) << current->data.gpa << "\n";
                file << "Courses      : ";
                
                if (current->data.numCourses == 0) {
                    file << "N/A";
                } else {
                    for (int j = 0; j < current->data.numCourses; j++) {
                        file << current->data.courseNames[j] << " (" 
                             << fixed << setprecision(1) << current->data.courseGrades[j] << "%)";
                        if (j < current->data.numCourses - 1) file << ", ";
                    }
                }
                file << "\n--------------------------------------\n";
                current = current->next;
            }
        }
        
        file.close();
        cout << "Data saved to " << filename << " successfully!\n";
    }

    void reverseStudentsArray() {
        int count = 0;
        Student* students = getAllStudents(count);
        
        if (count == 0) {
            cout << "No students to reverse.\n";
            return;
        }

        cout << "\n========== STUDENTS ARRAY - REVERSED ==========\n";
        
        for (int i = 0; i < count / 2; i++) {
            Student temp = students[i];
            students[i] = students[count - 1 - i];
            students[count - 1 - i] = temp;
        }

        for (int i = 0; i < count; i++) {
            displayStudentInfo(students[i]);
        }

        delete[] students;
    }

    void loadFromFile(string filename) {
        ifstream file(filename.c_str());
        
        if (!file.is_open()) {
            cout << "No previous data found. Starting with empty database.\n";
            return;
        }
        
        string line;
        Student currentStudent;
        bool readingStudent = false;
        int coursesRead = 0;
        
        getline(file, line); 
        getline(file, line); 
        
        while (getline(file, line)) {
            if (line.find("--------------------------------------") != string::npos) {
                if (readingStudent) {
                    int index = hashFunction(currentStudent.studentID);
                    
                    Node* newNode = new Node;
                    newNode->data = currentStudent;
                    newNode->next = table[index];
                    table[index] = newNode;
                    
                    elementCount++;
                    readingStudent = false;
                }
                continue;
            }
            
            if (line.find("Student ID   : ") != string::npos) {
                readingStudent = true;
                currentStudent = Student(); 
                coursesRead = 0;
                
                string idStr = line.substr(15);
                currentStudent.studentID = stoi(idStr);
            }
            else if (line.find("Name         : ") != string::npos && readingStudent) {
                currentStudent.studentName = line.substr(15);
            }
            else if (line.find("Department   : ") != string::npos && readingStudent) {
                currentStudent.department = line.substr(15);
            }
            else if (line.find("Level        : ") != string::npos && readingStudent) {
                string levelStr = line.substr(15);
                currentStudent.level = stoi(levelStr);
            }
            else if (line.find("GPA (5.0)    : ") != string::npos && readingStudent) {
                string gpaStr = line.substr(15);
                currentStudent.gpa = stod(gpaStr);
            }
            else if (line.find("Courses      : ") != string::npos && readingStudent) {
                string coursesStr = line.substr(15);
                
                if (coursesStr != "N/A") {
                    size_t start = 0;
                    size_t end = 0;
                    
                    while (end != string::npos && coursesRead < MAX_COURSES) {
                        end = coursesStr.find(',', start);
                        string courseToken;
                        
                        if (end == string::npos) {
                            courseToken = coursesStr.substr(start);
                        } else {
                            courseToken = coursesStr.substr(start, end - start);
                            start = end + 2; 
                        }
                        
                        size_t gradeStart = courseToken.find('(');
                        size_t gradeEnd = courseToken.find(')');
                        
                        if (gradeStart != string::npos && gradeEnd != string::npos) {
                            string courseName = courseToken.substr(0, gradeStart - 1);
                            string gradeStr = courseToken.substr(gradeStart + 1, gradeEnd - gradeStart - 2); // Remove %)
                            
                            currentStudent.courseNames[coursesRead] = courseName;
                            currentStudent.courseGrades[coursesRead] = stod(gradeStr);
                            coursesRead++;
                        }
                    }
                    currentStudent.numCourses = coursesRead;
                } else {
                    currentStudent.numCourses = 0;
                }
            }
        }
        
        file.close();
        cout << "Data loaded from " << filename << " successfully! Loaded " << elementCount << " students.\n";
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != NULL) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};

void handleUpdateMenu(Student* student) {
    if (student == NULL) return;

    int choice;
    bool updating = true;

    while (updating) {
        cout << "\n========== UPDATE STUDENT: " << student->studentName << " ==========\n";
        cout << "1. Update Name\n";
        cout << "2. Update Department\n";
        cout << "3. Update Level\n";
        cout << "4. Add Course\n";
        cout << "5. Remove Course\n";
        cout << "6. Return to Main Menu\n";
        cout << "Enter choice: ";
        
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!\n";
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            cout << "Enter new name: ";
            getline(cin, student->studentName);
            cout << "Name updated!\n";
        }
        else if (choice == 2) {
            string dept;
            cout << "Enter new department (IT/CS/CE): ";
            getline(cin, dept);
            if (dept == "IT" || dept == "CS" || dept == "CE") {
                student->department = dept;
                cout << "Department updated!\n";
            } else {
                cout << "Invalid department!\n";
            }
        }
        else if (choice == 3) {
            int level;
            cout << "Enter new level (1-10): ";
            cin >> level;
            if (cin.fail() || level < 1 || level > 10) {
                cin.clear();
                cout << "Invalid level!\n";
            } else {
                student->level = level;
                cout << "Level updated!\n";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (choice == 4) {
            if (student->numCourses >= MAX_COURSES) {
                cout << "Error: Student already has the maximum number of courses (" << MAX_COURSES << ").\n";
            } else {
                string courseName;
                double grade;
                
                cout << "Enter course name: ";
                getline(cin, courseName);
                
                cout << "Enter grade (0-100): ";
                cin >> grade;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if(cin.fail()) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid grade!\n";
                }
                else if (student->addCourse(courseName, grade)) {
                    cout << "Course added and GPA updated!\n";
                }
            }
        }
        else if (choice == 5) {
            if (student->numCourses == 0) {
                cout << "Student has no courses to remove.\n";
                continue;
            }
            
            string courseName;
            cout << "Enter course name to remove: ";
            getline(cin, courseName);
            
            bool found = false;
            int foundIndex = -1;
            for (int i = 0; i < student->numCourses; i++) {
                if (student->courseNames[i] == courseName) {
                    found = true;
                    foundIndex = i;
                    break;
                }
            }
            
            if (found) {
                for (int i = foundIndex; i < student->numCourses - 1; i++) {
                    student->courseNames[i] = student->courseNames[i + 1];
                    student->courseGrades[i] = student->courseGrades[i + 1];
                }
                student->numCourses--;
                student->calculateGPA();
                cout << "Course removed and GPA updated!\n";
            } else {
                cout << "Course not found!\n";
            }
        }
        else if (choice == 6) {
            updating = false;
        }
        else {
            cout << "Invalid choice!\n";
        }
    }
}

void displayMenu() {
    cout << "\n========== STUDENT RECORD SYSTEM ==========\n";
    cout << "--- (Add, Update, Delete) ---\n";
    cout << " 1. Add Student\n";
    cout << " 2. Update Student\n";
    cout << " 3. Delete Student\n";
    cout << "--- (Find & Search) ---\n";
    cout << " 4. Find Student by ID\n";
    cout << " 5. Find Students by Level\n";
    cout << " 6. Find Students by Department\n";
    cout << " 7. Find Students by Course\n";
    cout << "--- (Sort & View All) ---\n";
    cout << " 8. View All Students\n";
    cout << " 9. Sort Students by GPA\n";
    cout << "10. Sort Students by Name\n";
    cout << "--- (Statistics & Advanced) ---\n";
    cout << "11. Display Student Statistics\n";
    cout << "12. Display Hash Table Statistics\n";
    cout << "13. Reverse All Students (Array Example)\n";
    cout << "14. Count Students (Recursive)\n";
    cout << "15. Display All (Recursive)\n";
    cout << "--- (System) ---\n";
    cout << "16. Save and Exit\n"; 
    cout << "Enter choice: ";
}

int main() {
    HashTable studentDB;
    
    studentDB.loadFromFile("students.txt");
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Invalid input! Please enter a number.\n";
            continue; 
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

       
        switch (choice) {
            case 1: {
                int id, level, numCourses;
                string name, dept;
                
                
                string tempCourses[MAX_COURSES];
                double tempGrades[MAX_COURSES];
                
                cout << "Enter Student ID: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                cout << "Enter Name: ";
                getline(cin, name);
                
                cout << "Enter Department (IT/CS/CE): ";
                getline(cin, dept);
                
                cout << "Enter Level (1-10): ";
                cin >> level;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                cout << "Enter number of courses (0-" << MAX_COURSES << "): ";
                cin >> numCourses;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (numCourses > MAX_COURSES || numCourses < 0) {
                    cout << "Invalid number of courses. Setting to 0.\n";
                    numCourses = 0;
                }

                for (int i = 0; i < numCourses; i++) {
                    cout << "Course " << (i + 1) << " name: ";
                    getline(cin, tempCourses[i]);
                    
                    cout << "Grade (0-100): ";
                    cin >> tempGrades[i];
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    if (cin.fail() || tempGrades[i] < 0 || tempGrades[i] > 100) {
                        cout << "Invalid grade! Try again.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        i--; 
                        continue;
                    }
                }
                
                if (studentDB.addStudent(id, name, dept, level, tempCourses, tempGrades, numCourses)) {
                    cout << "Student added successfully!\n";
                }
                break;
            }
            case 2: { 
                int id;
                cout << "Enter Student ID to update: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                Student* found = studentDB.findStudent(id);
                if (found != NULL) {
                    handleUpdateMenu(found);
                } else {
                    cout << "Student not found!\n";
                }
                break;
            }
            case 3: { 
                int id;
                cout << "Enter Student ID to delete: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                studentDB.deleteStudent(id);
                break;
            }
            case 4: { 
                int id;
                cout << "Enter Student ID: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                Student* found = studentDB.findStudent(id);
                if (found != NULL) {
                    cout << "\n========== STUDENT FOUND ==========\n";
                    studentDB.displayStudentInfo(*found);
                } else {
                    cout << "Student not found!\n";
                }
                break;
            }
            case 5: { 
                int level;
                cout << "Enter level (1-10): ";
                cin >> level;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                studentDB.findStudentsByLevel(level);
                break;
            }
            case 6: { 
                string dept;
                cout << "Enter department (IT/CS/CE): ";
                getline(cin, dept);
                studentDB.findStudentsByDepartment(dept);
                break;
            }
            case 7: { 
                string courseName;
                cout << "Enter course name: ";
                getline(cin, courseName);
                studentDB.findStudentsByCourse(courseName);
                break;
            }
            case 8: 
                studentDB.viewAllStudents();
                break;
            
            case 9: 
                studentDB.sortStudentsByGPA();
                break;
            
            case 10: 
                studentDB.sortStudentsByName();
                break;
            
            case 11: 
                studentDB.displayStudentStatistics();
                break;
            
            case 12:
                studentDB.displayHashTableStatistics();
                break;

            case 13: 
                studentDB.reverseStudentsArray();
                break;

            case 14: { 
                int count = studentDB.countStudents();
                cout << "\nTotal students (counted recursively): " << count << "\n";
                break;
            }
            case 15: 
                studentDB.displayRecursiveAll();
                break;
            
            
            case 16: 
                studentDB.saveToFile("students.txt");
                running = false;
                cout << "Goodbye!\n";
                break;
            
            default:
                cout << "Invalid choice! Please try again.\n";
                break;
        }
    }
    
    return 0;
}