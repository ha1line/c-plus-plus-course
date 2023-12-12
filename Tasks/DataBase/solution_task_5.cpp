#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

class Date
{
public:
    Date(const int year, const int month, const int day):
            m_year(year),m_month(month),m_day(day)
    {
        if(month<1 || month>12)
        {
            throw std::runtime_error( "Month value is invalid: " + std::to_string(month));
        }
        if(day<1 || day>31)
        {
            throw std::runtime_error( "Day value is invalid: " + std::to_string(day));
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
    std::string FormatDateValue(const int nf_value, const int num) const
    {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(num) << nf_value;
        return ss.str();
    }
private:
    const int m_year;
    const int m_month;
    const int m_day;
};
std::ostream& operator<<(std::ostream& ostream, const Date& date){
    ostream << date.FormatDateValue(date.GetYear(),4) << "-"
            << date.FormatDateValue(date.GetMonth(),2) << "-"
            << date.FormatDateValue(date.GetDay(),2) ;
    return ostream;
}

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
        if (container.contains(date) && container.at(date).contains(event)){
            container[date].erase(event);
            return true;
        }
        return false;
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
        const auto it = container.find(date);
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
            if(pair.first.GetYear()>=0)
            {
                for(const auto& event : pair.second)
                {
                    std::cout << pair.first <<" "<< event << std::endl;
                }
            }
        }
    }
private:
    std::map<Date, std::set<std::string>> container;
};

Date GetDateFromString(const std::string& string)
{
    int year, month, day;
    std::stringstream stream(string);
    char delimiter1;
    char delimiter2;
    stream >> year >> delimiter1 >> month >> delimiter2 >> day;
    if(stream.fail() || !stream.eof() || delimiter1 != '-' || delimiter2 != '-')
    {
        throw std::runtime_error("Wrong date format: " + string);
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
        }catch (const std::exception& exception)
        {
            std::cout << exception.what() << std::endl;
            break;
        }
    }
    return 0;
}
