#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include <tuple>
#include <iomanip>

class Date
{
public:
    Date(const int year, const int month, const int day) : 
    m_year(year),
    m_month(month),
    m_day(day)
    {
        if (m_month < 1 || m_month > 12)
        {
            throw std::out_of_range("Month value is invalid: " + std::to_string(m_month));
        }

        if (m_day < 1 || m_day > 31)
        {
            throw std::out_of_range("Day value is invalid: " + std::to_string(m_day));
        }
    }

    int GetYear() const
    {
        return m_year;
    };
    int GetMonth() const
    {
        return m_month;
    };
    int GetDay() const
    {
        return m_day;
    };

private:
    const int m_year;
    const int m_month;
    const int m_day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
    return std::make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
           std::make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

std::ostream& operator<<(std::ostream& stream, const Date& date)
{
    stream << std::setw(4) << std::setfill('0') << date.GetYear()
           << "-" << std::setw(2) << std::setfill('0') << date.GetMonth()
           << "-" << std::setw(2) << std::setfill('0') << date.GetDay();
    return stream;
}

class Database
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        m_storage[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const std::string& event)
    {
        if (!m_storage.contains(date) || !m_storage[date].contains(event))
        {
            return false;
        }

        m_storage[date].erase(event);
        return true;
    }
    int DeleteDate(const Date& date) noexcept
    {
        if (!m_storage.contains(date))
        {
            return 0;
        }

        size_t event_count = m_storage[date].size();
        m_storage.erase(date);

        return int(event_count);
    }

    std::set<std::string> Find(const Date& date) const
    {
        if (!m_storage.contains(date))
        {
            return {};
        }

        return m_storage.at(date);
    }

    void Print() const noexcept
    {
        for (const auto [date, events] : m_storage)
        {
            for (const auto event : events)
            {
                std::cout << date << " " << event << std::endl;
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> m_storage;
};

Date parse_date(const std::string& date)
{
    std::istringstream dateStream(date);

    int year, month, day;
    dateStream >> year;

    if (dateStream.peek() != '-')
    {
        throw std::runtime_error("Wrong date format: " + date);
    }

    dateStream.ignore(1);
    dateStream >> month;

    if (dateStream.peek() != '-')
    {
        throw std::runtime_error("Wrong date format: " + date);
    }

    dateStream.ignore(1);
    dateStream >> day;

    if (!dateStream.eof())
    {
        throw std::runtime_error("Wrong date format: " + date);
    }

    return Date(year, month, day);
}

int main()
{
    Database database;

    std::string commandLine;
    while (std::getline(std::cin, commandLine))
    {
        if (commandLine.empty())
        {
            continue;
        }

        try
        {
            std::istringstream stream_data(commandLine);

            std::string command;
            stream_data >> command;

            if (command == "Add")
            {
                std::string date, event;
                stream_data >> date >> event;

                database.AddEvent(parse_date(date), event);
            }
            else if (command == "Del")
            {
                std::string date;
                stream_data >> date;

                if (stream_data.eof())
                {
                    std::cout << "Deleted " << database.DeleteDate(parse_date(date)) << " events" << std::endl;
                }
                else
                {
                    std::string event;
                    stream_data >> event;

                    if (database.DeleteEvent(parse_date(date), event))
                    {
                        std::cout << "Deleted successfully" << std::endl;
                    }
                    else
                    {
                        std::cout << "Event not found" << std::endl;
                    }
                }
            }
            else if (command == "Find")
            {
                std::string date;
                stream_data >> date;

                for (const auto& row : database.Find(parse_date(date)))
                {
                    std::cout << row << std::endl;
                }
            }
            else if (command == "Print")
            {
                database.Print();
            }
            else
            {
                throw std::runtime_error("Unknown command: " + command);
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    return 0;
}