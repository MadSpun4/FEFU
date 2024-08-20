#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>

using namespace std;

// Структура для хранения кода направления студента
struct Code {
    char letter;
    unsigned int numbers;
};

// Структура для хранения ФИО студента
struct FullName {
    string surname;
    string name;
    string patronymic;
    string FIO;
};

// Структура для хранения названия направления студента
struct CourseName {
    string name;
};

// Структура для хранения данных студента
struct Student {
    Code code;
    FullName fullName;
    CourseName name;
    int applicationNum;
    int lineNumber;
};

// Функция для генерации случайного кода направления
Code generateCode() {
    Code code;
    code.letter = 'B';
    code.numbers = 1000 + rand() % 9000;
    return code;
}

// Перегрузки оператора сравнения для Student
bool operator<(const Student& a, const Student& b) {
    return (a.applicationNum < b.applicationNum);
}

bool operator>(const Student& a, const Student& b) {
    return (a.applicationNum > b.applicationNum);
}

// Перегрузка оператора вывода для Code
ostream& operator<<(ostream& os, const Code& code) {
    os << code.letter << code.numbers;
    return os;
}

// Перегрузка оператора вывода для CourseName
ostream& operator<<(ostream& os, const CourseName& name) {
    os << name.name;
    return os;
}

// Перегрузка оператора вывода для Student
ostream& operator<<(ostream& os, const Student& student) {
    os << student.code.letter << student.code.numbers << " " << student.fullName.FIO << " " << student.name.name << " " << student.applicationNum;
    return os;
}

// Генерация рандомного имени студента
FullName generateFullName() {
    string names[] = {"Artem", "Ekaterina", "Maxim", "Alexander", "Dmitry", "Nikita", "Egor", "Mikhail", "Roman", "Vladimir", "Pavel", "Daniel", "Anton", "Alexey", "Sergei"};
    string surnames[] = {"Polyakov", "Krushinina", "Johnson", "Brown", "Taylor", "Wilson", "Harris", "Clark", "White", "Anderson", "Allen", "Davis", "Martin", "Hall", "Lee"};
    string patronymics[] = {"Victorovich", "Vladislavovna", "Ivanovich", "Petrovich", "Sidorovich", "Mikhailovich", "Fedorovich", "Egorovich", "Romanovich", "Vladimirovich", "Pavlovich", "Daniilovich", "Antonovich", "Alexandrovich", "Sergeevich"};

    FullName fullName;
    fullName.FIO = surnames[rand() % 15] + " " + names[rand() % 15] + " " + patronymics[rand() % 15];

    return fullName;
}

// Генерация рандомного названия курса
CourseName generateCourseName() {
    string names[] = {"progin", "economic", "menegment", "urfuck", "med", "arhitecture", "himbio", "fizvoz", "politech", "ped"};

    CourseName name;
    name.name = names[rand() % 10];

    return name;
}

// Генератор ключей для входного файла
void generateKyesFile() {
    ofstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Num_Search/inputFile.txt");

    for (int i = 0; i < 10000; ++i) {
        Student student;
        student.code = generateCode();
        student.fullName = generateFullName();
        student.name = generateCourseName();
        student.applicationNum = rand() % 10000;

        inFile << student.code << " " << student.fullName.FIO << " " << student.name << " " << student.applicationNum << endl;
    }
    inFile.close();
    cout << "Ключи сгенерированы\n" << endl;
}

// Функция для чтения данных студентов из файла
vector<Student> readKeyFromFile(int numOfLines) {
    ifstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Num_Search/inputFile.txt");
    vector<Student> students;
    int lineNumber = 1;
    int i = 1;
    while (i <= numOfLines) {
        Student student;
        inFile >> student.code.letter >> student.code.numbers;
        inFile >> student.fullName.surname >> student.fullName.name >> student.fullName.patronymic;
        student.fullName.FIO = student.fullName.surname + " " + student.fullName.name + " " + student.fullName.patronymic;
        inFile >> student.name.name;
        inFile >> student.applicationNum;
        i++;
        if (!inFile.fail()) {
            student.lineNumber = lineNumber++;
            students.push_back(student);
        }
    }

    inFile.close();
    return students;
}

void printStudentVector(vector<Student> students) {
    for (const auto& student : students) {
        cout << student.code << " " << student.fullName.FIO << " " << student.name.name << " " << student.applicationNum << endl;
    }
}

// Функция сортировки вставками
void insertionSort(vector<Student>& students) {
    for (size_t i = 1; i < students.size(); i++) {
        Student key = students[i];
        int j = i - 1;

        while (j >= 0 && students[j] > key) {
            students[j + 1] = students[j];
            j = j - 1;
        }
        students[j + 1] = key;
    }
}

// Линейный поиск с барьером
void linearSearchWithBarrier(vector<Student>& students, int key) {
    // Добавляем барьерный элемент в конец вектора
    Student barrierStudent;
    barrierStudent.applicationNum = key;
    students.push_back(barrierStudent);

    int i = 0;
    while (students[i].applicationNum != key)
        i++;

    // Удаляем барьерный элемент из вектора
    students.pop_back();

    if (i == students.size()) {
        cout << "Элемент не найден" << endl;
    } else {
        cout << i + 1 << " - итерраций," << " " << students[i].lineNumber << " - лайн намбер" << endl;
    }
}

// Однородный бинарный поиск с таблицей шагов
void binarySearchWithSteps(const vector<Student>& students, int key) {
    int n = students.size();
    int counter = 1;
    // Определение таблицы шагов
    vector<int> steps;
    int step = 1;
    while (step <= n) {
        steps.push_back(step);
        step *= 2;
    }

    // Поиск
    int low = 0, high = n - 1;
    for (int i = steps.size() - 1; i >= 0; --i) {
        int mid = low + steps[i] - 1;
        if (mid >= n) {
            mid = n - 1;
        }

        if (students[mid].applicationNum == key) {
            cout << counter << " - итерраций," << " " << students[mid].lineNumber << " - лайн намбер" << endl; // Элемент найден
            return;   
        } else if (students[mid].applicationNum < key) {
            low = mid + 1; // Поиск в правой половине
        } else {
            high = mid - 1; // Поиск в левой половине
        }

        counter++;
    }

    cout << "Элемент не найден" << endl; 
}

int main() {
    //generateKyesFile();

    vector<Student> students = readKeyFromFile(10000);
    //insertionSort(students);
    //printStudentVector(students);

    // Линейный с барьером
    int key = 5977;
    linearSearchWithBarrier(students, key);
    insertionSort(students);
    // Однородный бинарный с таблицей шагов
    binarySearchWithSteps(students, key);

    //printStudentVector(students);

    return 0;
}
