#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream> 
#include <iomanip>
#include <vector>
#include <exception>
#include <array>
class Date
{
public:
    Date(int year, int month, int day):
    m_year(year), 
    m_month(month), m_day(day)
    {
        if (m_month < 1 || m_month > 12)
        {
            throw std::invalid_argument("Month value is invalid: " + std::to_string(m_month));
        }
        if (m_day < 1 || m_day > 31)
        {
            throw std::invalid_argument("Day value is invalid: " + std::to_string(m_day));
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
private:
    const int m_year;
    const int m_month;
    const int m_day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() == rhs.GetYear())
    {
        if (lhs.GetMonth() == rhs.GetMonth())
        {
            return lhs.GetDay() < rhs.GetDay();
        }
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetYear() < rhs.GetYear();
};

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    os << std::setfill('0') << std::setw(4)<<date.GetYear() << "-"
        << std::setfill('0') << std::setw(2)<<date.GetMonth() << "-"
            << std::setfill('0') << std::setw(2)<<date.GetDay();
    return os;
};

class Database
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        m_dataBase[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const std::string& event)
    {
        if (m_dataBase.contains(date) && m_dataBase[date].contains(event)) //countains
        {
           m_dataBase[date].erase(event);
           std::cout << "Deleted successfully" << std::endl;
           return true;
        }
        std::cout << "Event not found" << std::endl;
        return false;
    }

    int  DeleteDate(const Date& date)
    {
        const size_t countEventOnDate = m_dataBase[date].size();
        if (m_dataBase.contains(date)) //countains
        {
            m_dataBase.erase(date);
            std::cout << "Deleted" << " " << countEventOnDate << " " << "events" << std::endl;
        }
        return countEventOnDate;
    }
  
    std::set<std::string> Find(const Date& date) const
    {
        if (!m_dataBase.contains(date)) //countains
        {
            return {};
        }
        return m_dataBase.at(date);
    }
    
    void Print() const
    {
        for (const auto& [date, events] : m_dataBase)
        {
            for (const auto& event : events)
            {
                std::cout << date << " " << event << std::endl;
            }
        }
    }
private:
    std::map<Date, std::set<std::string>> m_dataBase;
};

Date date_prossesing(std::string& date)
{
    std::istringstream stream(date);
    std::array <int,3> shaterredDate;
    for (size_t j = 0; j != shaterredDate.size(); ++j)
    {
        int partDate;
        stream >> partDate;
        if ((stream.peek() != '-' && j != 2) || (!stream.eof() && j == 2))
        {
            std::string message = "Wrong date format: " + date;
            throw std::invalid_argument(message);
        }
        shaterredDate[j] = partDate;
        stream.ignore(1);
    }
    return Date(shaterredDate[0], shaterredDate[1], shaterredDate[2]);
};

int main()
{
    Database db;
    std::string commandLine;
    while (std::getline(std::cin, commandLine))
    {
        if (commandLine.size() == 0)
        {
            continue;
        }  
        try
        {
            std::string nameCommand;
            std::istringstream stream(commandLine);
            std::string dateToString, event;
            stream >> nameCommand;
            
            if (nameCommand == "Print")
            {
                db.Print();
            }
            else if (nameCommand == "Add")
            {
                stream >> dateToString;
                Date fullDate = date_prossesing(dateToString);
                stream >> event;
                db.AddEvent(fullDate,event);
            }
            else if (nameCommand == "Del")
            {
                stream >> dateToString;
                Date fullDate = date_prossesing(dateToString);
                stream >> event;
                if (event.empty())
                {
                    db.DeleteDate(fullDate);
                }
                else
                {
                    db.DeleteEvent(fullDate,event);
                }
            }
            else if (nameCommand == "Find")
            {
                stream >> dateToString;
                Date fullDate = date_prossesing(dateToString);
                for (const auto& event : db.Find(fullDate))
                {
                    std::cout << event << std::endl;
                }
            }
            else
            {
                std::string message ="Unknown command: " + nameCommand;
                throw std::invalid_argument(message);
            }
        }
        catch(const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return 0;
        }
    }
    return 0;
};