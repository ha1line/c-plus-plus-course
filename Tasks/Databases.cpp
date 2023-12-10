#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include <tuple>
#include <iomanip>

using std::string, std::out_of_range, std::to_string, std::ostream, std::make_tuple, std::set, std::cout,
        std::endl, std::map, std::runtime_error, std::getline, std::exception, std::cin;

class Date
{
public:
    Date(const int year, const int month, const int day) : m_year(year), m_month(month), m_day(day)
    {
        if (m_month < 1 || m_month > 12)
        {
            throw out_of_range("Month value is invalid: " + to_string(m_month));
        }

        if (m_day < 1 || m_day > 31)
        {
            throw out_of_range("Day value is invalid: " + to_string(m_day));
        }
    }

    [[nodiscard]] int GetYear() const noexcept
    {
        return m_year;
    }

    [[nodiscard]] int GetMonth() const noexcept
    {
        return m_month;
    }

    [[nodiscard]] int GetDay() const noexcept
    {
        return m_day;
    }

private:
    const int m_year;
    const int m_month;
    const int m_day;
};

ostream& operator<<(ostream& stream, const Date& date)
{
    stream << std::setw(4) << std::setfill('0') << date.GetYear()
           << "-" << std::setw(2) << std::setfill('0') << date.GetMonth()
           << "-" << std::setw(2) << std::setfill('0') << date.GetDay();
    return stream;
}

bool operator<(const Date& lhs, const Date& rhs) noexcept
{
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
           make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

class Database
{
public:
    void AddEvent(const Date& date, const string& event) noexcept
    {
        m_storage[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event) noexcept
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

    [[nodiscard]] set<string> Find(const Date& date) const noexcept
    {
        if (!m_storage.contains(date))
        {
            return {};
        }

        return m_storage.at(date);
    }

    void Print() const noexcept
    {
        for (const auto& [date, events]: m_storage)
        {
            for (const auto& event: events)
            {
                cout << date << " " << event << endl;
            }
        }
    }

private:
    map<Date, set<string>> m_storage;
};

Date parse_date(const string& date)
{
    std::istringstream stream_date(date);

    int year, month, day;
    stream_date >> year;

    if (stream_date.peek() != '-')
    {
        throw runtime_error("Wrong date format: " + date);
    }

    stream_date.ignore(1);
    stream_date >> month;

    if (stream_date.peek() != '-')
    {
        throw runtime_error("Wrong date format: " + date);
    }

    stream_date.ignore(1);
    stream_date >> day;

    if (!stream_date.eof())
    {
        throw runtime_error("Wrong date format: " + date);
    }

    return {year, month, day};
}

void processing_command(Database& database, const string& commandLine)
{
    std::istringstream stream_data(commandLine);

    string command;
    stream_data >> command;

    if (command == "Add")
    {
        string date, event;
        stream_data >> date >> event;

        database.AddEvent(parse_date(date), event);
    } else if (command == "Del")
    {
        string date;
        stream_data >> date;

        if (stream_data.eof())
        {
            cout << "Deleted " << database.DeleteDate(parse_date(date)) << " events" << endl;
        } else
        {
            string event;
            stream_data >> event;

            if (database.DeleteEvent(parse_date(date), event))
            {
                cout << "Deleted successfully" << endl;
            } else
            {
                cout << "Event not found" << endl;
            }
        }
    } else if (command == "Find")
    {
        string date;
        stream_data >> date;

        for (const auto& row: database.Find(parse_date(date)))
        {
            cout << row << endl;
        }
    } else if (command == "Print")
    {
        database.Print();
    } else
    {
        throw runtime_error("Unknown command: " + command);
    }
}

int main()
{
    Database db;
    string commandLine;

    while (getline(cin, commandLine))
    {
        if (commandLine.empty())
        {
            continue;
        }

        try
        {
            processing_command(db, commandLine);
        } catch (exception& e)
        {
            cout << e.what() << endl;
        }
    }

    return 0;
}
