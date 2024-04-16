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
};

// Структура для хранения данных студента
struct Student {
    Code code;
    FullName fullName;
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
    return (a.code.letter < b.code.letter) ||
           (a.code.letter == b.code.letter && a.code.numbers < b.code.numbers) ||
           (a.code.letter == b.code.letter && a.code.numbers == b.code.numbers && a.fullName.surname < b.fullName.surname) ||
           (a.code.letter == b.code.letter && a.code.numbers == b.code.numbers && a.fullName.surname == b.fullName.surname && a.fullName.name < b.fullName.name) ||
           (a.code.letter == b.code.letter && a.code.numbers == b.code.numbers && a.fullName.surname == b.fullName.surname && a.fullName.name == b.fullName.name && a.fullName.patronymic < b.fullName.patronymic);
}

bool operator>(const Student& a, const Student& b) {
    return (a.code.letter > b.code.letter) ||
           (a.code.letter == b.code.letter && a.code.numbers > b.code.numbers) ||
           (a.code.letter == b.code.letter && a.code.numbers == b.code.numbers && a.fullName.surname > b.fullName.surname) ||
           (a.code.letter == b.code.letter && a.code.numbers == b.code.numbers && a.fullName.surname == b.fullName.surname && a.fullName.name > b.fullName.name) ||
           (a.code.letter == b.code.letter && a.code.numbers == b.code.numbers && a.fullName.surname == b.fullName.surname && a.fullName.name == b.fullName.name && a.fullName.patronymic > b.fullName.patronymic);
}

// Перегрузка оператора вывода для Code
ostream& operator<<(ostream& os, const Code& code) {
    os << code.letter << code.numbers;
    return os;
}

// Перегрузка оператора вывода для Student
ostream& operator<<(ostream& os, const Student& student) {
    os << student.code.letter << student.code.numbers << " " << student.fullName.surname << " " << student.fullName.name << " " << student.fullName.patronymic;
    return os;
}

// Генерация рандомного имени студента
FullName generateFullName() {
    string names[] = {"Artem", "Ekaterina", "Maxim", "Alexander", "Dmitry", "Nikita", "Egor", "Mikhail", "Roman", "Vladimir", "Pavel", "Daniel", "Anton", "Alexey", "Sergei"};
    string surnames[] = {"Polyakov", "Krushinina", "Johnson", "Brown", "Taylor", "Wilson", "Harris", "Clark", "White", "Anderson", "Allen", "Davis", "Martin", "Hall", "Lee"};
    string patronymics[] = {"Victorovich", "Vladislavovna", "Ivanovich", "Petrovich", "Sidorovich", "Mikhailovich", "Fedorovich", "Egorovich", "Romanovich", "Vladimirovich", "Pavlovich", "Daniilovich", "Antonovich", "Alexandrovich", "Sergeevich"};


    FullName fullName;
    fullName.surname = surnames[rand() % 15];
    fullName.name = names[rand() % 15];
    fullName.patronymic = patronymics[rand() % 15];

    return fullName;
}

// Генератор ключей для входного файла
void generateKyesFile() {
    ofstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Sorting/KeysForSort.txt");

    for (int i = 0; i < 1000; ++i) {
        Student student;
        student.code = generateCode();
        student.fullName = generateFullName();

        inFile << student.code << " " << student.fullName.surname << " "
                << student.fullName.name << " " << student.fullName.patronymic << endl;
    }
    inFile.close();
    cout << "Ключи сгенерированы\n" << endl;
}

// Функция для чтения данных студентов из файла
vector<Student> readKeyFromFile() {
    ifstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Sorting/KeysForSort.txt");
    vector<Student> students;
    int lineNumber = 1;

    while (!inFile.eof()) {
        Student student;
        inFile >> student.code.letter >> student.code.numbers;
        inFile >> student.fullName.surname >> student.fullName.name >> student.fullName.patronymic;

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
        cout << student.code << " " << student.fullName.surname << " "
             << student.fullName.name << " " << student.fullName.patronymic << endl;
    }
}

// Функция сортировки вставками
void insertionSort(vector<Student>& students) {
    for (size_t i = 1; i < students.size(); i++) {
        Student key = students[i];
        int j = i - 1;

        while (j >= 0 && students[j] < key) {
            students[j + 1] = students[j];
            j = j - 1;
        }
        students[j + 1] = key;
    }
}


void printInsertionSortedVectorToFile(vector<Student> students, chrono::duration<double> insertionSortTime, ofstream& outFile) {
    for (int i = 0; i < size(students); ++i) {
        outFile << students[i] << " " << students[i].lineNumber << endl;
    }

    outFile << insertionSortTime.count() << " секунд" << endl;
    cout << "Отсортированные вставками элементы выведены в файл\n" << endl;
    
}

int partitioning(vector<Student>& students, int low, int high) {
    Student supporting = students[high]; // Опорный элемент
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        // Если текущий элемент меньше или равен опорному
        if (students[j] > supporting) {
            i++; // увеличиваем индекс меньшего элемента
            auto temp = students[i];
            students[i] = students[j];
            students[j] = temp;
        }
    }
    swap(students[i + 1], students[high]);
    return (i + 1);
}

void quickSort(vector<Student>& students, int low, int high) {
    if (low < high) {
        int sup = partitioning(students, low, high);

        quickSort(students, low, sup - 1);
        quickSort(students, sup + 1, high);
    }
}



void printQuickSortedVectorToFile(vector<Student> students, ofstream& outFile) {

    for (int i = 0; i < size(students); ++i) {
        outFile << students[i] << " " << students[i].lineNumber << endl;
    }
    
    cout << "Отсортированные быстрой рекурсивной реализацией элементы выведены в файл\n" << endl;
}

int main() {
    srand(time(nullptr));
    vector<Student> students;

    //generateKyesFile();

    students = readKeyFromFile();

    ofstream insertionFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Sorting/InsertionSort.txt");
    // Сортировка вставками
    auto start = chrono::high_resolution_clock::now();
    vector<Student> forInsertionSort = students;
    insertionSort(forInsertionSort);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> insertionSortTime = end - start;
    cout << "Время сортировки вставками: " << insertionSortTime.count() << " секунд" << endl;
    printInsertionSortedVectorToFile(forInsertionSort, insertionSortTime, insertionFile);
    insertionFile.close();

    ofstream quickFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/Sorting/QuickSort.txt");
    // Быстрая рекурсивная реализация
    start = chrono::high_resolution_clock::now();
    vector<Student> forQuickSort = students;
    quickSort(forQuickSort, 0, forQuickSort.size() - 1);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> quickSortTime = end - start;
    cout << "Время быстрой сортировки: " << quickSortTime.count() << " секунд" << endl;
    printQuickSortedVectorToFile(forQuickSort, quickFile);

    quickFile << quickSortTime.count() << " секунд" << endl;
    quickFile.close();

    return 0;
}
