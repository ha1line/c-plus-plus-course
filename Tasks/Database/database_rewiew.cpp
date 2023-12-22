#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <set>

class Date
{
public:
    Date(int year, int month, int day){
        if (month < 1 || month > 12)
            throw std::runtime_error("Month value is invalid: " + std::to_string(month));
        if (day < 1 || day > 31)
            throw std::runtime_error("Day value is invalid: " + std::to_string(day));
        m_year = year; m_month = month; m_day = day;
    };
    bool operator< (const Date& other) const
    {
        return std::vector<int>{m_year, m_month, m_day} < std::vector<int>{other.m_year, other.m_month, other.m_day};
    }
    int m_year;
    int m_month;
    int m_day;
};

std::ostream& operator<< (std::ostream& os,const Date& date)
{
    os << std::setfill('0') << std::setw(4) << date.m_year << '-' <<
       std::setw(2) << date.m_month << '-' <<
       std::setw(2) << date.m_day << " ";
    return os;
}

class DataBase
{
public:
    void Add(const Date& date, const std::string& event)
    {
        database[date].insert(event);
    }
    void DeleteEvent(const Date& date, const std::string& event)
    {
        if (database.count(date) && database[date].erase(event))
            std::cout << "Deleted successfully" << std::endl;
        else
            std::cout << "Event not found" << std::endl;
    }
    void DeleteDate(const Date& date)
    {
        int count = 0;
        if (database.count(date))
        {
            count = database[date].size();
            database.erase(date);
        }
        std::cout << "Deleted " << count << " events" << std::endl;
    }
    void Find(const Date& date)
    {
        if (database.count(date)) {
            std::set<std::string> set = database[date];
            for (const auto &event: set)
                std::cout << event << std::endl;
        }
    }
    void Print()
    {
        for (const auto & date : database)
        {
            std::set<std::string> set = database[date.first];
            for (const auto & event : set)
            {
                std::cout << date.first << event << std::endl;
            }
        }
    }
private:
    std::map<Date, std::set<std::string>> database;
};

Date ReadDate(std::istream& stream)
{
    std::string string; stream >> string;
    std::istringstream is(string);

    int year; int month; int day;
    is >> year;
    is.ignore(1);
    is >> month;
    is.ignore(1);
    is >> day;
    char peek = is.peek();
    if (is.fail() && !is.eof() || peek != -1)
        throw std::runtime_error("Wrong date format: " + string);
    return {year, month, day};
}

int main()
{
    DataBase dataBase;
    std::string commandLine;
    try {
        while (std::getline(std::cin, commandLine)) {
            std::istringstream is(commandLine);
            std::string command;
            is >> command;
            if (command.empty())
                continue;
            else if (command == "Add") {
                Date date = ReadDate(is);
                std::string event;
                is >> event;
                dataBase.Add(date, event);
            } else if (command == "Del") {
                Date date = ReadDate(is);
                std::string event;
                is >> event;
                if (event.empty())
                    dataBase.DeleteDate(date);
                else
                    dataBase.DeleteEvent(date, event);
            } else if (command == "Find") {
                Date date = ReadDate(is);
                dataBase.Find(date);
            } else if (command == "Print")
                dataBase.Print();
            else
                throw std::runtime_error("Unknown command: " + commandLine);
        }
    }
    catch (const std::runtime_error& error)
    {
        std::cout << error.what() << std::endl;
    }

    return 0;
}
