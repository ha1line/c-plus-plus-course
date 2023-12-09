// вы можете подключить все необходимые вам библиотеки из стандарта
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <set>
#include <exception>

// нужно заполнить методы и перегрузить нужные операторы
class Date
{
public:
    Date(int year, int month, int day) : 
        m_year(year),
        m_month(month),
        m_day(day)
    {}

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

    static Date ParseDate(std::istringstream& is)
    {
        int year, month, day;

        std::string date;
        is >> std::ws;
        std::getline(is, date, ' ');

        std::istringstream stream(date);

        if (!isdigit(date[0]) || !isdigit(date.back()))
        {
            throw std::runtime_error("Wrong date format: " + date);
        }

        stream >> year;
        CheckSeparator(stream, date);

        stream >> month;
        if (month < 1 || month > 12)
        {
            throw std::runtime_error("Month value is invalid: " + std::to_string(month));
        }

        CheckSeparator(stream, date);

        stream >> day;
        if (day < 1 || day > 31)
        {
            throw std::runtime_error("Day value is invalid: " + std::to_string(day));
        }

        return Date(year, month, day);
    }
private:
    int m_year;
    int m_month;
    int m_day;

    static void CheckSeparator(std::istringstream& stream, const std::string& date)
    {
        if (stream.get() != '-')
        {
            throw std::runtime_error("Wrong date format: " + date);
        }
    }
};

// здесь советую воспользоваться трюком с вектором из лекции, либо std::tie
bool operator<(const Date& lhs, const Date& rhs)
{
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} < 
        std::vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

class DataBase
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        if (m_db.find(date) != m_db.end())
        {
            m_db[date].insert(event);
        }
        else
        {
            m_db[date] = std::set<std::string>{event};
        }
    }

    bool Delete(const Date& date, const std::string& event)
    {
        auto iterator = m_db.find(date);
        if (iterator == m_db.end())
        {
            return false;
        }

        std::set<std::string>& events = iterator->second;

        auto eventIterator = std::find(events.begin(), events.end(), event);
        if (eventIterator == events.end())
        {
            return false;
        }

        events.erase(eventIterator);

        if (events.empty())
        {
            m_db.erase(iterator);
        }

        return true;
    }

    int Delete(const Date& date)
    {
        int eventsCount = m_db[date].size();
        m_db.erase(date);

        return eventsCount;
    }
    
    std::set<std::string> Find(const Date& date)
    {
        return m_db[date];
    }

    void Print() const
    {
        for (const auto& pair : m_db)
        for (const auto& event : pair.second)
        {
            std::cout << std::setw(4) << std::setfill('0');
            std::cout << pair.first.GetYear() << '-';
            std::cout << std::setw(2) << std::setfill('0');
            std::cout << pair.first.GetMonth() << '-';
            std::cout << std::setw(2) << std::setfill('0');
            std::cout << pair.first.GetDay() << ' ' << event << std::endl;
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
            if (command == "Add")
            {
                Date date = Date::ParseDate(is);
                std::string event;

                is >> std::ws;
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
                    if (!db.Delete(date, event))
                    {
                        std::cout << "Event not found" << std::endl;
                    }
                    else
                    {
                        std::cout << "Deleted successfully" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Deleted "<< db.Delete(date) << " events" << std::endl;
                }
            }
            else if (command == "Find")
            {
                Date date = Date::ParseDate(is);
                std::set<std::string> events = db.Find(date);

                for (const auto& event : events)
                {
                    std::cout << event << std::endl;
                }
            }
            else if (command == "Print")
            {
                db.Print();
            }
            else if (command == "")
            {
                continue;
            }
            else
            {
                throw std::runtime_error("Unknown command: " + command);
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            //return 1;
        }

    }

    return 0;
}