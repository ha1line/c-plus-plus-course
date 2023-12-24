#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <tuple>

class Date
{
public:
  Date(const int day, const int month, const int year) :
   m_day(day), 
   m_month(month), 
   m_year(year)
  {
    validateDate();
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
  bool operator<(const Date& other) const
  {
    return std::tie(m_year, m_month, m_day) < 
           std::tie(other.m_year, other.m_month, other.m_day);
  }
private:
   void validateDate() const
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

  int m_year;
  int m_month;
  int m_day;
};

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    os << std::setw(4) << std::setfill('0') << date.GetYear() << "-"
       << std::setw(2) << std::setfill('0') << date.GetMonth() << "-"
       << std::setw(2) << std::setfill('0') << date.GetDay();
    return os;
}

Date parseDate(const std::string& dateStr)
{
  std::istringstream ss(dateStr);
  int day, month, year;
  char delimiter;
  ss >> year >> delimiter >> month >> delimiter >> day;
  if (ss.fail() || delimiter != '-' || !ss.eof())
  {
    throw std::invalid_argument("Wrong date format: " + dateStr);
  }
  return Date(day, month, year);
}

class Database
{
public:
  void AddEvent(const Date& date, const std::string& event)
  {
    storage[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const std::string& event)
  {
    if (storage.contains(date) && storage[date].contains(event)) 
    {
      storage[date].erase(event);
      return true;
    }
    return false;
  }

  int DeleteDate(const Date& date)
  {
    if (storage.contains(date)) 
    {
      int n = storage[date].size();
      storage.erase(date);
      return n;
    }
    return 0;
  }

  std::set<std::string> Find(const Date& date) const
  {
    if (storage.contains(date)) 
    {
      return storage.at(date);
    }
    return {};
  }
  
  void Print() const {
    for (const auto& item : storage) {
        for (const std::string& event : item.second) {
            std::cout << item.first << " " << event << std::endl;
        }
    }
}
private:
  std::map<Date, std::set<std::string>> storage;
};


int main()
{
  Database db;

  std::string commandLine;
  while (std::getline(std::cin, commandLine)) 
  {
    std::istringstream ss(commandLine);
    std::string command;
    ss >> command;
    try 
    {
      if (commandLine.empty()) 
      {
        continue;
      }

      if (command == "Add") 
      {
          std::string dateStr, event;
          ss >> dateStr >> event;
          db.AddEvent(parseDate(dateStr), event);
      }

      else if (command == "Del") 
      {
          std::string dateStr, event;
          ss >> dateStr;

          if (ss >> event)
          {
              if (db.DeleteEvent(parseDate(dateStr), event))
              {
                 std::cout << "Deleted successfully" << std::endl;
              }
              else
              {
                 std::cout << "Event not found" << std::endl;
              }
          }
          else
          {
              int deletedCount = db.DeleteDate(parseDate(dateStr));
              std::cout << "Deleted " << deletedCount << " events" << std::endl;
          }
        }

      else if (command == "Find") 
      {
        std::string dateStr;
        ss >> dateStr;
        for (const std::string& event : db.Find(parseDate(dateStr))) 
        {
          std::cout << event << std::endl;
        }
      }

      else if (command == "Print") 
      {
          db.Print();
      }
      else {
          throw std::invalid_argument("Unknown command: " + command);
      }
    }
    
    catch (const std::invalid_argument& e)
    {
       std::cout << e.what() << "\n";
       return 0;
    }
  }
  return 0;
}
