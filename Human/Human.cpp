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

class Mother : public Human
{
public:
    Mother(string nameVal, unsigned short int ageVal, bool genderVal)
        : Human(nameVal, ageVal, genderVal) {}

};

class Son : public Mother
{
public:
    Son(string nameVal, unsigned short int ageVal, bool genderVal)
        : Mother(nameVal, ageVal, genderVal) {}

};

class Daughter : public Mother
{
public:
    Daughter(string nameVal, unsigned short int ageVal, bool genderVal)
        : Mother(nameVal, ageVal, genderVal) {}

};

class Grandson : public Daughter
{
public:
    Grandson(string nameVal, unsigned short int ageVal, bool genderVal)
        : Daughter(nameVal, ageVal, genderVal) {}
        
};



int main() {
    Grandson Borya("Borya", 22, 1); 
    Human Ilya("Ilya", 22, 1);
    cout << Borya << endl;
    cout << Ilya << endl;

    Borya.setName("Grandson");
    Borya.setGender(1);
    cout << Borya << endl;

    return 0;
}