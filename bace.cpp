#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>

static std::set<std::string> empty = {};

class Date
{
public:
    Date() = default;
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
    short m_year,
          m_month,
          m_day;
};

// здесь советую воспользоваться трюком с вектором из лекции, либо std::tie
bool operator<(const Date& lhs, const Date& rhs)
{
    short y1 = lhs.GetYear();
    short y2 = rhs.GetYear();
    short m1 = lhs.GetMonth();
    short m2 = rhs.GetMonth();
    short d1 = lhs.GetDay();
    short d2 = rhs.GetDay();
    return (std::tie(y1, m1, d1) < std::tie(y2, m2, d2));
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    bool negative = date.GetYear() < 0;
    os << (negative ? "-" : "") << std::setw(4) << std::setfill('0') << std::abs(date.GetYear()) << '-'
        << std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
        << std::setw(2) << std::setfill('0') << date.GetDay();
    return os;
}

std::istream& operator>>(std::istream& is, Date& date)
{
    std::string date_buffer;
    is >> date_buffer;
    std::istringstream date_is(date_buffer);
    short year, month, day;
    char delimiter;
    date_is >> year >> delimiter >> month;
    if (delimiter != '-')
    {
        std::cout << "Wrong date format: " << date_buffer << '\n';
        throw std::runtime_error("Wrong date format");
    }
    date_is >> delimiter >> day;
    if (delimiter != '-' || date_is.fail() || !date_is.eof() || year > 9999 || month > 99 || day > 99)
    {
        std::cout << "Wrong date format: " << date_buffer << '\n';
        throw std::runtime_error("Wrong date format");
    }
    if (month < 1 or month > 12)
    {
        std::cout << "Month value is invalid: " << month << '\n';
        throw std::runtime_error("Wrong month value");
    }
    if (day < 1 or day > 31)
    {
        std::cout << "Day value is invalid: " << day << '\n';
        throw std::runtime_error("Wrong day value");
    }
    date = Date(year, month, day);
    return is;
}


class Database
{
public:
    void AddEvent(const Date& date, const std::string& event);
    bool DeleteEvent(const Date& date, const std::string& event);
    size_t  DeleteDate(const Date& date);

    const std::set<std::string>& Find(const Date& date) const;

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

const std::set<std::string>& Database::Find(const Date& date) const
{
    if (!bace.contains(date)) return empty;
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
                std::string event;
                Date date;
                stream >> date >> event;
                db.AddEvent(date, event);
            }
            else if (nameCommand == "Del")
            {
                std::string event;
                Date date;
                stream >> date;
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
                Date date;
                stream >> date;
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
        catch (const std::runtime_error& )
        {
            break;
        }
    }

    return 0;
}
