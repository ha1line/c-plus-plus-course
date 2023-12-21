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
  Date(const std::string& dateStr)
   {
       std::istringstream ss(dateStr);
       char delimiter;
       ss >> m_year  >> delimiter >> m_month  >> delimiter >> m_day;
       validateDate(ss, delimiter, dateStr);
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
  friend bool operator<(const Date& lhs, const Date& rhs)
  {
  return std::make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
         std::make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
  }

private:
   void validateDate(std::istringstream& ss, char delimiter, const std::string& dateStr) const
   {
     if (ss.fail() || delimiter != '-' || !ss.eof())
     {
       throw std::invalid_argument("Wrong date format: " + dateStr);
     }
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


class Database
{
public:
  void AddEvent(const Date& date, const std::string& event)
  {
    storage[date].insert(event);
  }

  bool DeleteEvent(const Date& date, const std::string& event)
  {
    if (storage.count(date) > 0 && storage[date].count(event) > 0) 
    {
      storage[date].erase(event);
      return true;
    }
    return false;
  }

  int DeleteDate(const Date& date)
  {
    if (storage.count(date) > 0) 
    {
      int n = storage[date].size();
      storage.erase(date);
      return n;
    }
    return 0;
  }

  std::set<std::string> Find(const Date& date) const
  {
    if (storage.count(date) > 0) 
    {
      return storage.at(date);
    }
    else
    { 
      return {};
    }
  }
  
  void Print() const {
          for (const auto& item : storage) {
              for (const std::string& event : item.second) {
                  std::cout << std::setw(4) << std::setfill('0') << item.first.GetYear() <<
                      "-" << std::setw(2) << std::setfill('0') << item.first.GetMonth() <<
                      "-" << std::setw(2) << std::setfill('0') << item.first.GetDay() <<
                      " " << event << std::endl;
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
          db.AddEvent(Date(dateStr), event);
      }
      else if (command == "Del") 
      {
          std::string dateStr, event;
          ss >> dateStr;

          if (ss >> event)
          {
              if (db.DeleteEvent(Date(dateStr), event))
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
              int deletedCount = db.DeleteDate(Date(dateStr));
              std::cout << "Deleted " << deletedCount << " events" << std::endl;
          }
        }
      else if (command == "Find") 
      {
        std::string dateStr;
        ss >> dateStr;
        for (const std::string& event : db.Find(dateStr)) 
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
