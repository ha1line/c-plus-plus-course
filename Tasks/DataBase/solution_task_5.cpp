#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

class Date
{
public:
    Date(const int year, const int month, const int day):
        m_year(year),
        m_month(month),
        m_day(day)
    {
    }
    [[nodiscard]] int GetYear() const
    {
        return m_year;
    }
    [[nodiscard]] int GetMonth() const
    {
        return m_month;
    }
    [[nodiscard]] int GetDay() const
    {
        return m_day;
    }
    [[nodiscard]] std::string ToStr() const
    {
        return std::string(
                formatYear(m_year)+"-"+
                formatMonthOrDay(m_month)+"-"+
                formatMonthOrDay(m_day)
        );
    }
private:
    [[nodiscard]] std::string formatYear(const int nf_year) const
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(4) << nf_year;
        return ss.str();
    }
    [[nodiscard]] std::string formatMonthOrDay(const int nf_monthday) const
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << nf_monthday;
        return ss.str();
    }
    const int m_year;
    const int m_month;
    const int m_day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
    return std::vector<int>{lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()} <
           std::vector<int>{rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()};
}

class Database
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        container[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const std::string& event)
    {
        bool foundDate = container.contains(date);
        bool foundEvent = false;
        if(foundDate)
        {
            foundEvent = container[date].contains(event);
            if(foundEvent )
            {
                container[date].erase(event);
            }
        }
        return foundDate && foundEvent;
    }

    int DeleteDate(const Date& date)
    {
        int num = 0;
        if(container.contains(date))
        {
            num = container[date].size();
            container.erase(date);
        }
        return num;
    }

    [[nodiscard]] std::set<std::string> Find(const Date& date) const
    {
        auto it = container.find(date);
        if(it == container.end())
        {
            return std::set<std::string>{};
        }
        return it->second;
    }

    void Print() const
    {
        for (const auto& pair : container)
        {
            std::string s_date = pair.first.ToStr();
            if(pair.first.GetYear()>=0)
            {
                for(const auto& event : pair.second)
                {
                    std::cout << s_date <<" "<< event << std::endl;
                }
            }
        }
    }
private:
    std::map<Date, std::set<std::string>> container;
};

Date GetDateFromString(const std::string& string)
{
    std::stringstream ss(string);
    int year, month, day;
    try
    {
        std::stringstream stream(string);
        char delimiter1;
        char delimiter2;
        stream >> year >> delimiter1 >> month >> delimiter2 >> day;
        if(stream.fail() || !stream.eof() || delimiter1 != '-' || delimiter2 != '-')
        {
            throw std::runtime_error("Wrong date format: " + string);
        }
    }catch (std::exception)
    {
        throw std::runtime_error("Wrong date format: " + string);
    }
    if(month<1 || month>12)
    {
        throw std::runtime_error( "Month value is invalid: " + std::to_string(month));
    }
    if(day<1 || day>31)
    {
        throw std::runtime_error( "Day value is invalid: " + std::to_string(day));
    }
    return Date{year, month, day};
}
int main()
{
    Database db;
    std::string commandLine;
    while (std::getline(std::cin, commandLine))
    {
        if(commandLine.empty())
        {
            continue;
        }
        std::istringstream is(commandLine);
        std::string command;
        std::string date;
        std::string event;
        is >> command;
        is >> date;
        try
        {
            if(command == "Add")
            {
                is >> event;
                db.AddEvent(GetDateFromString(date),event);
            }else if(command == "Del")
            {
                is >> event;
                if(event.empty())
                {
                    int num = db.DeleteDate(GetDateFromString(date));
                    std::cout << "Deleted "<< num <<" events" << std::endl;
                }else
                {
                    bool status = db.DeleteEvent(GetDateFromString(date), event);
                    if(status)
                    {
                        std::cout << "Deleted successfully" << std::endl;
                    }else
                    {
                        std::cout << "Event not found" << std::endl;
                    }
                }
            }else if(command == "Find")
            {
                std::set<std::string> events = db.Find(GetDateFromString(date));
                if(!events.empty())
                {
                    for(const auto& m_event : events)
                    {
                        std::cout << m_event << std::endl;
                    }
                }
            }else if(command == "Print")
            {
                db.Print();
            }else
            {
                throw std::runtime_error("Unknown command: "+command);
            }
        }catch (std::exception& exception)
        {
            std::cout << exception.what() << std::endl;
            break;
        }
    }
    return 0;
}
