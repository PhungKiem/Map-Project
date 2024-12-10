#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <utility>

namespace std
{

// Class representing a single row in the schedule
class ScheduleItem {
private:
    string subject;
    string catalog;
    string section;
    string component;
    string session;
    int units;
    int totEnrl;
    int capEnrl;
    string instructor;

public:
    // Constructor with default parameters
    ScheduleItem(string subj = "", string cat = "", string sec = "", string comp = "",
                 string sess = "", int unit = 0, int tot = 0, int cap = 0, string instr = "")
        : subject(subj), catalog(cat), section(sec), component(comp), session(sess),
          units(unit), totEnrl(tot), capEnrl(cap), instructor(instr) {}

    // Overloaded operators
    bool operator==(const ScheduleItem &other) const {
        return subject == other.subject && catalog == other.catalog && section == other.section;
    }

    bool operator!=(const ScheduleItem &other) const {
        return !(*this == other);
    }

    bool operator>=(const ScheduleItem &other) const {
        return subject >= other.subject;
    }

    // Print function
    void print() const {
        cout << subject << "\t" << catalog << "\t" << section << "\t" << component << "\t"
             << session << "\t" << units << "\t" << totEnrl << "\t" << capEnrl << "\t"
             << instructor << "\n";
    }

    // Getters for key components
    string getKey() const {
        return subject + "_" + catalog + "_" + section;
    }

    string getSubject() const { return subject; }
    string getCatalog() const { return catalog; }
    string getInstructorLastName() const {
        size_t commaPos = instructor.find(",");
        return (commaPos != string::npos) ? instructor.substr(0, commaPos) : instructor;
    }
};

// Class representing the container for the schedule
class Schedule {
private:
    map<string, ScheduleItem> scheduleMap;

public:
    // Initialize the schedule from a file
    void initSchedule(ifstream &file) {
        string line;

        // Discard the header row
        getline(file, line);

        while (getline(file, line)) {
            istringstream ss(line);
            vector<string> fields;
            string field;

            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }

            if (fields.size() < 9) continue; // Ensure there are enough fields

            ScheduleItem item(fields[0], fields[1], fields[2], fields[3], fields[4],
                              stoi(fields[5]), stoi(fields[6]), stoi(fields[7]),
                              fields[8]);
            addEntry(item);
        }
    }

    // Add an entry to the schedule
    void addEntry(const ScheduleItem &item) {
        scheduleMap[item.getKey()] = item;
    }

    // Print the entire schedule
    void print() const {
        for (const auto &[key, item] : scheduleMap) {
            item.print();
        }
    }

    // Find and print records by subject
    void findBySubject(const string &subject) const {
        for (const auto &[key, item] : scheduleMap) {
            if (item.getSubject() == subject) {
                item.print();
            }
        }
    }

    // Find and print records by subject and catalog number
    void findBySubjectAndCatalog(const string &subject, const string &catalog) const {
        for (const auto &[key, item] : scheduleMap) {
            if (item.getSubject() == subject && item.getCatalog() == catalog) {
                item.print();
            }
        }
    }

    // Find and print records by instructor's last name
    void findByInstructorLastName(const string &lastName) const {
        for (const auto &[key, item] : scheduleMap) {
            if (item.getInstructorLastName() == lastName) {
                item.print();
            }
        }
    }
};

}

// Main function
int main() {
    using namespace std;

    ifstream file("schedule.csv");
    if (!file.is_open()) {
        cerr << "Error opening file.\n";
        return 1;
    }

    Schedule schedule;
    schedule.initSchedule(file);
    file.close();

    int choice;
    string input1, input2;

    do {
        cout << "\nMenu:\n"
             << "1. Print Schedule\n"
             << "2. Find by Subject\n"
             << "3. Find by Subject and Catalog\n"
             << "4. Find by Instructor Last Name\n"
             << "0. Exit\n"
             << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            schedule.print();
            break;
        case 2:
            cout << "Enter Subject: ";
            getline(cin, input1);
            schedule.findBySubject(input1);
            break;
        case 3:
            cout << "Enter Subject: ";
            getline(cin, input1);
            cout << "Enter Catalog: ";
            getline(cin, input2);
            schedule.findBySubjectAndCatalog(input1, input2);
            break;
        case 4:
            cout << "Enter Instructor's Last Name: ";
            getline(cin, input1);
            schedule.findByInstructorLastName(input1);
            break;
        case 0:
            cout << "Exiting program.\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
