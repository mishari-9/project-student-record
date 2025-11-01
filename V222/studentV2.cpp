#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>
#include <numeric>
#include <cstdlib>

const int TABLE_SIZE = 100;

struct Student {
    int studentID;
    std::string studentName;
    std::string department;
    int level;
    std::vector<std::pair<std::string, double>> courses;
    double gpa;

    void calculateGPA() {
        if (courses.empty()) {
            gpa = 0.0;
            return;
        }
        
        double totalGradePoints = 0.0;
        for (const auto& course : courses) {
            double percentage = course.second;
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
        
        gpa = totalGradePoints / courses.size();
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

public:
    HashTable() : elementCount(0) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    int hashFunction(int studentID) {
        return studentID % TABLE_SIZE;
    }

    bool addStudent(int id, std::string name, std::string dept, int lvl, 
                   const std::vector<std::pair<std::string, double>>& courses) {
        if (findStudent(id) != nullptr) {
            std::cout << "Error: Student with ID " << id << " already exists.\n";
            return false;
        }
        
        if (lvl < 1 || lvl > 10) {
            std::cout << "Error: Level must be between 1 and 10.\n";
            return false;
        }
        
        if (dept != "IT" && dept != "CS" && dept != "CE") {
            std::cout << "Error: Department must be IT, CS, or CE.\n";
            return false;
        }
        
        for (const auto& course : courses) {
            if (course.second < 0 || course.second > 100) {
                std::cout << "Error: Grade for " << course.first << " must be between 0 and 100.\n";
                return false;
            }
        }
        
        int index = hashFunction(id);
        
        Student newStudent;
        newStudent.studentID = id;
        newStudent.studentName = name;
        newStudent.department = dept;
        newStudent.level = lvl;
        newStudent.courses = courses;
        newStudent.calculateGPA();
        
        Node* newNode = new Node;
        newNode->data = newStudent;
        newNode->next = nullptr;
        
        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            newNode->next = table[index];
            table[index] = newNode;
        }
        
        elementCount++;
        return true;
    }

    Student* findStudent(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        
        while (current != nullptr) {
            if (current->data.studentID == id) {
                return &(current->data);
            }
            current = current->next;
        }
        
        return nullptr;
    }

    void deleteStudent(int id) {
        int index = hashFunction(id);
        Node* current = table[index];
        Node* prev = nullptr;
        
        while (current != nullptr) {
            if (current->data.studentID == id) {
                if (prev == nullptr) {
                    table[index] = current->next;
                } else {
                    prev->next = current->next;
                }
                delete current;
                elementCount--;
                std::cout << "Student deleted successfully!\n";
                return;
            }
            prev = current;
            current = current->next;
        }
        
        std::cout << "Student not found!\n";
    }

    void viewAllStudents() {
        bool found = false;
        
        std::cout << "\n================================ ALL STUDENTS ================================\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                found = true;
                displayStudentInfo(current->data);
                current = current->next;
            }
        }
        
