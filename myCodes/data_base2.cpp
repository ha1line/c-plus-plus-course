#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>
#include <iostream>

class Date
{
public:
    Date(int year, int month, int day) : m_year(year),
                                         m_month(month),
                                         m_day(day)
    {
        if (month > 12 || month <= 0)
        {
            throw std::runtime_error("Month value is invalid: " + std::to_string(month));
        }

        if (day > 31 || day <= 0)
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

    static Date ReadDate(std::istream &stream)
    {
        int year, month, day;
        std::string date;
        std::getline(stream, date, ' ');

        if (IsValidDate(year, month, day, date))
        {
            return Date(year, month, day);
        }
    }

private:
    int m_year, m_month, m_day;
    static bool IsValidDate(int &year, int &month, int &day, std::string date)
    {
        std::stringstream is(date);
        char delimiter;
        char delimiter2;
        if (is >> year >> delimiter >> month >> delimiter2 >> day)
        {
            if (delimiter == '-' && delimiter2 == '-' && is.get() == EOF)
            {
                return true;
            }

            else
            {
                throw std::runtime_error("Wrong date format: " + date);
            }
        }

        else
        {
            throw std::runtime_error("Wrong date format: " + date);
        }
    }
};

bool operator<(const Date &lhs, const Date &rhs)
{
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
           std::vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

class Database
{
public:
    void Add(const Date &date, const std::string &event)
    {
        m_dataBase[date].insert(event);
    }

    bool DeleteEvent(const Date &date, const std::string &event)
    {
        if (m_dataBase.contains(date) && m_dataBase[date].contains(event))
        {
            m_dataBase[date].erase(event);
            return true;
        }

        else
        {
            throw std::invalid_argument("Event not found");
        }

        return false;
    }

    int DeleteDate(const Date &date)
    {
        if (m_dataBase.contains(date))
        {
            int countLogs = m_dataBase[date].size();
            m_dataBase.erase(date);
            return countLogs;
        }
        
        return 0;
    }

    std::set<std::string> Find(const Date &date)
    {
        if (m_dataBase.contains(date))
        {
            return m_dataBase[date];
        }

        return std::set<std::string>();
    }

    void Print() const
    {
        for (const auto &pair : m_dataBase)
        {
            for (const auto &log : pair.second)
            {
                std::cout << std::setw(4) << std::setfill('0');
                std::cout << pair.first.GetYear() << '-';
                std::cout << std::setw(2) << std::setfill('0');
                std::cout << pair.first.GetMonth() << '-';
                std::cout << std::setw(2) << std::setfill('0');
                std::cout << pair.first.GetDay() << ' ' << log << std::endl;
            }
        }
    };

private:
    std::map<Date, std::set<std::string>> m_dataBase;
};

int main()
{
    Database dataBase;

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
                Date date = Date::ReadDate(is);
                std::string event;
                is >> event;
                dataBase.Add(date, event);
            }

            else if (command == "Del")
            {
                Date date = Date::ReadDate(is);
                std::string event;
                is >> event;
                if (event.size() == 0)
                {
                    std::cout << "Deleted " << dataBase.DeleteDate(date) << " events" << std::endl;
                }
                else
                {
                    if (dataBase.DeleteEvent(date, event))
                    {
                        std::cout << "Deleted successfully" << std::endl;
                    }
                }
            }

            else if (command == "Find")
            {
                for (const auto &event : dataBase.Find(Date::ReadDate(is)))
                {
                    std::cout << event << std::endl;
                }
            }

            else if (command == "Print")
            {
                dataBase.Print();
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
        catch (const std::exception &e)
        {
            std::cout << e.what() << '\n';
        }
    }

    return 0;
}
