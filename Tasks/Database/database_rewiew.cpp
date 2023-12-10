#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <set>

class Date
{
public:
    Date(int year, int month, int day): m_year(year), m_month(month), m_day(day) {};
    Date(std::istream& stream)
    {
        int stops[2] = {0, 0};
        std::string string;
        stream >> string;
        for (int i = 1; i < string.size(); i++)
        {
            if (string[i] == '-' && isdigit(string[i-1]))
            {
                if (stops[0] == 0)
                    stops[0] = i;
                else
                    stops[1] = i;
            }
        }
        int year; int month; int day;
        try {
            year = std::stoi(string.substr(0, stops[0]));
            month = std::stoi(string.substr(stops[0] + 1, stops[1] - stops[0] - 1));
            day = std::stoi(string.substr(stops[1] + 1));
        }
        catch(std::invalid_argument& error)
        {
            throw std::runtime_error("Wrong date format: " + string);
        }

        if (month < 1 || month > 12)
            throw std::runtime_error("Month value is invalid: " + std::to_string(month));
        if (day < 1 || day > 31)
            throw std::runtime_error("Day value is invalid: " + std::to_string(day));
        m_year = year; m_month = month; m_day = day;
    }
    void PrintDate() const {
        std::cout << std::setfill('0') << std::setw(4) << m_year << '-' <<
                     std::setw(2) << m_month << '-' <<
                     std::setw(2) << m_day << " ";
    }
    bool operator<(Date const& other) const
    {
        return std::vector<int>{m_year, m_month, m_day} <std::vector<int>{other.m_year, other.m_month, other.m_day};
    }

private:
    int m_year;
    int m_month;
    int m_day;
};

class DataBase
{
public:
    void Add(const Date& date, const std::string& event)
    {
        database[date].insert(event);
    }
    void DeleteEvent(const Date& date, const std::string& event)
    {
        if (database[date].erase(event))
            std::cout << "Deleted successfully" << std::endl;
        else
            std::cout << "Event not found" << std::endl;
    }
    void DeleteDate(const Date& date)
    {
        std::cout << "Deleted " << database[date].size() << " events" << std::endl;
        database[date].clear();
    }
    void Find(const Date& date)
    {
        std::set<std::string> set = database[date];
        for (const auto & i : set)
            std::cout << i << std::endl;
    }
    void Print()
    {
        for (const auto & i : database)
        {
            if (database[i.first].empty())
                continue;
            std::set<std::string> set = database[i.first];
            for (const auto & j : set)
            {
                i.first.PrintDate();
                std::cout << j << std::endl;
            }
        }
    }
private:
    std::map<Date, std::set<std::string>> database;
};

int main()
{
    DataBase dataBase;
    std::string command;
    try {
        for (std::string line; std::getline(std::cin, line);) {
            std::istringstream is(line);
            if (is.rdbuf()->in_avail() == 0)
                continue;
            is >> command;
            if (command == "Add")
            {
                Date date = Date(is);
                std::string event;
                is >> event;
                dataBase.Add(date, event);
            }
            else if (command == "Del")
            {
                Date date = Date(is);
                if(is.rdbuf()->in_avail() == 0)
                    dataBase.DeleteDate(date);
                else
                {
                    std::string event;
                    is >> event;
                    dataBase.DeleteEvent(date, event);
                }
            }
            else if (command == "Find")
            {
                Date date = Date(is);
                dataBase.Find(date);
            }
            else if (command == "Print")
                dataBase.Print();
            else
                throw std::runtime_error("Unknown command: " + command);
        }
    }
    catch (std::runtime_error& error)
    {
        std::cout << error.what() << std::endl;
    }
    return 0;
}