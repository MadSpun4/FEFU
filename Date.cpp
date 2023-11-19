#include <iostream>
 
using namespace std;
 
class Date {
private:
    int year, month, day, hour, minute, second;
    bool isOurEra;
    bool mistakeFlag = false;
 
    int daysInMonth(int m, int y) {
        if (m == 2) {
            return (abs(y) % 4 == 0 && abs(y) % 100 != 0) || (abs(y) % 400 == 0) ? 29 : 28;
        } else if (m == 4 || m == 6 || m == 9 || m == 11) {
            return 30;
        } else {
            return 31;
        }
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
 
        if ((day < 1) or (day > 31)) {
            cout << "Day should be between 1 and 31" << endl;
            mistakeFlag = true;
        }
 
        if (month < 1 or month > 12) {
            cout << "Month should be between 1 and 12" << endl;
            mistakeFlag = true;
        }
 
        if (year < 0) {
            cout << "Year couldn't be less than 0" << endl;
            mistakeFlag = true;
        }
 
        if ((year % 4 == 0 and year % 100 != 0) or (year % 400 == 0)) {
            if (month == 2 and (day < 1 or day > 29)) {
                cout << "In a leap year, February should have days between 1 and 29" << endl;
                mistakeFlag = true;
            }
        } else {
            if (month == 2 and (day < 1 or day > 28)) {
                cout << "In a non-leap year, February should have days between 1 and 28" << endl;
                mistakeFlag = true;
            }
        }
 
        if ((month == 4 or month == 6 or month == 9 or month == 11) and (day < 1 or day > 30)) {
            cout << "This month should have days between 1 and 30" << endl;
            mistakeFlag = true;
        }
    }
 
    Date(int dayVal, int monthVal, int yearVal, bool isOurEraVal, int hourVal, int minuteVal, int secondVal)
        : Date(dayVal, monthVal, yearVal, isOurEraVal) {
        hour = hourVal;
        minute = minuteVal;
        second = secondVal;
 
        if (hour < 0 or hour >= 24) {
            cout << "Hour should be between 0 and 23" << endl;
            mistakeFlag = true;
        }
 
        if (minute < 0 or minute >= 60) {
            cout << "Minute should be between 0 and 59" << endl;
            mistakeFlag = true;
        }
 
        if (second < 0 or second >= 60) {
            cout << "Second should be between 0 and 59" << endl;
            mistakeFlag = true;
        }
    }
 
    ~Date() {}
 
    bool getMistakeFlag() {
        return mistakeFlag;
    }
 
