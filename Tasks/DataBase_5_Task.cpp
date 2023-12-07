#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <sstream> 
#include <iomanip>
#include <vector>
#include <exception>

class Date
{
public:
    Date(int year, int month, int day):
    m_year(year), 
    m_month(month), m_day(day)
    {
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
    int m_year;
    int m_month;
    int m_day;
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
}

class Database
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        dataBase[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const std::string& event)
    {
        if (dataBase[date].count(event) != 0)
        {
            dataBase[date].erase(event);
            std::cout << "Deleted successfully" << std::endl;
            return true;
        }
        std::cout << "Event not found" << std::endl;
        return false;
    }

    int  DeleteDate(const Date& date)
    {
        int countEventOnDate = dataBase[date].size();
        dataBase[date].clear();
        std::cout << "Deleted" << " " << countEventOnDate << " " << "events" << std::endl;
        return countEventOnDate;
    }
  
    void Find(const Date& date) const
    {
        for (const auto& event : dataBase.at(date))
        {
            std::cout << event << std::endl;
        }
    }
    
    void Print() const
    {
        for (const auto& [date, events] : dataBase)
        {
            for (const auto& event : events)
            {
                std::cout << std::setfill('0') << std::setw(4)<<date.GetYear() << "-";
                std::cout << std::setfill('0') << std::setw(2)<<date.GetMonth() << "-";
                std::cout << std::setfill('0') << std::setw(2)<<date.GetDay() << " ";
                std::cout << event << std::endl;
            }
        }
    }
private:
    std::map<Date, std::set<std::string>> dataBase {};
};

Date date_prossesing(std::string& date)
{
    std::string partDate = "";
    std::vector <int> shaterredDate (3);
    for (size_t i = 0, j = 0; i != date.size(); ++i)
    {
        if (date[i] != '-' || 
            (i + 1 < date.size()  && i > 0 && date[i + 1] != '-' && date[i - 1] == '-') 
                || i == 0)
        {
            partDate+=date[i];
        }
        else
        {
            if ((i - 1 == 0 && date[i - 1] == '-') || i + 1 >= date.size() 
                || ( i + 1 < date.size() && date[i + 1] == '-' && i > 0 && date[i - 1] == '-'))
            {
                std::string message = "Wrong date format: " + date;
                throw std::invalid_argument(message);
            }
            else
            {
                try
                {
                    shaterredDate[j] = stoi(partDate);
                    ++j;
                    partDate = "";
                }
                catch (const std::exception& e)
                {
                    std::string message = "Wrong date format: " + date;
                    throw std::invalid_argument(message);
                }
            }
        }
        if (i + 1 == date.size())
        {
            try
            {
                shaterredDate[j] = stoi(partDate);
                ++j;
                partDate = "";
            }
            catch (const std::exception& e)
            {
                std::string message = "Wrong date format: " + date;
                throw std::invalid_argument(message);
            }
        }
    }
    if (shaterredDate[1] < 1 || shaterredDate[1] > 12)
    {
        throw std::invalid_argument("Month value is invalid: " + std::to_string(shaterredDate[1]));
    }
    if (shaterredDate[2] < 1 || shaterredDate[2] > 31)
    {
        throw std::invalid_argument("Day value is invalid: " + std::to_string(shaterredDate[2]));
    }
    return Date(shaterredDate[0], shaterredDate[1], shaterredDate[2]);
}

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
        std::string nameCommand;
        std::istringstream stream(commandLine);
        stream >> nameCommand;
        if (nameCommand != "Print" && nameCommand != "Add" && nameCommand != "Del" && nameCommand != "Find")
        {
            std::cout<<"Unknown command:"<<" "<<nameCommand<<std::endl;
            return 0;
        }
        if (nameCommand == "Print")
        {
            db.Print();
        }
        else
        {
            try
            {
                std::string dateToString, event;
                stream >> dateToString;
                Date fullDate = date_prossesing(dateToString);
                if (nameCommand == "Add")
                {
                    stream >> event;
                    db.AddEvent(fullDate,event);
                }
                if (nameCommand == "Del")
                {
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
                if (nameCommand == "Find")
                {
                    db.Find(fullDate);
                }
            }
            catch(const std::exception& e)
            {
                std::cout << e.what() << std::endl;
                return 0;
            }
        }
    }
    return 0;
}