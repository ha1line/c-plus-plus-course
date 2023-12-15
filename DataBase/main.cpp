#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <regex>
#include <set>

class Date
{
public:
    explicit Date(std::string&  date)
    {
        const std::regex r(R"(([+-]{0,1}(\d{1,}))-([+-]{0,1}(\d{1,}))-([+-]{0,1}(\d{1,})))");
        if (!std::regex_match(date,r))
        {
            throw std::runtime_error("Wrong date format: " + date);
        }
        else
        {
            {
                std::regex year(R"(([+-]{0,})\b(\d{1,}))");
                std::smatch y;
                std::regex_search(date, y, year);
                m_year = std::stoi(y[0]);
            }
            {
                std::regex month(R"(\b\-([+-]{0,})(\d{1,})\b)");
                std::smatch m;
                std::regex_search(date,m,month);
                std::string stringMonth = m[0];
                stringMonth.erase(0,1);
                m_month = std::stoi(stringMonth);
                if (m_month > 12 || m_month < 1)
                    throw std::runtime_error("Month value is invalid: " + stringMonth);
            }
            {
                std::regex day(R"(\b\-([+-]{0,})(\d{1,})$)");
                std::smatch d;
                std::regex_search(date,d,day);
                std::string stringDay = d[0];
                stringDay.erase(0, 1);
                m_day = std::stoi(stringDay);
                if (m_day > 31 || m_day < 1)
                    throw std::runtime_error("Day value is invalid: " + stringDay);
            }
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
    int m_year;
    int m_month;
    int m_day;
};

std::ostream& operator <<(std::ostream& os, Date date)
{
    os << std::setfill('0');
    os << std::setw(4) << date.GetYear()  << "-";
    os << std::setw(2) << date.GetMonth()  << "-" << std::setw(2) << date.GetDay();
    return os;
}

bool operator<(const Date& lhs, const Date& rhs)
{
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
    std::vector<int>{rhs.GetYear(),rhs.GetMonth(), rhs.GetDay()};
}

class Database
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        m_database[date].insert(event);
    }

    void DeleteEvent(const Date& date, const std::string& event)
    {
        if (m_database.contains(date) && m_database[date].contains(event))
        {
            m_database[date].erase(event);
            std::cout << "Deleted successfully" << std::endl;
        }
        else std::cout << "Event not found" << std::endl;
    }

    int  DeleteDate(const Date& date)
    {
        int events_num = 0;
        if (m_database.contains(date))
        {
            events_num = m_database[date].size();
            m_database.erase(date);
        }
        return events_num;
    }

    // будет зависеть от контейнера, который вы выберете
    std::set<std::string> Find(const Date& date) const
    {
        if (m_database.contains(date))
        {
            std::set<std::string> events = m_database.at(date);
            return events;
        }
        else return std::set<std::string>{};
    };

    void Print() const
    {
        for (const auto& keyValue : m_database)
        {
            Date date = keyValue.first;
            std::set<std::string> events = keyValue.second;
            for (const auto& event:events)
            {
                std::cout << date << " " << event << std::endl;
            }
        }
    };
private:
    std::map <Date, std::set<std::string>> m_database;
};

int main()
{
    Database db;

    std::string commandLine;
    while (std::getline(std::cin, commandLine))
    {
        std::istringstream is(commandLine);
        std::string command;
        is >> command;
        if (command.empty()) continue;
        else
        {
            std::string stringDate, event;
            is >> stringDate >> event;
            if(command == "Print")
            {
                db.Print();
            }
            else
            {
                try
                {
                    if (command == "Add")
                    {
                        Date date(stringDate);
                        db.AddEvent(date, event);
                    }

                    else if (command == "Del")
                    {
                        Date date(stringDate);
                        if (event.empty())
                        {
                            std::cout << "Deleted " << db.DeleteDate(date) << " events" << std::endl;
                        }
                        else db.DeleteEvent(date, event);
                    }

                    else if (command == "Find")
                    {
                        Date date(stringDate);
                        std::set<std::string> events = db.Find(date);
                        for (const auto &record: events)
                        {
                            std::cout << record << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "Unknown command: " << command << std::endl;
                        break;
                    }
                }
                catch (std::runtime_error& err)
                {
                    std::cout << err.what() << std::endl;
                    break;
                }
            }
        }
    }
    return 0;
}
