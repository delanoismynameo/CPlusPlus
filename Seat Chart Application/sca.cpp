#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class WorkShift {
private:
    static int nextID;
    int shiftID;
    std::string employeeName; //bob, etc.
    std::string dayOfWeek;    //monday, etc.
    std::string startTime;    // 09:00, etc.
    std::string endTime;      // 17:00, etc

//create constructor
public:
    WorkShift(std::string name, std::string day, std::string start, std::string end) //{
        : employeeName(name), dayOfWeek(day), startTime(start), endTime(end) {
            shiftID = nextID++; //assing current ID then set up next ID
        }
        int getID() const {return shiftID;}
    //     employeeName = name;
    //     dayOfWeek = day;
    //     startTime = start;
    //     endTime = end;
    // }
};

//get data from employee csv file
std::string readCSV() {
    return;
}


std::time_t workPlz() {
    auto now = std::chrono::system_clock::now();
    std::time_t work = std::chrono::system_clock::to_time_t(now);

    std::tm* localTime = std::localtime(&work);

    // std::cout << std::put_time(localTime, "%H:%M") << std::endl;
    std::cout << localTime << std::endl;

    return work;
}

//call to main function here
int main() {
    workPlz();
}