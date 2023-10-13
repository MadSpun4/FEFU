#include <iostream>
#include <cmath>

using namespace std;

class Quadrilateral {
private:
    float aSide, bSide, cSide, dSide;

public:
    virtual float perimetr() {
        return aSide + bSide + cSide + dSide;
    }

};

class Rectangle : public Quadrilateral {
private:
    float aSide, bSide;

public:
    Rectangle(float a, float b) {
        aSide = a;
        bSide = b;
    }

    float perimetr() override{
        return (aSide + bSide) * 2;
    }

    void setA(float A) {
        aSide = A;
    }

    float getA() {
        return aSide;
    }
    
    void setB(float B) {
        bSide = B;
    }

    float getB() {
        return bSide;
    }

};

class Square : public Quadrilateral {
private:
    float aSide;

public:
    Square(float side) {
        this->aSide = side;
    }

    float perimetr() override {
        return aSide * 4;
    }
    
    void setA(float A) {
        aSide = A;
    }

    float getA() {
        return aSide;
    }

};

int main() {
    float sideLength;   
    cout << "Input lenght of square side: ";
    cin >> sideLength;

    Square square(sideLength);
    cout << "Square perimetr: " << square.perimetr() << endl;

    return 0;
}
