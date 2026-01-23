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
    
    // make getters
    int getID() const {return shiftID;}
    std::string getName() const {return employeeName;}
    std::string getDay() const {return dayOfWeek;}

    // Shift start and end time helper
    void display() const{
        std::cout << "ID: " <<shiftID<< " | "<<employeeName<< " ( " <<dayOfWeek<< " " <<startTime << "-" <<endTime << ")" << std::endl;
    }
};

//get data from employee csv file
std::vector<WorkShift> readCSV(std::string filename) {
    std::vector<WorkShift> shifts;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()){
        std::cerr <<"Could not open the file" << std::endl;
        return shifts;
    }

    // skip the first row of csv file... name, day, etc.
    std::getline(file, line);

    // read each file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, day, start, end;

        std::getline(ss, name, ',');
        std::getline(ss, day, ',');
        std::getline(ss, start, ',');
        std::getline(ss, end, ',');

        shifts.emplace_back(name, day, start, end);
    }
    file.close();
    return shifts;
}


std::time_t workPlz() {
    auto now = std::chrono::system_clock::now();
    std::time_t work = std::chrono::system_clock::to_time_t(now);

    std::tm* localTime = std::localtime(&work);

    std::cout << std::put_time(localTime, "%H:%M") << std::endl;
    // std::cout << localTime << std::endl;

    return work;
}
int WorkShift::nextID = 1000;
//call to main function here
int main() {
    // tester function
    workPlz();

    std::string filename = "win26hrs.csv";
    std::vector<WorkShift> myShifts = readCSV(filename);

    if (myShifts.empty()) {
        std::cout << "No shifts loaded. Check if " <<filename<< " exists and has data" <<std::endl;
    } else {
        std::cout << "\n--- Current Schedule ---" <<std::endl;
        for (const auto& shift : myShifts) {
            shift.display();
        }
    }

    return 0;
}