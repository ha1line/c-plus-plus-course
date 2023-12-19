#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>
#include <algorithm>

class Date
{
public:
    Date(int year, int month, int day) : m_year(year),
                                         m_month(month),
                                         m_day(day)
    {
        if (month < 1 || month > 12)
        {
            throw std::runtime_error("Month value is invalid: " + std::to_string(month));
        }

        if (day < 1 || day > 31)
        {
            throw std::runtime_error("Day value is invalid: " + std::to_string(day));
        }
    }

    int GetYear() const
    {
        return m_year;
    }

    int GetMonth() const
    {
        return m_month;
    }

    int GetDay() const
    {
        return m_day;
    }

    static Date ParseDate(std::istringstream &is)
    {
        int year, month, day;
        char sep1, sep2;

        std::string date;
        std::getline(is, date, ' ');

        std::istringstream stream(date);

        if (!(stream >> year >> sep1 >> month >> sep2 >> day))
        {
            throw std::runtime_error("Wrong date format: " + date);
        }

        else if (sep1 == '-' && sep2 == '-' && stream.get() == EOF)
        {
            return Date(year, month, day);
        }

        else
        {
            throw std::runtime_error("Wrong date format: " + date);
        }
    }

private:
    int m_year;
    int m_month;
    int m_day;
};

bool operator<(const Date &lhs, const Date &rhs)
{
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
           std::vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

class DataBase
{
public:
    void AddEvent(const Date &date, const std::string &event)
    {
        m_db[date].insert(event);
    }

    bool Delete(const Date &date, const std::string &event)
    {
        if (m_db.contains(date) && m_db[date].contains(event))
        {
            m_db[date].erase(event);
            return true;
        }

        return false;
    }

    int Delete(const Date &date)
    {
        if (m_db.contains(date))
        {
            int deleteCount = m_db[date].size();
            m_db.erase(date);

            return deleteCount;
        }

        return 0;
    }

    std::set<std::string> Find(const Date &date)
    {
        if (m_db.contains(date) && m_db[date].size() > 0)
        {
            return m_db[date]; 
        }

        return std::set<std::string>();
    }

    void Print() const
    {
        for (const auto &pair : m_db)
        {
            for (const auto &event : pair.second)
            {
                std::cout << std::setw(4) << std::setfill('0');
                std::cout << pair.first.GetYear() << '-';
                std::cout << std::setw(2) << std::setfill('0');
                std::cout << pair.first.GetMonth() << '-';
                std::cout << std::setw(2) << std::setfill('0');
                std::cout << pair.first.GetDay() << ' ' << event << std::endl;
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> m_db;
};

int main()
{
    DataBase db;

    std::string commandLine;
    while (std::getline(std::cin, commandLine))
    {
        std::istringstream is(commandLine);

        std::string command;
        is >> command;

        try
        {
            is >> std::ws;

            if (command == "Add")
            {
                Date date = Date::ParseDate(is);

                std::string event;
                is >> event;

                db.AddEvent(date, event);
            }
            else if (command == "Del")
            {
                Date date = Date::ParseDate(is);

                std::string event;
                is >> event;

                if (!event.empty())
                {
                    if (db.Delete(date, event))
                    {
                        std::cout << "Deleted successfully" << std::endl;
                    }
                    else
                    {
                        throw std::runtime_error("Event not found");
                    }
                }
                else
                {
                    std::cout << "Deleted " << db.Delete(date) << " events" << std::endl;
                }
            }
            else if (command == "Find")
            {
                Date date = Date::ParseDate(is);
                std::set<std::string> events = db.Find(date);

                for (const auto &event : events)
                {
                    std::cout << event << std::endl;
                }
            }
            else if (command == "Print")
            {
                db.Print();
            }
            else if (command.empty())
            {
                continue;
            }
            else
            {
                throw std::runtime_error("Unknown command: " + command);
            }
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}