#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>

// нужно заполнить методы и перегрузить нужные операторы
class Date
{
public:
  Date(int year, int month, int day) : m_year(year), m_month(month), m_day(day) {}

  int GetYear() const {return m_year;};
  int GetMonth() const {return m_month;};
  int GetDay() const {return m_day;};

private:
  int m_year, m_month, m_day;
};

// здесь советую воспользоваться трюком с вектором из лекции, либо std::tie
bool operator<(const Date& lhs, const Date& rhs)
{
  if (lhs.GetYear() != rhs.GetYear()) return lhs.GetYear() < rhs.GetYear();
      if (lhs.GetMonth() != rhs.GetMonth()) return lhs.GetMonth() < rhs.GetMonth();
      return lhs.GetDay() < rhs.GetDay();
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
    auto item = m_data.find(date);
    if(item != m_data.end())
    {
      auto item1 = item->second.find(event);
        if(item1 != item->second.end())
        {
          item->second.erase(item1);
          return true;
        }
    }
    return false;
  }
  int DeleteDate(const Date& date)
  {
    auto item = m_data.find(date);
    if(item != m_data.end())
    {
      int n = item->second.size();
      m_data.erase(item);
      return n;
    }

    return 0;
  }

  void Find(const Date& date) const
  {
    auto item = m_data.find(date);
    if(item != m_data.end())
    {
      for(const std::string& event : item->second)
      {
        std::cout << event << std::endl;
      }
    }
  }
  
  void Print() const
  {
    for (const auto& date_events : m_data)
    {
      for (const std::string& event : date_events.second)
      {
        std::cout << std::setfill('0') << std::setw(4) << date_events.first.GetYear() << "-"
          << std::setw(2) << date_events.first.GetMonth() << "-"
          << std::setw(2) << date_events.first.GetDay() << " "
          << event << std::endl;
      }
    }
  }

private:
  std::map<Date, std::set<std::string>> m_data;
};

int main()
{
  Database db;
  std::string commandLine, command, date, event, afterDay;
  int year, month, day;
  char dash;
  
  while (std::getline(std::cin, commandLine))
  {
    if(commandLine.empty())
      continue;
    std::istringstream is(commandLine);
    
    is >> command;

    if(command == "Add")
    {
      is >> date >> event;
      std::istringstream dateIs(date);
      if (!(dateIs >> year >> dash >> month >> dash >> day) || dateIs >> afterDay)
        std::cout << "Wrong date format: " << date << std::endl;
      else if (month < 1 || month > 12)
        std::cout << "Month value is invalid: " << month << std::endl;
      else if (day < 1 || day > 31)
        std::cout << "Day value is invalid: " << day << std::endl;
      else
        db.AddEvent(Date(year, month, day), event);
  }

  else if(command == "Del")
  {
    is >> date;
    std::istringstream dateIs(date);
    if (!(dateIs >> year >> dash >> month >> dash >> day))
        std::cout << "Wrong date format: " << date << std::endl;
    else if (month < 1 || month > 12)
      std::cout << "Month value is invalid: " << month << std::endl;
    else if (day < 1 || day > 31)
      std::cout << "Day value is invalid: " << day << std::endl;

    else if(is >> event)
    {
      if(db.DeleteEvent(Date(year, month, day), event))
        std::cout << "Deleted successfully" << std::endl;
      else
        std::cout << "Event not found" << std::endl;
    }
    else
    {
      int n = db.DeleteDate(Date(year, month, day));
      std::cout << "Deleted " << n << " events" << std::endl;
    }
  }

  else if(command == "Find")
  {
    is >> date;
    std::istringstream dateIs(date);
    if (!(dateIs >> year >> dash >> month >> dash >> day))
        std::cout << "Wrong date format: " << date << std::endl;
    else if (month < 1 || month > 12)
      std::cout << "Month value is invalid: " << month << std::endl;
    else if (day < 1 || day > 31)
      std::cout << "Day value is invalid: " << day << std::endl;
    else 
      db.Find(Date(year, month, day));
  }

  else if(command == "Print")
    db.Print();

  else
    std::cout << "Unknown command: " << command << std::endl;
  }
  
  return 0;
}