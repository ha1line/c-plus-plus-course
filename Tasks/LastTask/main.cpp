#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include <tuple>
#include <iomanip>

class Date
{
public:
	Date(const int year, const int month, const int day) : m_Year(year), m_Month(month), m_Day(day)
	{
		if (m_Month < 1 || m_Month > 12)
		{
			throw std::out_of_range("Month value is invalid: " + std::to_string(m_Month));
		}

		if (m_Day < 1 || m_Day > 31)
		{
			throw std::out_of_range("Day value is invalid: "+ std::to_string(m_Day));
		}
	}

	int GetYear() const noexcept
	{
		return m_Year;
	}

	int GetMonth() const noexcept
	{
		return m_Month;
	}

	int GetDay() const noexcept
	{
		return m_Day;
	}

private:
	int m_Year;
	int m_Month;
	int m_Day;
};

Date parse_date(const std::string& date_str)
{
	std::istringstream is(date_str);
	int year, month, day;

	is >> year;
	if (is.peek() != '-')
	{
		throw std::logic_error("Wrong date format: " + date_str);
	}
	else
	{
		is.ignore(1);
	}

	is >> month;
	if (is.peek() != '-')
	{
		throw std::logic_error("Wrong date format: " + date_str);
	}
	else
	{
		is.ignore(1);
	}

	if (!(is >> day && is.eof()))
	{
		throw std::logic_error("Wrong date format: " + date_str);
	}

	return Date(year, month, day);
}

bool operator<(const Date& lhs, const Date& rhs) noexcept
{
	return std::make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
			std::make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

std::ostream& operator<<(std::ostream& ostream, const Date& date)
{
	ostream << (date.GetYear() < 0 ? "-" : "") << std::setw(4) << std::setfill('0') <<
			   std::abs(date.GetYear()) << "-" << std::setw(2) << std::setfill('0') <<
			   date.GetMonth() << "-" << std::setw(2) << std::setfill('0') <<
			   date.GetDay();
	return ostream;
}

class Database
{
public:
	void AddEvent(const Date& date, const std::string& event) noexcept
	{
		m_map[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const std::string& event) noexcept
	{
		if (m_map[date].count(event))
		{
			m_map[date].erase(event);
			return true;
		}
		return false;
	}
	int DeleteDate(const Date& date) noexcept
	{
		if (m_map.count(date))
		{
			size_t events_count = m_map[date].size();
			m_map.erase(date);
			return int(events_count);
		}
		return 0;
	}

	std::set<std::string> Find(const Date& date) const noexcept
	{
		if (m_map.count(date))
		{
			return m_map.at(date);
		}
		return std::set<std::string>();
	}

	void Print() const noexcept
	{
		for (const auto& e : m_map)
		{
			for (const auto& event : e.second)
			{
				std::cout << e.first << " " << event << std::endl;
			}
		}
	}
private:
	std::map<Date, std::set<std::string>> m_map;
};

void execut_command(const std::string& commandLine, Database& db)
{
	std::istringstream is (commandLine);
	std::string command;
	is >> command;
	if (command.empty())
	{
		return;
	}
	if (command == "Add")
	{
		std::string date_str, event;
		is >> date_str >> event;
		Date date = parse_date(date_str);
		db.AddEvent(date, event);
	}
	else if (command == "Del")
	{
		std::string date_str;
		is >> date_str;
		if (is.eof())
		{
			Date date = parse_date(date_str);
			std::cout << "Deleted " <<db.DeleteDate(date) << " events" << std::endl;
		}
		else
		{
			std::string event;
			is >> event;
			Date date = parse_date(date_str);
			if (db.DeleteEvent(date, event))
			{
				std::cout << "Deleted successfully" << std::endl;
			}
			else
			{
				std::cout << "Event not found"<< std::endl;
			}
		}
	}
	else if (command == "Find")
	{
		std::string date_str;
		is >> date_str;
		Date date = parse_date(date_str);
		for (const auto& row : db.Find(date))
		{
			std::cout << row << std::endl;
		}
	}
	else if (command =="Print")
	{
		db.Print();
	} else
	{
		throw std::runtime_error("Unknown command: " + command);
	}
}

int main()
{
	Database db;
	std::string commandLine;

	while (std::getline(std::cin, commandLine))
	{
		try
		{
			execut_command(commandLine, db);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return 0;
		}
	}
	return 0;
}
