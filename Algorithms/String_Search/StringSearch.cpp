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
    int lineNumber;
};

// Функция для генерации случайного кода направления
Code generateCode() {
    Code code;
    code.letter = 'B';
    code.numbers = 1000 + rand() % 9000;
    return code;
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
    os << student.code.letter << student.code.numbers << " " << student.fullName.FIO << " " << student.name.name;
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
    ofstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/String_Search/InputFile.txt");

    for (int i = 0; i < 1000000; ++i) {
        Student student;
        student.code = generateCode();
        student.fullName = generateFullName();
        student.name = generateCourseName();

        inFile << student.code << " " << student.fullName.FIO << " " << student.name << endl;
    }
    inFile.close();
    cout << "Ключи сгенерированы\n" << endl;
}

// Функция для чтения данных студентов из файла
vector<Student> readKeyFromFile() {
    ifstream inFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/String_Search/InputFile.txt");
    vector<Student> students;
    int lineNumber = 1;

    while (!inFile.eof()) {
        Student student;
        inFile >> student.code.letter >> student.code.numbers;
        inFile >> student.fullName.surname >> student.fullName.name >> student.fullName.patronymic;
        student.fullName.FIO = student.fullName.surname + " " + student.fullName.name + " " + student.fullName.patronymic;
        inFile >> student.name.name;

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
        cout << student.code << " " << student.fullName.FIO << " " << student.name.name << endl;
    }
}


vector<Student> directedSearch(const vector<Student>& students, const string& fio, int fioOccurrences, const string& courseName, int courseNameOccurrences) {
    vector<Student> foundStudents;

    for (const auto& student : students) {
        // Поиск по ФИО
        const string& studentFIO = student.fullName.FIO;
        int fioOccurrencesFound = 0;

        for (size_t i = 0; i <= studentFIO.size() - fio.size(); ++i) {
            size_t j = 0;

            if (studentFIO.size() < fio.size()) {
                break;
            }
            
            while (j < fio.size() && studentFIO[i + j] == fio[j]) {
                ++j;
            }

            if (j == fio.size()) {
                ++fioOccurrencesFound;
                if (fioOccurrencesFound == fioOccurrences) {
                    break;
                }
            }
        }

        // Поиск по названию курса
        const string& studentCourseName = student.name.name;
        int courseNameOccurrencesFound = 0;

        for (size_t i = 0; i <= studentCourseName.size() - courseName.size(); ++i) {
            size_t j = 0;

            if (studentCourseName.size() < courseName.size()) {
                break;;
            }

            while (j < courseName.size() && studentCourseName[i + j] == courseName[j]) {
                ++j;
            }

            if (j == courseName.size()) {
                ++courseNameOccurrencesFound;
                if (courseNameOccurrencesFound == courseNameOccurrences) {
                    break;
                }
            }
        }

        // Если найдено нужное количество вхождений в ФИО и в названии курса, добавляем студента в список найденных
        if (fioOccurrencesFound == fioOccurrences && courseNameOccurrencesFound == courseNameOccurrences) {
            foundStudents.push_back(student);
        }
    }

    return foundStudents;
}


const int ASIZE = 256; // ASCII
// Функция для препроцессинга алгоритма Райта
vector<int> preWrightBc(const char* x, int m) {
    vector<int> bmBc(256, m); // ASIZE = 256 для ASCII
    for (int i = 0; i < m - 1; ++i) {
        bmBc[static_cast<int>(x[i])] = m - i - 1;
    }
    return bmBc;
}

int findFirst(const char* y, int fromIndex, int toIndex, char symbol) {
    for (int i = fromIndex; i <= toIndex; ++i) {
        if (y[i] == symbol)
            return i;
    }
    return -1;
}

bool restEquals(const char* y, int fromIndex, const char* x, int toIndex) {
    for (int i = fromIndex, j = 0; i <= toIndex; ++i, ++j) {
        if (y[i] != x[j])
            return false;
    }
    return true;
}

