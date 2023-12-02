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