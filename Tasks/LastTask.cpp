// Don't use name guards and pragma once 'cause not a header

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <stdexcept>

using i32 = int32_t;
using u64 = uint64_t;

class Date
{
public:
	Date() = default;
	Date(i32 year, i32 month, i32 day);

	i32 GetYear() const;
	i32 GetMonth() const;
	i32 GetDay() const;
private:

	// Not consts 'cause I use operator>> with the date
	i32 m_year = 0;
	i32 m_month = 0;
	i32 m_day = 0;
};

/* Region: Date implementation */

Date::Date(i32 year, i32 month, i32 day) :
	m_year(year),
	m_month(month),
	m_day(day)
{
	if (month < 1 or month > 12)
	{
		std::cout << "Month value is invalid: " << month << '\n';
		throw std::runtime_error("Wrong month value");
	}
	else if (day < 1 or day > 31)
	{
		std::cout << "Day value is invalid: " << day << '\n';
		throw std::runtime_error("Wrong day value");
	}
}

i32 Date::GetYear() const
{
	return m_year;
}

i32 Date::GetMonth() const
{
	return m_month;
}

i32 Date::GetDay() const
{
	return m_day;
}

bool operator<(const Date& lhs, const Date& rhs)
{
	i32 y1 = lhs.GetYear();
	i32 y2 = rhs.GetYear();
	i32 m1 = lhs.GetMonth();
	i32 m2 = rhs.GetMonth();
	i32 d1 = lhs.GetDay();
	i32 d2 = rhs.GetDay();
	return (std::tie(y1, m1, d1) < std::tie(y2, m2, d2));
}

std::ostream& operator<<(std::ostream& out, const Date& date)
{
	// I wanna print negative years anyway.
	// I've read that it's not necessary.
	bool negative = date.GetYear() < 0;
	out << (negative ? "-" : "") << std::setw(4) << std::setfill('0') << std::abs(date.GetYear()) << '-'
		<< std::setw(2) << std::setfill('0') << date.GetMonth() << '-'
		<< std::setw(2) << std::setfill('0') << date.GetDay();
	return out;
}

std::istream& operator>>(std::istream& is, Date& date)
{
	std::string date_buffer;
	is >> date_buffer;
	std::istringstream date_is(date_buffer);
	i32 year, month, day;
	char delimiter;
	std::streampos index = date_is.tellg();
	date_is >> year;
	if (date_is.tellg() - index > 4)
	{
		std::cout << "Wrong date format: " << date_buffer << '\n';
		throw std::runtime_error("Wrong date format");
	}
	index = date_is.tellg();
	date_is >> delimiter >> month;
	if (delimiter != '-' or is.tellg() - index > 3)
	{
		std::cout << "Wrong date format: " << date_buffer << '\n';
		throw std::runtime_error("Wrong date format");
	}
	index = date_is.tellg();
	date_is >> delimiter >> day;
	if (delimiter != '-' or date_is.fail() or date_is.tellg() - index > 3)
	{
		std::cout << "Wrong date format: " << date_buffer << '\n';
		throw std::runtime_error("Wrong date format");
	}
	date = { year, month, day };
	return is;
}

/* End of region: Date implementation */

class Database
{
public:
	void AddEvent(const Date& date, const std::string& event);
	bool DeleteEvent(const Date& date, const std::string& event);
	u64  DeleteDate(const Date& date);

	const std::set<std::string>& Find(const Date& date) const;

	// Not SOLID probably, not good, not sure...
	void Print() const;
private:

	std::map<Date, std::set<std::string>> m_db;
};

/* Region: Database implementation */

void Database::AddEvent(const Date& date, const std::string& event)
{
	m_db[date].insert(event);
}

bool Database::DeleteEvent(const Date& date, const std::string& event)
{
	if (not m_db.contains(date) or not m_db[date].contains(event))
		return false;
	return m_db[date].erase(event) > 0;
}

u64 Database::DeleteDate(const Date& date)
{
	if (not m_db.contains(date))
		return 0;
	u64 result = m_db[date].size();
	m_db[date].clear();
	m_db.erase(date);
	return result;
}

static std::set<std::string> empty_set = {};

const std::set<std::string>& Database::Find(const Date& date) const
{
	if (not m_db.contains(date)) return empty_set;
	return m_db.at(date);
}

void Database::Print() const
{
	for (const auto& [date, events] : m_db)
	{
		for (const auto& event : events)
		{
			std::cout << date << ' ' << event << '\n';
		}
	}
}

/* End of region: Database implementation */

/* Region: Base functions */

void Add(Database& db, std::stringstream& line)
{
	std::string event;
	Date date;
	line >> date >> event;
	db.AddEvent(date, event);
}

void Del(Database& db, std::stringstream& line)
{
	std::string event;
	Date date;
	line >> date;
	if (not line.eof())
	{
		line >> event;
		if (db.DeleteEvent(date, event))
			std::cout << "Deleted successfully\n";
		else
			std::cout << "Event not found\n";
	}
	else
	{
		std::cout << "Deleted " << db.DeleteDate(date) << " events\n";
	}
}

void Find(Database& db, std::stringstream& line)
{
	Date date;
	line >> date;
	for (const auto& each : db.Find(date))
	{
		std::cout << each << '\n';
	}
}

i32 main()
{
	Database db;

	std::string commandLine;
	while (std::getline(std::cin, commandLine))
	{
		if (commandLine == "") continue;
		std::stringstream line(commandLine);
		std::string command;
		line >> command;
		try
		{
			if (command == "Add") Add(db, line);
			else if (command == "Del") Del(db, line);
			else if (command == "Find") Find(db, line);
			else if (command == "Print") db.Print();
			else
			{
				std::cout << "Unknown command: " << command << '\n';
				break;
			}
		}
		catch (const std::runtime_error&)
		{
			break;
		}
	}

	return 0;
}

/* End of region: Base functions */
