// вы можете подключить все необходимые вам библиотеки из стандарта
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <iomanip>

/* TODO 
    1) потоковый ввод в main
    2) обработка ошибок
    3) перегрузить оператор < (что он должен делать, я немного не понял)
*/

// нужно заполнить методы и перегрузить нужные операторы
class Date
{
public:
  Date(int d_year, int d_month, int d_day) {
    year = d_year;
    month = d_month;
    day = d_day;
  };
  int GetYear() const
  {
    return year;
  };

  int GetMonth() const
  {
    return month;
  };

  int GetDay() const
  {
    return day;
  };
private:
  int year;
  int month;
  int day;
};

// здесь советую воспользоваться трюком с вектором из лекции, либо std::tie
bool operator<(const Date& lhs, const Date& rhs)
{
  if (lhs.GetYear() != rhs.GetYear())
  {
    return lhs.GetYear() < rhs.GetYear();
  }
  if (lhs.GetMonth() != rhs.GetMonth())
  {
    return lhs.GetMonth() < rhs.GetMonth();
  }
  return lhs.GetDay() < rhs.GetDay();
};

class Database
{
public:
  void AddEvent(const Date& date, const std::string& event)
  {
    data[date].insert(event);
  };

  bool DeleteEvent(const Date& date, const std::string& event)
  {
  auto it = data.find(date);
  if (it != data.end())
  {
    auto& events = it->second;
    auto event_it = events.find(event);
    if (event_it != events.end())
    {
      events.erase(event_it);
      if (events.empty())
      {
        data.erase(it);
      }
      return true;
    }
  }
  return false;
  };
  
  int DeleteDate(const Date& date)
  {
    auto it = data.find(date);
    if (it != data.end())
    {
      int events_count = it->second.size();
      data.erase(it);
      return events_count;
    }
    return 0;
  };

  // будет зависеть от контейнера, который вы выберете
  /* ??? */ //Find(const Date& date) const;
  
  void Print() const
  {
    for (const auto& date_events : data)
    {
      const Date& date = date_events.first;
      const auto& events = date_events.second;
      std::cout << date.GetYear() << "-" << std::setw(2) << std::setfill('0') << date.GetMonth() << "-" << std::setw(2) << std::setfill('0') << date.GetDay();
      for (const auto& event : events)
      {
        std::cout << " " << event;
      }
      std::cout << std::endl;
    }
  };
private:
  std::map<Date, std::set<std::string>> data;
};

int main()
{
  Database db;

  std::string commandLine;
  while (std::getline(std::cin, commandLine))
  {
    std::istringstream iss(commandLine);
    std::string operation;
    iss >> operation;

    if (operation == "Add") 
    {
        is >> std::ws;
    } 
    else if (operation == "Del") 
    {
        //del
    } 
    else if (operation == "Find") 
    {
        //find
    } 
    else if (operation == "Print") 
    {
        //print
    } 
    else 
    {
        std::cout << "Unknown command: " << operation << std::endl;
    }
  }

  return 0;
}
