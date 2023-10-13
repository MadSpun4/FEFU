#include <iostream>
#include <random>
#include <cmath>

using namespace std;

int main()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> distribution(-5.0, 5.0);

    int numberOfShots = 0;
    int score = 0;
    float aim_1 = 1.0, aim_2 = 2.0, aim_3 = 3.0, aim_4 = 4.0, aim_5 = 5.0;
    
    while (numberOfShots != 5) {
        cout << endl;
        cout << "Press << Enter >> to shoot" << endl;
        cin.get();

        float x = distribution(gen);
        float y = distribution(gen);
        cout << x << endl;
        cout << y << endl;
        float distance = sqrt(x * x + y * y); // по формуле окружности

        int choice = 1;
        if ((distance >= 0.0) and (distance < aim_1)) {
            choice << 0; // = 1
        } else if ((distance >= aim_1) and (distance < aim_2)) {
            choice = choice << 1; // = 2
        } else if ((distance >= aim_2) and (distance < aim_3)) {
            choice = (choice << 1) & choice; // = 3
        } else if ((distance >= aim_3) and (distance < aim_4)) {
            choice = (choice << 2); // = 4
        } else if ((distance >= aim_4) and (distance < aim_5)) {
            choice = (choice << 2) & choice; // = 5
        }

        switch (choice)
        {
        case 1:
            score += 5;
            cout << "+5 scores!" << endl;
            cout << distance << endl;
            cout << x << " " << y << endl;
            break;
        case 2:
            score += 4;
            cout << "+4 scores!" << endl;
            cout << distance << endl;
            cout << x << " " << y << endl;
            break;
        case 3:
            score += 3;
            cout << "+3 scores!" << endl;
            cout << distance << endl;
            cout << x << " " << y << endl;
            break;
        case 4:
            score += 2;
            cout << "+2 scores!" << endl;
            cout << distance << endl;
            cout << x << " " << y << endl;
            break;
        case 5:
            score += 1;
            cout << "+1 scores!" << endl;
            cout << distance << endl;
            cout << x << " " << y << endl;
            break;
        default:
            cout << "+0 scores!\nMiss!" << endl; 
            cout << distance << endl;
            cout << x << " " << y << endl;
            break;                               
        }
        numberOfShots++;
    }
    cout << endl;
    if (score < 10) {
        cout << "LOOSER!" << endl;
    } else {
      cout  << "!!!CONGRATULATIONS!!!\nFinal scores: " << score << endl;
    }
    return 0;
}
