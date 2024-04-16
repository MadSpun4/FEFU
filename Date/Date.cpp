#include <iostream>
 
using namespace std;
 
class Date {
private:
    int year, month, day, hour, minute, second;
    bool isOurEra;

    int daysInMonth(int month) {
        int daysM = 31;
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        daysM = 31;
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            daysM = 30;
        } else if (month == 2 && ((this->year % 4 == 0 && this->year % 100 != 0) || (this->year % 400 == 0))) {
            daysM = 29;
        } else if (month == 2) {
            daysM = 28;
        }
        return daysM;
    }
 
public:
    Date() {
        year = 1960;
        month = 1;
        day = 1;
        hour = 0;
        minute = 0;
        second = 0;
        isOurEra = true;
    }
 
    Date(int dayVal, int monthVal, int yearVal, bool isOurEraVal) {
        year = yearVal;
        month = monthVal;
        day = dayVal;
        isOurEra = isOurEraVal;
 
    }
 
    Date(int dayVal, int monthVal, int yearVal, bool isOurEraVal, int hourVal, int minuteVal, int secondVal)
        : Date(dayVal, monthVal, yearVal, isOurEraVal) {
        hour = hourVal;
        minute = minuteVal;
        second = secondVal;
 
    }
    Date(const Date& other) {
        this->isOurEra = other.isOurEra;
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
        this->hour = other.hour;
        this->minute = other.minute;
        this->second = other.second;
    }
 
    ~Date() {}
 
    Date add(int addYears, int addMonths, int addDays, int addHours, int addMinutes, int addSeconds) {
        Date res = *this;

        // Инвертируем значения, если до н.э.
        if (!res.isOurEra) {
            addYears = -addYears;
            addMonths = -addMonths;
            addDays = -addDays;
            addHours = -addHours;
            addMinutes = -addMinutes;
            addSeconds = -addSeconds;
        }

        // Добавление времени
        res.second += addSeconds;
        while (res.second >= 60) {
            res.minute++;
            res.second -= 60;
        }

        res.minute += addMinutes;
        while (res.minute >= 60) {
            res.hour++;
            res.minute -= 60;
        }

        res.hour += addHours;
        while (res.hour >= 24) {
            res.day++;
            res.hour -= 24;
        }

        // Добавление дней и месяцев
        res.month += addMonths;
        res.day += addDays;

        // Коррекция месяцев и годов
        while (res.month > 12) {
            res.month -= 12;
            res.year++;
        }

        // Коррекция дней и месяцев
        while (true) {
            int daysM = res.daysInMonth(res.month);
            if (res.day > daysM) {
                res.day -= daysM;
                res.month++;
                if (res.month > 12) {
                    res.month -= 12;
                    res.year++;
                }
            } else {
                break;
            }
        }

        // Добавление годов
        res.year += addYears;

        // Обработка времени
        while (res.second < 0) {
            res.minute--;
            res.second += 60;
        }

        while (res.minute < 0) {
            res.hour--;
            res.minute += 60;
        }

        while (res.hour < 0) {
            res.day--;
            res.hour += 24;
        }
        
        while (res.day < 1) {
            res.month--;
            int daysM = res.daysInMonth(res.month);
            res.day += daysM; 
        }

        while (res.month < 1) {
            res.year--;
            res.month += 12;
        }

        // Коррекция для перехода эр
        if (!res.isOurEra) {
            if (res.year <= 0) {
                res.year = 1 - res.year; // Переход через нулевой год
                res.isOurEra = true;
            }
        } else {
            if (res.year <= 0) {
                res.year = 1 - res.year; // Обработка отрицательного года в AD
                res.isOurEra = false;
            }
        }

        return res;
    }

    Date subtract(int substractYears, int substractMonths, int substractDays, int substractHours, int substractMinutes, int substractSeconds) {
        Date res = *this;

        // Инвертируем значения, если до н.э.
        if (!res.isOurEra) {
            substractYears = -substractYears;
            substractMonths = -substractMonths;
            substractDays = -substractDays;
            substractHours = -substractHours;
            substractMinutes = -substractMinutes;
            substractSeconds = -substractSeconds;
        }

        // Обработка времени
        res.second -= substractSeconds;
        while (res.second < 0) {
            res.minute--;
            res.second += 60;
        }

        res.minute -= substractMinutes;
        while (res.minute < 0) {
            res.hour--;
            res.minute += 60;
        }

        res.hour -= substractHours;
        while (res.hour < 0) {
            res.day--;
            res.hour += 24;
        }

        // Обработка дней и месяцев
        res.month -= substractMonths;
        res.day -= substractDays;

        // Коррекция месяцев и годов
        while (res.month < 1) {
            res.month += 12;
            res.year--;
        }

        // Коррекция дней и месяцев
        while (res.day < 1) {
            res.month--;
            if (res.month < 1) {
                res.month = 12;
                res.year--;
            }
            int daysM = res.daysInMonth(res.month);
            res.day += daysM;
        }

        // Обработка годов и эры
        res.year -= substractYears;
        if (res.year <= 0) {
            int remainingYears = 1 - res.year; // +1 для коррекции нулевого года
            res.year = remainingYears;
            res.isOurEra = !res.isOurEra; // Смена эры
        }

        // Обработка времени
        while (res.second >= 60) {
            res.minute++;
            res.second -= 60;
        }

        while (res.minute >= 60) {
            res.hour++;
            res.minute -= 60;
        }

        while (res.hour >= 24) {
            res.day++;
            res.hour -= 24;
        }

        int daysM = res.daysInMonth(res.month);
        // Коррекция месяцев и дней
        while (res.day > daysM) {
            int daysM = res.daysInMonth(res.month);
            res.month++;
            res.day -= daysM;
        }

        // Коррекция месяцев и годов
        while (res.month > 12) {
            res.year++;
            res.month -= 12;
        }

        return res;
    }

    Date& operator=(const Date& other) {
        if (this != &other) {
            year = other.year;
            month = other.month;
            day = other.day;
            hour = other.hour;
            minute = other.minute;
            second = other.second;
            isOurEra = other.isOurEra;
        }
        return *this;
    }
 
    friend ostream& operator<<(ostream& os, const Date& dt) {
        string eraStr = dt.isOurEra ? "AD" : "BC";

        os << "Date: ";
        os << (dt.day < 10 ? "0" : "") << dt.day << "/";
        os << (dt.month < 10 ? "0" : "") << dt.month << "/";
        os << (dt.year < 1000 ? "0" : "") << (dt.year < 100 ? "0" : "") << (dt.year < 10 ? "0" : "") << dt.year << " ";
        os << eraStr << endl;

        os << "Time: ";
        os << (dt.hour < 10 ? "0" : "") << dt.hour << ":";
        os << (dt.minute < 10 ? "0" : "") << dt.minute << ":";
        os << (dt.second < 10 ? "0" : "") << dt.second << endl;

        return os;
    }

    Date operator+(const Date& other)
    {
        Date res;
        res = *this;
        res = res.add(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return res;
    }
 
    Date& operator+= (const Date& other)
    {
        *this = this->add(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return *this;
    }
 
    Date operator-(const Date& other)
    {
        Date res;
        res = *this;
        res = res.subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return res;
    }
 
    Date& operator-= (const Date& other)
    {
        *this = this->subtract(other.year, other.month, other.day, other.hour, other.minute, other.second);

        return *this;
    }
 
    friend bool operator>(const Date& first, const Date& second) {
        return (first.isOurEra > second.isOurEra) ||
               (first.isOurEra == second.isOurEra && first.year > second.year) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month > second.month) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day > second.day) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour > second.hour) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute > second.minute) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute == second.minute && first.second > second.second);
    }
 
    friend bool operator<(const Date& first, const Date& second) {
        return (first.isOurEra < second.isOurEra) ||
               (first.isOurEra == second.isOurEra && first.year < second.year) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month < second.month) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day < second.day) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour < second.hour) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute < second.minute) ||
               (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute == second.minute && first.second < second.second);
    }
 
    friend bool operator==(const Date& first, const Date& second) {
        return (first.isOurEra == second.isOurEra && first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute == second.minute && first.second == second.second);
    }
};
 
int main() {

    Date d1(1, 1, 2000, 0, 0, 0, 0);
    cout << d1 << endl;
    Date d2(d1-d1+d1-d1+d1);
    cout << d2 << endl; // 2 2 4000 bc -> 1 1 2000 bc -> 2 2 4000 bc -> 1 1 2000 bc
    return 0;
}