        if (!found) {
            std::cout << "No students found!\n";
        }
    }

    void displayStudentInfo(const Student& student) {
        std::cout << "==============================================================================\n";
        std::cout << "Student ID    : " << student.studentID << std::endl;
        std::cout << "Student Name  : " << student.studentName << std::endl;
        std::cout << "Department    : " << student.department << std::endl;
        std::cout << "Level         : " << student.level << std::endl;
        std::cout << "GPA (5.0)     : " << std::fixed << std::setprecision(2) << student.gpa << std::endl;
        std::cout << "Courses       : ";
        
        if (student.courses.empty()) {
            std::cout << "N/A";
        } else {
            for (size_t i = 0; i < student.courses.size(); i++) {
                std::cout << student.courses[i].first << " (" << std::fixed << std::setprecision(1) << student.courses[i].second << "%)";
                if (i < student.courses.size() - 1) std::cout << ", ";
            }
        }
        std::cout << "\n==============================================================================\n" << std::endl;
    }

    std::vector<Student> getAllStudents() {
        std::vector<Student> students;
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                students.push_back(current->data);
                current = current->next;
            }
        }
        
        return students;
    }

    void sortStudentsByGPA() {
        auto students = getAllStudents();
        
        if (students.empty()) {
            std::cout << "No students to sort.\n";
            return;
        }

        std::sort(students.begin(), students.end(), 
            [](const Student& a, const Student& b) {
                return a.gpa > b.gpa;
            });
        
        std::cout << "\n======================== STUDENTS SORTED BY GPA (DESCENDING) ========================\n";
        for (const auto& student : students) {
            displayStudentInfo(student);
        }
    }

    void sortStudentsByName() {
        auto students = getAllStudents();

        if (students.empty()) {
            std::cout << "No students to sort.\n";
            return;
        }

        std::sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) {
                return a.studentName < b.studentName;
            });
        
        std::cout << "\n======================== STUDENTS SORTED BY NAME (ASCENDING) ========================\n";
        for (const auto& student : students) {
            displayStudentInfo(student);
        }
    }

    void findStudentsByLevel(int level) {
        bool found = false;
        
        if (level < 1 || level > 10) {
            std::cout << "Error: Level must be between 1 and 10.\n";
            return;
        }
        
        std::cout << "\n======================== STUDENTS IN LEVEL: " << level << " ========================\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                if (current->data.level == level) {
                    found = true;
                    displayStudentInfo(current->data);
                }
                current = current->next;
            }
        }
        
        if (!found) {
            std::cout << "No students found in level: " << level << "\n";
        }
    }

    void findStudentsByDepartment(const std::string& dept) {
        bool found = false;
        
        if (dept != "IT" && dept != "CS" && dept != "CE") {
            std::cout << "Error: Department must be IT, CS, or CE.\n";
            return;
        }
        
        std::cout << "\n======================== STUDENTS IN DEPARTMENT: " << dept << " ========================\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                if (current->data.department == dept) {
                    found = true;
                    displayStudentInfo(current->data);
                }
                current = current->next;
            }
        }
        
        if (!found) {
            std::cout << "No students found in department: " << dept << "\n";
        }
    }

    void findStudentsByCourse(const std::string& courseName) {
        bool found = false;
        
        std::cout << "\n======================== STUDENTS TAKING COURSE: " << courseName << " ========================\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                bool isEnrolled = false;
                for (const auto& course : current->data.courses) {
                    if (course.first == courseName) {
                        isEnrolled = true;
                        break;
                    }
                }

                if (isEnrolled) {
                    found = true;
                    displayStudentInfo(current->data);
                }
                current = current->next;
            }
        }
        
        if (!found) {
            std::cout << "No students found taking course: " << courseName << "\n";
        }
    }

    void displayStudentStatistics() {
        auto students = getAllStudents();
        
        if (students.empty()) {
            std::cout << "No student data available to analyze.\n";
            return;
        }

        std::cout << "\n================================ STUDENT STATISTICS ================================\n";
        std::cout << "Total Students: " << students.size() << std::endl;

        double totalGPA = 0.0;
        std::map<std::string, int> deptCounts;
        std::map<std::string, double> deptGpaSums;
        std::map<int, int> levelCounts;
        std::map<int, double> levelGpaSums;

        for (const auto& student : students) {
            totalGPA += student.gpa;
            deptCounts[student.department]++;
            deptGpaSums[student.department] += student.gpa;
            levelCounts[student.level]++;
            levelGpaSums[student.level] += student.gpa;
        }

        std::cout << "Overall Average GPA: " << std::fixed << std::setprecision(2)
                  << (totalGPA / students.size()) << "/5.0" << std::endl;
        
        std::cout << "\n---------------------------- Statistics by Department ----------------------------\n";
        for (const auto& pair : deptCounts) {
            std::string dept = pair.first;
            int count = pair.second;
            double avgGpa = (count > 0) ? (deptGpaSums[dept] / count) : 0.0;
            
            std::cout << dept << ": " << count << " student(s), "
                      << "Average GPA: " << std::fixed << std::setprecision(2) << avgGpa << "/5.0" << std::endl;
        }
        
        std::cout << "\n----------------------------- Statistics by Level -----------------------------\n";
        for (const auto& pair : levelCounts) {
            int level = pair.first;
            int count = pair.second;
            double avgGpa = (count > 0) ? (levelGpaSums[level] / count) : 0.0;
            
            std::cout << "Level " << level << ": " << count << " student(s), "
                      << "Average GPA: " << std::fixed << std::setprecision(2) << avgGpa << "/5.0" << std::endl;
        }
        std::cout << "==============================================================================\n";
    }

    void displayHashTableStatistics() {
        std::cout << "\n============================== HASH TABLE STATISTICS ==============================\n";
        std::cout << "Total Students: " << elementCount << std::endl;
        std::cout << "Table Size: " << TABLE_SIZE << std::endl;
        std::cout << "Load Factor: " << std::fixed << std::setprecision(2) 
                  << (double)elementCount / TABLE_SIZE << std::endl;
        
        int collisions = 0;
        int emptyBuckets = 0;
        int longestChain = 0;
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            int chainLength = 0;
            Node* current = table[i];
            
            while (current != nullptr) {
                chainLength++;
                current = current->next;
            }
            
            if (chainLength > 1) collisions += (chainLength - 1);
            if (chainLength == 0) emptyBuckets++;
            if (chainLength > longestChain) longestChain = chainLength;
        }
        
        std::cout << "Total Collisions: " << collisions << std::endl;
        std::cout << "Empty Buckets: " << emptyBuckets << std::endl;
        std::cout << "Longest Chain: " << longestChain << std::endl;
        std::cout << "==============================================================================\n";
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "Error opening file for writing!\n";
            return;
        }
        
        file << "==============================================================================\n";
        file << "                            STUDENT DATABASE RECORDS\n";
        file << "==============================================================================\n\n";
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                file << "Student ID    : " << current->data.studentID << "\n";
                file << "Student Name  : " << current->data.studentName << "\n";
                file << "Department    : " << current->data.department << "\n";
                file << "Level         : " << current->data.level << "\n";
                file << "GPA (5.0)     : " << std::fixed << std::setprecision(2) << current->data.gpa << "\n";
                file << "Courses       : ";
                
                if (current->data.courses.empty()) {
                    file << "N/A";
                } else {
                    for (size_t j = 0; j < current->data.courses.size(); j++) {
                        file << current->data.courses[j].first << " (" << std::fixed << std::setprecision(1) << current->data.courses[j].second << "%)";
                        if (j < current->data.courses.size() - 1) file << ", ";
                    }
                }
                file << "\n";
                file << "------------------------------------------------------------------------------\n";
                current = current->next;
            }
        }
        
        file.close();
        std::cout << "Data saved to file successfully!\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "No previous data found. Starting fresh.\n";
            return;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.find("Student ID") != std::string::npos) {
                int id, level;
                std::string name, dept;
                std::vector<std::pair<std::string, double>> courses;
                
                id = std::stoi(line.substr(15));
                
                std::getline(file, line);
                name = line.substr(15);
                
                std::getline(file, line);
                dept = line.substr(15);
                
                std::getline(file, line);
                level = std::stoi(line.substr(15));
                
                std::getline(file, line);
                std::getline(file, line);
                
                std::string coursesLine = line.substr(15);
                if (coursesLine != "N/A") {
                    size_t pos = 0;
                    while (pos < coursesLine.length()) {
                        size_t courseEnd = coursesLine.find('(', pos);
                        if (courseEnd == std::string::npos) break;
                        
                        std::string courseName = coursesLine.substr(pos, courseEnd - pos - 1);
                        
                        size_t gradeStart = courseEnd + 1;
                        size_t gradeEnd = coursesLine.find('%', gradeStart);
                        double grade = std::stod(coursesLine.substr(gradeStart, gradeEnd - gradeStart));
                        
                        courses.push_back({courseName, grade});
                        
                        pos = gradeEnd + 3;
                        if (pos >= coursesLine.length()) break;
                    }
                }
                
                std::getline(file, line);
                
                addStudent(id, name, dept, level, courses);
            }
        }
        
        file.close();
        std::cout << "Data loaded from file successfully!\n";
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
    }
};

