#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <map>

const int TABLE_SIZE = 100;

struct Student {
    int studentID;
    std::string studentName;
    std::string level; 
    std::vector<std::pair<std::string, double>> courses; 
    double gpa;

    void calculateGPA() {
        if (courses.empty()) {
            gpa = 0.0;
            return;
        }
        
        double total = 0.0;
        for (const auto& course : courses) {
            total += course.second;
        }
        gpa = total / courses.size();
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

    void addStudent(int id, std::string name, std::string level, 
                   const std::vector<std::pair<std::string, double>>& courses) {
        int index = hashFunction(id);
        
        Student newStudent;
        newStudent.studentID = id;
        newStudent.studentName = name;
        newStudent.level = level;
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
        std::cout << "Student added successfully!\n";
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
        
        std::cout << "\n=== ALL STUDENTS ===\n";
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
        std::cout << "ID: " << student.studentID 
                  << ", Name: " << student.studentName 
                  << ", Level: " << student.level
                  << ", GPA: " << std::fixed << std::setprecision(2) << student.gpa
                  << "\nCourses: ";
        
        for (const auto& course : student.courses) {
            std::cout << course.first << "(" << course.second << ") ";
        }
        std::cout << "\n" << std::endl;
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
        
        for (size_t i = 0; i < students.size() - 1; i++) {
            for (size_t j = 0; j < students.size() - i - 1; j++) {
                if (students[j].gpa < students[j + 1].gpa) {
                    std::swap(students[j], students[j + 1]);
                }
            }
        }
        
        std::cout << "\n=== STUDENTS SORTED BY GPA (DESCENDING) ===\n";
        for (const auto& student : students) {
            displayStudentInfo(student);
        }
    }

    void sortStudentsByName() {
        auto students = getAllStudents();
        
        for (size_t i = 0; i < students.size() - 1; i++) {
            size_t minIndex = i;
            for (size_t j = i + 1; j < students.size(); j++) {
                if (students[j].studentName < students[minIndex].studentName) {
                    minIndex = j;
                }
            }
            if (minIndex != i) {
                std::swap(students[i], students[minIndex]);
            }
        }
        
        std::cout << "\n=== STUDENTS SORTED BY NAME (ASCENDING) ===\n";
        for (const auto& student : students) {
            displayStudentInfo(student);
        }
    }

    void findStudentsByLevel(const std::string& level) {
        bool found = false;
        
        std::cout << "\n=== STUDENTS IN LEVEL: " << level << " ===\n";
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

    void displayStatistics() {
        std::cout << "\n=== HASH TABLE STATISTICS ===\n";
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
    }

    
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "Error opening file for writing!\n";
            return;
        }
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                
                file << current->data.studentID << "\n";
                file << current->data.studentName << "\n";
                file << current->data.level << "\n";
                file << current->data.gpa << "\n";
                
               
                file << current->data.courses.size() << "\n";
                for (const auto& course : current->data.courses) {
                    file << course.first << "\n" << course.second << "\n";
                }
                
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
        
        int id;
        std::string name, level;
        double gpa;
        int courseCount;
        
        while (file >> id) {
            file.ignore(); 
            std::getline(file, name);
            std::getline(file, level);
            file >> gpa;
            file >> courseCount;
            file.ignore(); 
            
            std::vector<std::pair<std::string, double>> courses;
            for (int i = 0; i < courseCount; i++) {
                std::string courseName;
                double grade;
                std::getline(file, courseName);
                file >> grade;
                file.ignore(); 
                courses.push_back({courseName, grade});
            }
            
            addStudent(id, name, level, courses);
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

void displayMenu() {
    std::cout << "\n=== ENHANCED STUDENT RECORD SYSTEM ===\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Find Student (by ID)\n";
    std::cout << "3. Delete Student (by ID)\n";
    std::cout << "4. View All Students\n";
    std::cout << "5. Sort Students by GPA\n";
    std::cout << "6. Sort Students by Name\n";
    std::cout << "7. Find Students by Level\n";
    std::cout << "8. Display Hash Table Statistics\n";
    std::cout << "9. Save and Exit\n";
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
        
        switch (choice) {
            case 1: {
                int id;
                std::string name, level;
                std::vector<std::pair<std::string, double>> courses;
                
                std::cout << "Enter Student ID: ";
                std::cin >> id;
                std::cin.ignore();
                
                std::cout << "Enter Student Name: ";
                std::getline(std::cin, name);
                
                std::cout << "Enter Student Level (Freshman/Sophomore/Junior/Senior): ";
                std::getline(std::cin, level);
                
                std::cout << "Enter number of courses: ";
                int numCourses;
                std::cin >> numCourses;
                std::cin.ignore();
                
                for (int i = 0; i < numCourses; i++) {
                    std::string courseName;
                    double grade;
                    
                    std::cout << "Enter course " << (i + 1) << " name: ";
                    std::getline(std::cin, courseName);
                    
                    std::cout << "Enter grade for " << courseName << ": ";
                    std::cin >> grade;
                    std::cin.ignore();
                    
                    courses.push_back({courseName, grade});
                }
                
                studentDB.addStudent(id, name, level, courses);
                break;
            }
            
            case 2: {
                int id;
                std::cout << "Enter Student ID to find: ";
                std::cin >> id;
                
                Student* found = studentDB.findStudent(id);
                if (found != nullptr) {
                    std::cout << "Student Found!\n";
                    studentDB.displayStudentInfo(*found);
                } else {
                    std::cout << "Student not found!\n";
                }
                break;
            }
            
            case 3: {
                int id;
                std::cout << "Enter Student ID to delete: ";
                std::cin >> id;
                
                studentDB.deleteStudent(id);
                break;
            }
            
            case 4: {
                studentDB.viewAllStudents();
                break;
            }
            
            case 5: {
                studentDB.sortStudentsByGPA();
                break;
            }
            
            case 6: {
                studentDB.sortStudentsByName();
                break;
            }
            
            case 7: {
                std::string level;
                std::cout << "Enter level to search (Freshman/Sophomore/Junior/Senior): ";
                std::cin.ignore();
                std::getline(std::cin, level);
                
                studentDB.findStudentsByLevel(level);
                break;
            }
            
            case 8: {
                studentDB.displayStatistics();
                break;
            }
            
            case 9: {
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