// Функция для поиска с использованием алгоритма Райта
vector<Student> wrightSearch(const vector<Student>& students, const string& fio, int fioOccurrences, const string& courseName, int courseNameOccurrences) {
    vector<Student> foundStudents;

    // Препроцессинг для каждого паттерна
    vector<int> fioBc = preWrightBc(fio.c_str(), fio.size());
    vector<int> courseNameBc = preWrightBc(courseName.c_str(), courseName.size());

    // Применяем алгоритм Райта для каждого студента
    for (const auto& student : students) {

        // Поиск по ФИО
        int fioOccurrencesFound = 0;
        int j = 0;
        while (j <= student.fullName.FIO.size() - fio.size()) {
            // Проверяем, совпадает ли конечный символ строки с конечным символом подстроки и сами строки совпадают
            if (student.fullName.FIO[j + fio.size() - 1] == fio.back() && restEquals(student.fullName.FIO.c_str() + j, 0, fio.c_str(), fio.size() - 1)) {
                ++fioOccurrencesFound;
                if (fioOccurrencesFound == fioOccurrences) {
                    break;
                } else {
                    j += fio.size(); // Переход на следующую позицию после найденного вхождения
                }
            } else {
                j += fioBc[static_cast<int>(student.fullName.FIO[j + fio.size() - 1])];
            }
        }

        // Поиск по названию курса
        int courseNameOccurrencesFound = 0;
        j = 0;
        if (student.name.name.size() < courseName.size()) {
            continue;
        }

        while (j <= student.name.name.size() - courseName.size()) {
            // Проверяем, совпадает ли конечный символ строки с конечным символом подстроки и сами строки совпадают
            if (student.name.name[j + courseName.size() - 1] == courseName.back() && restEquals(student.name.name.c_str() + j, 0, courseName.c_str(), courseName.size() - 1)) {
                ++courseNameOccurrencesFound;
                if (courseNameOccurrencesFound == courseNameOccurrences) {
                    break;
                } else {
                    j += courseName.size(); // Переход на следующую позицию после найденного вхождения
                }
            } else {
                j += courseNameBc[static_cast<int>(student.name.name[j + courseName.size() - 1])];
            }
        }

        // Если найдено нужное количество вхождений в ФИО и в названии курса, добавляем студента в список найденных
        if (fioOccurrencesFound >= fioOccurrences && courseNameOccurrencesFound >= courseNameOccurrences) {
            foundStudents.push_back(student);
        }
    }

    return foundStudents;
}

void printDirectedSearchVectorToFile(vector<Student> students, chrono::duration<double> directedSearchTime, ofstream& outFile) {
    for (int i = 0; i < size(students); ++i) {
        outFile << students[i] << " " << students[i].lineNumber << endl;
    }

    outFile << directedSearchTime.count() << " секунд" << endl;
    cout << "Найденные прямым поиском строки выведены в файл\n" << endl;
    
}

void printWrightSearchVectorToFile(vector<Student> students, chrono::duration<double> wrightSearchTime, ofstream& outFile) {
    for (int i = 0; i < size(students); ++i) {
        outFile << students[i] << " " << students[i].lineNumber << endl;
    }

    outFile << wrightSearchTime.count() << " секунд" << endl;
    cout << "Найденные поиском Райты строки выведены в файл\n" << endl;
    
}


int main() {
    srand(time(nullptr));
    generateKyesFile();

    vector<Student> students = readKeyFromFile();

    string fio = "Artem";
    int fioOccurrences = 1;
    
    string courseName = "progin";
    int courseNameOccurrences = 1;
    
    // Прямой поиск
    ofstream directedFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/String_Search/DirectSearchOutput.txt");

    auto start = chrono::high_resolution_clock::now();
    vector<Student> forDirectedSearch = directedSearch(students, fio, fioOccurrences, courseName, courseNameOccurrences);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> directedSearchTime = end - start;

    printDirectedSearchVectorToFile(forDirectedSearch, directedSearchTime, directedFile);
    directedFile.close();

    // Райта серч
    ofstream wrightFile("C:/Users/User/Desktop/FEFU/Second_course/FDSA/String_Search/WrightOutput.txt");
    start = chrono::high_resolution_clock::now();
    vector<Student> forWrightSearch = wrightSearch(students, fio, fioOccurrences, courseName, courseNameOccurrences);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> wrightSearchTime = end - start;

    printWrightSearchVectorToFile(forWrightSearch, wrightSearchTime, wrightFile);
    wrightFile.close();



    return 0;
}
