#include <iostream>

using namespace std;

class Human
{
private:
    string Name;
    unsigned short int Age;
    bool Gender;

public:
    Human(string nameVal, unsigned short int ageVal, bool genderVal) {
        Name = nameVal;
        Age = ageVal;
        Gender = genderVal;
    };
    ~Human(){

    };

    void setName(string name) {
        Name = name;
    }
    
    void setAge(unsigned short int age) {
        Age = age;
    }

    void setGender(bool gender) {
        Gender = gender;
    }

    string getName() {
        return Name;
    }

    int getAge() {
        return Age;
    }

    bool getGender() {
        return Gender;
    }

    friend ostream& operator<<(ostream& os, const Human& hm) {
        os << "Name: " << hm.Name << "\n" << "Age: " << hm.Age << "\n"
        << "Gender: " << (hm.Gender ? "Male" : "Female") << endl;
 
        return os;
    }
};

int main() {
    Human Borya("Borya", 22, true);
    Human Ilya("Ilya", 22, true);
    cout << Borya << endl;
    cout << Ilya << endl;

    return 0;
}