    Date add(int addDays, int addMonths, int addYears, int addHours, int addMinutes, int addSeconds) {
        int newYear = year + addYears;
        int newMonth = month + addMonths;
        int newDay = day + addDays;
        int newHour = hour + addHours;
        int newMinute = minute + addMinutes;
        int newSecond = second + addSeconds;

        // Обработка переносов
        if (newSecond >= 60) {
            newMinute += newSecond / 60;
            newSecond %= 60;
        }

        if (newMinute >= 60) {
            newHour += newMinute / 60;
            newMinute %= 60;
        }

        if (newHour >= 24) {
            newDay += newHour / 24;
            newHour %= 24;
        }

        if (newYear == 0) {
            newYear += 1;
            isOurEra = true;
        } else {
            isOurEra = false;
        }

        if ((abs(year) < addYears) & (year < 0)) {
            newYear++;
        }

        // Для месяцев и лет
        while (newMonth > 12) {
            newYear++;
            if (newYear == 0) {
                newYear += 1;
                isOurEra = true;
            } else {
                isOurEra = false;
            }
            newMonth -= 12;
        }
        
     

        // Для дней в месяцах
        while (newDay > daysInMonth(newMonth, newYear)) {
            newDay -= daysInMonth(newMonth, newYear);
            newMonth++;

            // Переход от BC к AD
            if (newMonth > 12) {
                newYear++;
                newMonth = 1;
            }
        }


        return Date(newDay, newMonth, newYear, isOurEra, newHour, newMinute, newSecond);
    }

 
    Date subtract(int substractDays, int substractMonths, int substractYears, int substractHours, int substractMinutes, int substractSeconds) {
        int newYear = year - substractYears;
        int newMonth = month - substractMonths;
        int newDay = day - substractDays;
        int newHour = hour - substractHours;
        int newMinute = minute - substractMinutes;
        int newSecond = second - substractSeconds;

        // Обработка заемов
        while (newSecond < 0) {
            newSecond += 60;
            newMinute--;
        }

        while (newMinute < 0) {
            newMinute += 60;
            newHour--;
        }

        while (newHour < 0) {
            newHour += 24;
            newDay--;
        }

        if ((abs(year) <= substractYears) and (year > 0)) {
            newYear--;
        }
        // Для месяцев и лет
        while (newMonth < 1) {
            newYear--;

            // Переход от AD к BC
            if (newYear == 0) {
                newYear -= 1;
                isOurEra = false;
            } else {
                isOurEra = true;
            }

            newMonth += 12;
        }

        if (newDay > daysInMonth(newMonth, newYear)) {
            newDay = daysInMonth(newMonth, newYear);
        }
        // Для дней в месяцах
        while (newDay < 1) {
            newMonth--;
            if (newMonth < 1) {
                newYear--;

                // Переход от AD к BC
                if (newYear == 0) {
                    newYear -= 1;
                    isOurEra = false;
                } else {
                    isOurEra = true;
                }

                newMonth = 12;
            }

            newDay += daysInMonth(newMonth, newYear);
        }
        

        return Date(newDay, newMonth, newYear, isOurEra, newHour, newMinute, newSecond);
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
            mistakeFlag = other.mistakeFlag;
        }
        return *this;
    }
 
    friend ostream& operator<<(ostream& os, const Date& dt) {
        string eraStr = (dt.year <= -1) ? "BC" : "AD";
 
        os << "Date: " << (dt.day >= 0 && dt.day <= 9 ? "0" : "") << dt.day << "/" << (dt.month >= 0 && dt.month <= 9 ? "0" : "") << dt.month << "/"
        << (abs(dt.year) >= 0 && abs(dt.year) <= 9 ? "000" : "") << (abs(dt.year) >= 10 && abs(dt.year) <= 99 ? "00" : "")
        << (abs(dt.year) >= 100 && abs(dt.year) <= 999 ? "0" : "") << abs(dt.year) << " " << eraStr << endl;
 
        os << "Time: " << (dt.hour >= 0 && dt.hour <= 9 ? "0" : "") << dt.hour << ":" << (dt.minute >= 0 && dt.minute <= 9 ? "0" : "") << dt.minute << ":"
        << (dt.second >= 0 && dt.second <= 9 ? "0" : "") << dt.second << endl;
 
        return os;
    }
 
    Date operator+(const Date& other) const {
        Date res;
        res = *this;

        res = res.add(other.day, other.month, other.year, other.hour, other.minute, other.second);
        return res;
    }
 
    Date& operator+=(const Date& other) {
        *this = this->add(other.day, other.month, other.year, other.hour, other.minute, other.second);
        return *this;
    }
 
    Date operator-(const Date& other) const {
        Date res;
        res = *this;

        res = res.subtract(other.day, other.month, other.year, other.hour, other.minute, other.second);
        return res;
    }
 
    Date& operator-=(const Date& other) {
        *this = this->subtract(other.day, other.month, other.year, other.hour, other.minute, other.second);
        return *this;
    }
 
    friend bool operator>(const Date& first, const Date& second) {
        return (first.year > second.year) ||
               (first.year == second.year && first.month > second.month) ||
               (first.year == second.year && first.month == second.month && first.day > second.day) ||
               (first.year == second.year && first.month == second.month && first.day == second.day && first.hour > second.hour) ||
               (first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute > second.minute) ||
               (first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute == second.minute && first.second > second.second);
    }
 
    friend bool operator<(const Date& first, const Date& second) {
        return (first.year < second.year) ||
               (first.year == second.year && first.month < second.month) ||
               (first.year == second.year && first.month == second.month && first.day < second.day) ||
               (first.year == second.year && first.month == second.month && first.day == second.day && first.hour < second.hour) ||
               (first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute < second.minute) ||
               (first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute == second.minute && first.second < second.second);
    }
 
    friend bool operator==(const Date& first, const Date& second) {
        return (first.year == second.year && first.month == second.month && first.day == second.day && first.hour == second.hour && first.minute == second.minute && first.second == second.second);
    }
};
 
int main() {
    /*Date defaultDate;
    cout << "Default date: " << endl;
    cout << defaultDate << endl;

    Date myBirthDayWithTime(1, 1, 1, 1, 1, 1, 1);
    if (!myBirthDayWithTime.getMistakeFlag()) {
        cout << "My birthdayWithTime: " << endl;
        cout << myBirthDayWithTime << endl;
    }
    */
   
    /*Date myDate(27, 3, 10, 1, 1, 11 ,-0);
    cout << myDate << endl;
    myDate = myDate.subtract(0, 1, 0, 0, 0, 360);
    cout << myDate; */
    Date lol(28, 2, -1, 0, 23, 59, 59);
    Date kek(0, 0, 4, 1, 0, 0, 1);

    cout << "lol" << endl;
    cout << lol << endl;
    cout << "kek" << endl;
    cout << kek << endl;

    lol = lol.add(0, 0, 4, 0, 0, 1);
    cout << "lol after add" << endl;
    cout << lol << endl;

    lol = lol.subtract(0, 0, 5, 0, 0, 1);
    cout << "lol after sub" << endl;
    cout << lol << endl;
    
    lol = lol + kek;
    cout << lol << endl;

 
    return 0;
}