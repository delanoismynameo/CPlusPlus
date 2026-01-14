#include <iostream>
#include <iomanip>
#include <chrono>
#include<ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class WorkShift {
private:
    std::string employeeName; //bob, etc.
    std::string dayOfWeek; //monday, etc.
    std::string startTime;  // 09:00
    std::string endTime;    // 17:00
}

//call to main function here
int main() {
    workPlz();
}

char readCSV() {

}


std::time_t workPlz() {
    auto now = std::chrono::system_clock::now();
    std::time_t work = std::chrono::system_clock::to_time_t(now);

    std::tm* localTime = std::localtime(&work);

    // std::cout << std::put_time(localTime, "%H:%M") << std::endl;
    std::cout << localTime << std::endl;

    return work;
}