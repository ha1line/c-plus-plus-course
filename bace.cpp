#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>

class Date
{
public:
    Date(short year, short month, short day) : m_year(year), m_month(month), m_day(day) {}

    short GetYear() const
    {
        return m_year;
    }

    short GetMonth() const
    {
        return m_month;
    }

    short GetDay() const
    {
        return m_day;
    }
private:
    const short m_year,
        m_month,
        m_day;
};

// здесь советую воспользоваться трюком с вектором из лекции, либо std::tie
bool operator<(const Date& lhs, const Date& rhs)
{
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
        std::vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

std::ostream& operator<<(std::ostream& os, const Date& date) {

    os  << std::setw(4) << std::setfill('0') << date.GetYear() << '-'
        << std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
        << std::setw(2) << std::setfill('0') << date.GetDay();
    return os;
}

Date readDate(const std::string& read)
{
    std::istringstream date_is(read);
    short year, month, day;
    char delimiter;
    date_is >> year >> delimiter >> month;
    if (delimiter != '-')
    {
        std::cout << "Wrong date format: " << read << '\n';
        throw std::exception();
    }
    date_is >> delimiter >> day;
    if (delimiter != '-' || date_is.fail() || !date_is.eof() || year > 9999 || month > 99 || day > 99)
    {
        std::cout << "Wrong date format: " << read << '\n';
        throw std::exception();
    }
    if (month < 1 or month > 12)
    {
        std::cout << "Month value is invalid: " << month << '\n';
        throw std::exception();
    }
    if (day < 1 or day > 31)
    {
        std::cout << "Day value is invalid: " << day << '\n';
        throw std::exception();
    }
    return Date(year, month, day);
}


class Database
{
public:
    void AddEvent(const Date& date, const std::string& event);
    bool DeleteEvent(const Date& date, const std::string& event);
    size_t  DeleteDate(const Date& date);

    std::set<std::string> Find(const Date& date) const;

    void Print() const;
private:
    std::map<Date, std::set<std::string>> bace;
};

void Database::AddEvent(const Date& date, const std::string& event) 
{
    bace[date].insert(event);
}

bool Database::DeleteEvent(const Date& date, const std::string& event)
{
    if (!bace.contains(date) || !bace[date].contains(event))
        return false;
    return bace[date].erase(event) > 0;
}

size_t Database::DeleteDate(const Date& date)
{
    if (!bace.contains(date))
        return 0;
    size_t result = bace[date].size();
    bace.erase(date);
    return result;
}

std::set<std::string> Database::Find(const Date& date) const
{
    if (!bace.contains(date)) return {};
    return bace.at(date);
}

void Database::Print() const
{
    for (const auto& [date, events] : bace)
    {
        for (const auto& event : events)
        {
            std::cout << date << ' ' << event << '\n';
        }
    }
}

int main()
{
    Database db;

    std::string commandLine;
    while (std::getline(std::cin, commandLine))
    {
        if (commandLine.size() == 0)
            continue;

        std::string nameCommand;
        std::stringstream stream(commandLine);
        stream >> nameCommand;
        try
        {
            if (nameCommand == "Add")
            {
                std::string event, read;
                stream >> read;
                Date date = readDate(read);
                stream  >> event;
                db.AddEvent(date, event);
            }
            else if (nameCommand == "Del")
            {
                std::string event, read;
                stream >> read;
                Date date = readDate(read);
                if (stream.eof())
                {
                    std::cout << "Deleted " << db.DeleteDate(date) << " events\n";
                }
                else
                {
                    stream >> event;
                    if (db.DeleteEvent(date, event))
                        std::cout << "Deleted successfully\n";
                    else
                        std::cout << "Event not found\n";
                }
            }
            else if (nameCommand == "Find")
            {
                std::string read;
                stream >> read;
                Date date = readDate(read);
                for (const auto& n : db.Find(date))
                {
                    std::cout << n << '\n';
                }
            }
            else if (nameCommand == "Print")
                db.Print();
            else
            {
                std::cout << "Unknown command: " << nameCommand << '\n';
                break;
            }
        }
        catch (const std::runtime_error&)
        {
            break;
        }
    }

    return 0;
}

