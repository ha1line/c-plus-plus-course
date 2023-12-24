#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <memory>
#include <stdexcept>


class Date {
public:
    Date(int year, int month, int day) : year(year), month(month), day(day) {
        if (month < 1 || month > 12) {
            throw std::invalid_argument("Month value is invalid: " + std::to_string(month));
        }
        if (day < 1 || day > 31) {
            throw std::invalid_argument("Day value is invalid: " + std::to_string(day));
        }
    }

    int GetYear() const { return year; }
    int GetMonth() const { return month; }
    int GetDay() const { return day; }

private:
    int year = -1;
    int month = -1;
    int day = -1;
};

bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() != rhs.GetYear()) {
      return lhs.GetYear() < rhs.GetYear();
    }
    if (lhs.GetMonth() != rhs.GetMonth()) {
      return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetDay() < rhs.GetDay();
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << std::setw(4) << std::setfill('0') << date.GetYear() << '-'
       << std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
       << std::setw(2) << std::setfill('0') << date.GetDay();
    return os;
}

class Database {
public:
    void AddEvent(const Date& date, const std::string& event) {
        events[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const std::string& event) {
        auto it = events.find(date);
        if (it != events.end() && it->second.erase(event)) {
            return true;
        }
        return false;
    }

    int DeleteDate(const Date& date) {
        int count = 0;
        auto it = events.find(date);
        if (it != events.end()) {
            count = it->second.size();
            events.erase(it);
        }
        return count;
    }

    void PrintEventsOnDate(const Date& date) const {
        auto it = events.find(date);
        if (it != events.end()) 
        {
            const auto& events_on_date = it->second;
            for (const auto& event : events_on_date) 
            {
                std::cout << event << std::endl;
            }
        }
    }

    void Print() const {
        for (const auto& pair : events) {
            const auto& date = pair.first;
            const auto& events_on_date = pair.second;

            for (const auto& event : events_on_date) {
                std::cout << date << ' ' << event << '\n';
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> events;
};

std::istringstream& operator>>(std::istringstream& is, Date& date) {
    std::string date_str;
    is >> date_str;
    std::istringstream dateStream(date_str);

    char dash;
    int year, month, day;
    dateStream >> year >> dash >> month >> dash >> day;
    if (dateStream.fail() || dateStream.peek() != EOF) {
        throw std::invalid_argument("Wrong date format: " + date_str);
    }
    date = Date(year, month, day);
    return is;
}

int main() {
    Database db;

    for (std::string line; std::getline(std::cin, line);) {
        std::istringstream is(line);
        std::string command;
        is >> command;
        try {
            if (command == "Add") {
                Date date(1, 1, 1);
                is >> date;
                std::string text;
                is >> text;
                db.AddEvent(date, text);
            } else if (command == "Print") {
                db.Print();
            } else if (command == "Del") {
                Date date(1, 1, 1);
                is >> date;
                std::string event;
                is >> event;
                if (!event.empty()) {
                    if (db.DeleteEvent(date, event)) {
                        std::cout << "Deleted successfully" << std::endl;
                    } else {
                        std::cout << "Event not found" << std::endl;
                    }
                } else {
                    int deletedEvents = db.DeleteDate(date);
                    std::cout << "Deleted " << deletedEvents << " events" << std::endl;
                }
            } else if (command == "Find") {
                Date date(1, 1, 1);
                is >> date;
                db.PrintEventsOnDate(date);
            } else if (command == "Exit") {
                break;
            } else if (!command.empty()) {
                std::cout << "Unknown command: " << command << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
