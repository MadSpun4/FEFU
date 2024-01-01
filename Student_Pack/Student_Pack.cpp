// Итеративно разработать приложение для работы со студентами. В рамках задания будут появляться новые требования. Необходимо иногда рефакторить код (см low coupling, strong cohesion, SOLID, KISS, YAGNI, DRY и тд) под специфику требований, увеличивая сопровождаемость на будущее.

// Этап 1
// В предметной области есть Студент, студент может получать отметки.

// Необходимо произвольной формулой вычислить является ли студент отличником.

// Архитектурный тест 1
// Система должна позволять добавлять новых студентов, выставлять им оценки, определять кто отличник, а кто нет

// -—

// Этап 2
// В предметной области появляется Преподаватель, который может выставлять студентам отметки.

// Архитектурный тест 2
// Система должна позволять добавлять новых преподавателей. Преподаватель может выставлять оценки произвольным студентам

// -—

// Этап 3
// У преподавателя есть настроение. От настроения преподавателя зависит оценка, которую он поставит студенту. Также на оценку влияет тот, кому преподаватель ставит эту оценку.

// Пример формул расчёта итоговой оценки:
// хорошее настроение + отличник = 5
// плохое настроение + отличник = 4 или 5 (равновероятно)
// хорошее настроение + не отличник = 4
// плохое настроение + не отличник = 2 или 3 (равновероятно)

// -—

// Этап 4
// В предметной области появляется Занятие. На занятии присутствует один преподаватель и некоторые студенты. На занятии преподаватель выставляет произвольное количество оценок(внимая своему настроению) произвольным студентам, которые пришли на занятие.

// Архитектурный тест 4
// Система должна позволять добавлять новые занятия.

// -—

// Этап 5
// Есть преподаватели, которые ставят 5, вне зависимости от настроения и от того, кому они ставят оценку.

// Архитектурный тест 5
// Архитектура предусматривает появление и тех преподавателей, которые будут всегда ставить 2.

// -—

// Этап 6
// Каждую пятую выставленную оценку у преподавателя меняется настроение (случайным образом, не обязательно на противоположное)

// Архитектурный тест 6
// Архитектура предусматривает появление и тех преподавателей, настроение у которых меняется чаще или реже

// -—

// Этап 7
// В предметной области появляется Родитель. У родителя может быть настроение. Родитель может иметь несколько детей. Родитель может выполнять следующие функции:
// - Рассказать о каждом своём ребёнке что-нибудь (зависит от настроения и от того отличник он или нет)
// - Рассказать об одном случайном своём ребёнке что-нибудь (зависит от настроения и от того отличник он или нет)
// - Рассказать в общем, суммарно, обо всех своих детях (зависит от настроения и от того отличники они в среднем или нет)
// - Рассказать о конкретном своём ребёнке (зависит от настроения и от того отличники они в среднем или нет). Если это не ребёнок родителя, то сообщить об ошибке

// -—

// Этап 8
// В предметной области появляется Собрание. На собрании участвуют Преподаватели, Родители. Они обсуждают все прошедшие занятия и родители по каждому из своих детей, которые получали за эти занятия хотя бы одну оценку, высказываются "о конкретном своём ребёнке". Если на собрании нет преподавателя, который проводил Занятие, то эти выставленные оценки не учитываются при необходимости обсуждения ребёнка. Если на собрании нет родителя ребёнка, то список таких детей формируется в конце собрания и должен быть оглашён.

// Архитектурный тест 8
// Архитектура позволяет проводить несколько собраний по различному набору Занятий, меняя состав преподавателей и родителей.


// -—
// Этап 9
// В предметной области появляется Бабушка, которая являясь родителем/опекуном ребёнка может выступать на собраниях и обладает теми же функциями, что и Родитель. Единственное, что у Бабушка всегда о своих детях говорит хорошо, а о чужих по-настроению.

#include <iostream>
#include <vector>
#include <cstdlib> // Для rand()

using namespace std;

enum Mood { GOOD, BAD };

class Student {
private:
    vector<int> marks;
    string name;

public:
    Student(const string Name) : name(Name) {

    }

    void printMarks() const {
        for (int mark : marks) {
            cout << mark << " ";
        }
    }
    
    void addMark(int mark) {
        marks.push_back(mark);
    }
    

    bool isHonorsStudent() const {
        double sum = 0.0;
        for (int mark : marks) {
            sum += mark;
        }
        return sum / marks.size() >= 4.5;  // 4.5+ = отличник
    }
     
    string getName() const {
        return name;
    }
};

enum ConstMark { BASE, ALWAYS_FIVE}; // всегда одна оценка. Можно добавить другие оценки

class Teacher {
private:
    string name;
    Mood mood;
    ConstMark constMark;
public:
    Teacher(const string& Name, Mood Mood, ConstMark ConstMark = BASE)
        : name(Name), mood(Mood), constMark(ConstMark) {}

    void changeMood(Mood newMood) {
        mood = newMood;
    }

    void giveMark(Student& student, int mark) {
        student.addMark(mark);
    }

    void giveMoodMark(Student& student) {
        int mark;
        // если надо будет добавить другие оценки, тогда else if и тд
        if (constMark == ALWAYS_FIVE) { 
            mark = 5;
        } else { // нормальное оценивание
            if (mood == GOOD) {
                mark = student.isHonorsStudent() ? 5 : 4;
            } else { 
                if (student.isHonorsStudent()) {
                    mark = rand() % 2 + 4; // 4 или 5
                } else {
                    mark = rand() % 2 + 2; // 2 или 3
                }
            }
        }
        student.addMark(mark);
    }
};

class Lesson {
private:
    Teacher* teacher;
    vector<Student*> students;

public:
    Lesson(Teacher* Teacher, const vector<Student*>& Student) : teacher(Teacher), students(Student) {}

    void doLesson() {
        for (auto& student : students) {
            int numMarks = rand() % 3 + 1; // от 1 до 3 оценок за урок каждому пришедшему студенту
            for (int i = 0; i < numMarks; i++) {
                teacher->giveMoodMark(*student);
            }
        }
    }
};

vector<Student*> students;

void addStudent(Student* student) {
    students.push_back(student);
}

void printHonorsStudents() {
    for (const auto student : students) {
        cout << student->getName() << ": " << (student->isHonorsStudent() ? "отличник" : "не отличник") << endl;
    }
}

int main() {
    srand(time(0));

    Student* student1 = new Student("Ura");
    Student* student2 = new Student("Dima");

    addStudent(student1);
    addStudent(student2);

    Teacher* teacher1 = new Teacher("Vsiliy Vsilievich", BAD, BASE);
    teacher1->giveMark(*student1, 5);
    teacher1->giveMark(*student1, 4);
    teacher1->giveMark(*student2, 4);
    teacher1->giveMark(*student2, 3);

    student1->printMarks();
    cout << endl;

    teacher1->giveMoodMark(*student1);

    student1->printMarks();
    cout << endl;

    Lesson lesson1(teacher1, {student1, student2});
    lesson1.doLesson();

    cout << endl;

    student1->printMarks();
    cout << endl;
    student2->printMarks();
    cout << endl;

    Teacher* teacher2 = new Teacher("Valera", BAD, ALWAYS_FIVE);
    teacher2->giveMoodMark(*student1);
    student1->printMarks();
    cout << endl;


    printHonorsStudents();


    for (auto student : students) {
        delete student;
    }
    delete teacher1;

    return 0;
}