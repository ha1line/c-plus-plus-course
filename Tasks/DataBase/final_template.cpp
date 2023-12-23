#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>
#include <string>
#include <memory>

struct DateStruct {
    int year = -1;
    int month = -1;
    int day = -1;
};

bool operator<(const DateStruct& lhs, const DateStruct& rhs) {
    return std::tie(lhs.year, lhs.month, lhs.day) < std::tie(rhs.year, rhs.month, rhs.day);
}

class Date {
public:
    Date() = default;
    Date(int year, int month, int day) : date_{year, month, day} {}

    std::string GetFullDate() const {
        std::stringstream ss;
        ss << std::setw(4) << std::setfill('0') << date_.year << '-';
        ss << std::setw(2) << std::setfill('0') << date_.month << '-';
        ss << std::setw(2) << std::setfill('0') << date_.day;
        return ss.str();
    }

    int GetYear() const { return date_.year; }
    int GetMonth() const { return date_.month; }
    int GetDay() const { return date_.day; }

private:
    DateStruct date_;
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

    Date Find(const Date& date) const {
        auto it = events.find(date);
        if (it != events.end()) {
            return it->first;
        }
        return Date();
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
                std::cout << date.GetFullDate() << ' ' << event << '\n';
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> events;
};

class InvalidDateFormatException : public std::exception {
public:
    InvalidDateFormatException(const std::string& date) : error_message("Wrong date format: " + date) {}

    const char* what() const noexcept override { return error_message.c_str(); }

private:
    std::string error_message;
};

class InvalidMonthException : public std::exception {
public:
    InvalidMonthException(int month) : error_message("Month value is invalid: " + std::to_string(month)) {}

    const char* what() const noexcept override { return error_message.c_str(); }

private:
    std::string error_message;
};

class InvalidDayException : public std::exception {
public:
    InvalidDayException(int day) : error_message("Day value is invalid: " + std::to_string(day)) {}

    const char* what() const noexcept override { return error_message.c_str(); }

private:
    std::string error_message;
};

void HandleDateExceptions(int year, int month, int day) {
    if (month < 1 || month > 12) {
        throw InvalidMonthException(month);
    }
    if (day < 1 || day > 31) {
        throw InvalidDayException(day);
    }
}

int main() {
    Database db;

    for (std::string line; std::getline(std::cin, line);) {
        std::istringstream is(line);
        std::string command;
        is >> command;
        try {
            if (command == "Add") {
                int year, month, day;
                char dash;
                std::string full_date;
                is >> full_date;
                std::istringstream dateStream(full_date);
                dateStream >> year >> dash >> month >> dash >> day;
                if (dateStream.fail() || dateStream.peek() != EOF) {
                    throw InvalidDateFormatException(full_date);
                }
                HandleDateExceptions(year, month, day);
                std::string text;
                is >> text;
                db.AddEvent(Date(year, month, day), text);
            } else if (command == "Print") {
                db.Print();
            } else if (command == "Del") {
                int year, month, day;
                char dash;
                std::string date_str;
                is >> date_str;
                std::istringstream dateStream(date_str);
                dateStream >> year >> dash >> month >> dash >> day;
                if (dateStream.fail() || dateStream.peek() != EOF) {
                    throw InvalidDateFormatException(date_str);
                }
                HandleDateExceptions(year, month, day);
                std::string event;
                is >> event;
                if (!event.empty()) {
                    if (db.DeleteEvent(Date(year, month, day), event)) {
                        std::cout << "Deleted successfully" << std::endl;
                    } else {
                        std::cout << "Event not found" << std::endl;
                    }
                } else {
                    int deletedEvents = db.DeleteDate(Date(year, month, day));
                    std::cout << "Deleted " << deletedEvents << " events" << std::endl;
                }
            } else if (command == "Find") {
                int year, month, day;
                char dash;
                std::string date_str;
                is >> date_str;
                std::istringstream dateStream(date_str);
                dateStream >> year >> dash >> month >> dash >> day;
                if (dateStream.fail() || dateStream.peek() != EOF) {
                    throw InvalidDateFormatException(date_str);
                }
                HandleDateExceptions(year, month, day);
                db.PrintEventsOnDate(Date(year, month, day));
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