void handleUpdateMenu(Student* student, HashTable& studentDB) {
    if (!student) return;

    int choice;
    bool updating = true;
    std::string tempStr;
    double tempGrade;
    int tempLevel;

    while (updating) {
        std::cout << "\n======================== UPDATING STUDENT: " << student->studentName << " (ID: " << student->studentID << ") ========================\n";
        std::cout << "1. Update Name\n";
        std::cout << "2. Update Department\n";
        std::cout << "3. Update Level\n";
        std::cout << "4. Add Course\n";
        std::cout << "5. Remove Course\n";
        std::cout << "6. Return to Main Menu\n";
        std::cout << "Enter your choice: ";
        
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(10000, '\n'); 
            std::cout << "Invalid input. Please enter a number.\n";
            continue; 
        }
        
        std::cin.ignore(10000, '\n'); 

        switch (choice) {
            case 1:
                std::cout << "Enter new name: ";
                std::getline(std::cin, student->studentName);
                std::cout << "Name updated successfully!\n";
                break;
            case 2:
                std::cout << "Enter new department (IT/CS/CE): ";
                std::getline(std::cin, tempStr);
                if (tempStr == "IT" || tempStr == "CS" || tempStr == "CE") {
                    student->department = tempStr;
                    std::cout << "Department updated successfully!\n";
                } else {
                    std::cout << "Invalid department. Must be IT, CS, or CE.\n";
                }
                break;
            case 3:
                std::cout << "Enter new level (1-10): ";
                std::cin >> tempLevel;
                if (tempLevel >= 1 && tempLevel <= 10) {
                    student->level = tempLevel;
                    std::cout << "Level updated successfully!\n";
                } else {
                    std::cout << "Invalid level. Must be between 1 and 10.\n";
                }
                std::cin.ignore(10000, '\n');
                break;
            case 4:
                std::cout << "Enter course name to add: ";
                std::getline(std::cin, tempStr);
                
                std::cout << "Enter grade for " << tempStr << " (0-100): ";
                std::cin >> tempGrade;
                if (tempGrade < 0 || tempGrade > 100) {
                    std::cout << "Error: Grade must be between 0 and 100.\n";
                } else {
                    student->courses.push_back({tempStr, tempGrade});
                    student->calculateGPA(); 
                    std::cout << "Course added and GPA updated successfully!\n";
                }
                std::cin.ignore(10000, '\n');
                break;
            
            case 5: {
                std::cout << "Enter course name to remove: ";
                std::getline(std::cin, tempStr);
                
                bool found = false;
                for (auto it = student->courses.begin(); it != student->courses.end(); ++it) {
                    if (it->first == tempStr) {
                        student->courses.erase(it);
                        found = true;
                        break;
                    }
                }
                
                if (found) {
                    student->calculateGPA(); 
                    std::cout << "Course removed and GPA updated successfully!\n";
                } else {
                    std::cout << "Course not found.\n";
                }
                break;
            } 

            case 6:
                updating = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void displayMenu() {
    std::cout << "\n======================== ENHANCED STUDENT RECORD SYSTEM ========================\n";
    std::cout << "1.  Add Student\n";
    std::cout << "2.  Find Student (by ID)\n";
    std::cout << "3.  Update Student (by ID)\n";
    std::cout << "4.  Delete Student (by ID)\n";
    std::cout << "5.  View All Students\n";
    std::cout << "6.  Sort Students by GPA\n";
    std::cout << "7.  Sort Students by Name\n";
    std::cout << "8.  Find Students by Level\n";
    std::cout << "9.  Find Students by Department\n";
    std::cout << "10. Find Students by Course\n";
    std::cout << "11. Display Student Statistics\n";
    std::cout << "12. Display Hash Table Statistics\n";
    std::cout << "13. Save and Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    HashTable studentDB;
    
    studentDB.loadFromFile("students.txt");
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(10000, '\n'); 
            std::cout << "Invalid input. Please enter a number.\n";
            continue; 
        }
        
        std::cin.ignore(10000, '\n'); 

        switch (choice) {
            case 1: {
                int id, level;
                std::string name, dept;
                std::vector<std::pair<std::string, double>> courses;
                
                std::cout << "Enter Student ID: ";
                std::cin >> id;
                std::cin.ignore();
                
                std::cout << "Enter Student Name: ";
                std::getline(std::cin, name);
                
                std::cout << "Enter Department (IT/CS/CE): ";
                std::getline(std::cin, dept);
                
                std::cout << "Enter Level (1-10): ";
                std::cin >> level;
                std::cin.ignore();
                
                std::cout << "Enter number of courses: ";
                int numCourses;
                std::cin >> numCourses;
                std::cin.ignore();
                
                for (int i = 0; i < numCourses; i++) {
                    std::string courseName;
                    double grade;
                    
                    std::cout << "Enter course " << (i + 1) << " name: ";
                    std::getline(std::cin, courseName);
                    
                    std::cout << "Enter grade for " << courseName << " (0-100): ";
                    std::cin >> grade;
                    if (grade < 0 || grade > 100) {
                        std::cout << "Error: Grade must be between 0 and 100. Please try again.\n";
                        i--;
                        std::cin.ignore();
                        continue;
                    }
                    std::cin.ignore();
                    
                    courses.push_back({courseName, grade});
                }
                
                if (studentDB.addStudent(id, name, dept, level, courses)) {
                    std::cout << "Student added successfully!\n";
                }
                break;
            }
            
            case 2: {
                int id;
                std::cout << "Enter Student ID to find: ";
                std::cin >> id;
                
                Student* found = studentDB.findStudent(id);
                if (found != nullptr) {
                    std::cout << "\n======================== STUDENT FOUND ========================\n";
                    studentDB.displayStudentInfo(*found);
                } else {
                    std::cout << "Student not found!\n";
                }
                break;
            }
            
            case 3: {
                int id;
                std::cout << "Enter Student ID to update: ";
                std::cin >> id;
                std::cin.ignore(10000, '\n'); 
                
                Student* found = studentDB.findStudent(id);
                if (found != nullptr) {
                    handleUpdateMenu(found, studentDB);
                } else {
                    std::cout << "Student not found!\n";
                }
                break;
            }
            
            case 4: {
                int id;
                std::cout << "Enter Student ID to delete: ";
                std::cin >> id;
                
                studentDB.deleteStudent(id);
                break;
            }
            
            case 5: {
                studentDB.viewAllStudents();
                break;
            }
            
            case 6: {
                studentDB.sortStudentsByGPA();
                break;
            }
            
            case 7: {
                studentDB.sortStudentsByName();
                break;
            }
            
            case 8: {
                int level;
                std::cout << "Enter level to search (1-10): ";
                std::cin >> level;
                std::cin.ignore();
                
                studentDB.findStudentsByLevel(level);
                break;
            }
            
            case 9: {
                std::string dept;
                std::cout << "Enter department to search (IT/CS/CE): ";
                std::getline(std::cin, dept);
                
                studentDB.findStudentsByDepartment(dept);
                break;
            }
            
            case 10: {
                std::string courseName;
                std::cout << "Enter course name to search for: ";
                std::getline(std::cin, courseName);
                
                studentDB.findStudentsByCourse(courseName);
                break;
            }
            
            case 11: {
                studentDB.displayStudentStatistics();
                break;
            }
            
            case 12: {
                studentDB.displayHashTableStatistics();
                break;
            }
            
            case 13: {
                studentDB.saveToFile("students.txt");
                running = false;
                std::cout << "Goodbye!\n";
                break;
            }
            
            default: {
                std::cout << "Invalid choice! Please try again.\n";
                break;
            }
        }
    }
    
    return 0;
}