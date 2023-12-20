#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <format>
#include <string_view>
#include <exception>

class Date
{
public:
  Date(int year, int month, int day) : m_year(year), m_month(month), m_day(day)
  {
    if (month < 1 || month > 12)
      throw std::invalid_argument("Month value is invalid: " + std::to_string(month));
    else if (day < 1 || day > 31)
      throw std::invalid_argument("Day value is invalid: " +  std::to_string(day));
  }

  int GetYear() const {return m_year;};
  int GetMonth() const {return m_month;};
  int GetDay() const {return m_day;};

private:
  int m_year, m_month, m_day;
};

bool operator<(const Date& lhs, const Date& rhs)
{
  if (lhs.GetYear() != rhs.GetYear())
    return lhs.GetYear() < rhs.GetYear();
  if (lhs.GetMonth() != rhs.GetMonth()) 
    return lhs.GetMonth() < rhs.GetMonth();
  return lhs.GetDay() < rhs.GetDay();
}

std::ostream& operator<<(std::ostream& os, const Date& date)
  {
    os << std::setfill('0') << std::setw(4) << date.GetYear() << "-"
    << std::setw(2) << date.GetMonth() << "-"
    << std::setw(2) << date.GetDay();
    return os;
  }

class Database
{
public:
  void AddEvent(const Date& date, const std::string& event)
  {
    m_data[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const std::string& event)
  {
    if(m_data.contains(date) && m_data[date].erase(event) != 0)
      return true;
    return false;
  }

  int DeleteDate(const Date& date)
  {
    if(m_data.contains(date))
    {
      int n = m_data[date].size();
      m_data.erase(date);
      return n;
    }
    return 0;
  }

  std::set<std::string> Find(const Date& date)
  {
    if(m_data.contains(date))
      return m_data[date];
    else
      return std::set<std::string>();
  }
  
  void Print() const
  {
    for (const auto& date_events : m_data)
      for (const std::string& event : date_events.second)
        std::cout << date_events.first << " " << event << std::endl;
  }

private:
  std::map<Date, std::set<std::string>> m_data;
};

Date ParseDate(std::istringstream& is)
{
  std::string date, afterDay;
  int year, month, day;
  char dash;
  is >> date;
  std::istringstream dateIs(date);
  if(!(dateIs >> year >> dash >> month >> dash >> day) || dateIs >> afterDay)
    throw std::invalid_argument("Wrong date format: " + date);
  else
    return Date(year, month, day); 
}

bool IsUnknownCommand(const std::string& command)
{
  return command != "Add" && command != "Del" && command != "Find" && command != "Print";
}

int main()
{
  Database db;
  std::string commandLine, command, event;
  Date resultDate = Date(1,1,1);
  
  while (std::getline(std::cin, commandLine))
  {
    if(commandLine.empty())
      continue;
    std::istringstream is(commandLine);
    
    is >> command;
    if(IsUnknownCommand(command))
      std::cout << "Unknown command: " << command << std::endl;
    else if(command == "Print")
      db.Print();
    else
    {
      try
      {
        resultDate = ParseDate(is);
      }
      catch(const std::invalid_argument& e)
      {
        std::cout << e.what() << std::endl;
        continue;
      }
      if(command == "Add")
      {
        is >> event;
        db.AddEvent(resultDate, event);
      }
      else if(command == "Del")
      {
        if(is >> event)
        {
          if(db.DeleteEvent(resultDate, event))
            std::cout << "Deleted successfully" << std::endl;
          else
            std::cout << "Event not found" << std::endl;
        }
        else
        {
          int n = db.DeleteDate(resultDate);
          std::cout << "Deleted " << n << " events" << std::endl;
        }
      }
      else if(command == "Find")
      {
        std::set<std::string> findingEvents = db.Find(resultDate);
        for (const std::string& event : findingEvents)
          std::cout << event << std::endl;
      }
    }
  }
  return 0;
}