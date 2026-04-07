#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

class WorkShift {
private:
    static int nextID;
    int shiftID;
    std::string employeeName;
    std::string dayOfWeek;
    std::string startTime;
    std::string endTime;

    // Helper to convert "HH:MM" into total minutes from midnight
    int timeToMins(const std::string& t) const {
        if (t.length() < 5) return 0;
        int h = std::stoi(t.substr(0, 2));
        int m = std::stoi(t.substr(3, 2));
        return h * 60 + m;
    }

public:
    WorkShift(std::string name, std::string day, std::string start, std::string end)
        : employeeName(name), dayOfWeek(day), startTime(start), endTime(end) {
        shiftID = nextID++;
    }

    int getID() const { return shiftID; }
    std::string getName() const { return employeeName; }
    std::string getDay() const { return dayOfWeek; }
    std::string getStartTime() const { return startTime; }
    std::string getEndTime() const { return endTime; }

    int getStartMins() const { return timeToMins(startTime); }
    int getEndMins() const { return timeToMins(endTime); }

    double getDurationHours() const {
        return (getEndMins() - getStartMins()) / 60.0;
    }

    void display() const {
        std::cout << "ID: " << shiftID << " | " << employeeName << " (" << dayOfWeek << " " << startTime << "-" << endTime << ")" << std::endl;
    }
};

int WorkShift::nextID = 1000;

// Read data from employee CSV file
std::vector<WorkShift> readCSV(std::string filename) {
    std::vector<WorkShift> shifts;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open the file" << std::endl;
        return shifts;
    }

    // Skip the first row of CSV file...
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name, day, start, end;

        std::getline(ss, name, ',');
        std::getline(ss, day, ',');
        std::getline(ss, start, ',');
        std::getline(ss, end, ',');

        // Clean up any carriage returns (\r) at the end of the line from Windows CSVs
        if (!end.empty() && end.back() == '\r') {
            end.pop_back();
        }

        shifts.emplace_back(name, day, start, end);
    }
    file.close();
    return shifts;
}

// Check if two individual shifts overlap
bool shiftsOverlap(const WorkShift& a, const WorkShift& b) {
    if (a.getDay() != b.getDay()) return false;
    return (a.getStartMins() < b.getEndMins()) && (b.getStartMins() < a.getEndMins());
}

// Check if two employees EVER overlap in their weekly schedule
bool peopleOverlap(const std::string& p1, const std::string& p2, const std::vector<WorkShift>& shifts) {
    std::vector<WorkShift> s1, s2;
    for (const auto& s : shifts) {
        if (s.getName() == p1) s1.push_back(s);
        if (s.getName() == p2) s2.push_back(s);
    }
    for (const auto& a : s1) {
        for (const auto& b : s2) {
            if (shiftsOverlap(a, b)) return true;
        }
    }
    return false;
}

int main() {
    std::string filename = "win26hrs.csv";
    std::vector<WorkShift> myShifts = readCSV(filename);

    if (myShifts.empty()) {
        std::cout << "No shifts loaded. Check if " << filename << " exists and has data." << std::endl;
        return 1;
    }

    //Show weekly hours per person
    std::map<std::string, double> weeklyHours;
    std::vector<std::string> uniqueNames; 

    for (const auto& shift : myShifts) {
        if (weeklyHours.find(shift.getName()) == weeklyHours.end()) {
            uniqueNames.push_back(shift.getName()); // Keep track of unique employees
        }
        weeklyHours[shift.getName()] += shift.getDurationHours();
    }

    std::cout << "\n--- Weekly Hours per Employee ---\n";
    for (const auto& name : uniqueNames) {
        std::cout << name << ": " << std::fixed << std::setprecision(2) << weeklyHours[name] << " hours\n";
    }

    //Ask user for available seats
    int numSeats;
    std::cout << "\nHow many seats are available in the office? ";
    std::cin >> numSeats;

    if (numSeats <= 0) {
        std::cout << "You need at least 1 seat to assign employees!\n";
        return 1;
    }

    //Assign seats to avoid schedule overlaps
    std::vector<std::vector<std::string>> seats(numSeats); // Vector of seats, each holding a list of assigned names
    std::vector<std::string> unassigned;

    for (const auto& person : uniqueNames) {
        bool placed = false;
        
        // Try to fit the person into an existing seat
        for (int i = 0; i < numSeats; ++i) {
            bool canFit = true;
            for (const auto& occupant : seats[i]) {
                if (peopleOverlap(person, occupant, myShifts)) {
                    canFit = false; // Schedule conflicts with someone already in this seat
                    break;
                }
            }
            if (canFit) {
                seats[i].push_back(person);
                placed = true;
                break;
            }
        }
        
        // If they couldn't fit in any available seat
        if (!placed) {
            unassigned.push_back(person);
        }
    }

    // Output the seat assignments
    std::cout << "\n--- Seat Assignments ---\n";
    for (int i = 0; i < numSeats; ++i) {
        std::cout << "Seat " << (i + 1) << ": ";
        if (seats[i].empty()) {
            std::cout << "Empty";
        } else {
            for (size_t j = 0; j < seats[i].size(); ++j) {
                std::cout << seats[i][j] << (j < seats[i].size() - 1 ? ", " : "");
            }
        }
        std::cout << "\n";
    }

    if (!unassigned.empty()) {
        std::cout << "\nWARNING: Not enough seats! The following employees could not be seated due to schedule overlaps:\n";
        for (const auto& name : unassigned) {
            std::cout << "- " << name << "\n";
        }
    } else {
         std::cout << "\nSuccess! All employees have been seated without schedule conflicts.\n";
    }

    return 0;
}