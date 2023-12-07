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
	Date(i32 year, i32 month, i32 day);

	// No copy / move constructor & operator=, 'cause trivially copyable / moveable

	i32 GetYear() const;
	i32 GetMonth() const;
	i32 GetDay() const;
private:
	const i32 m_year;
	const i32 m_month;
	const i32 m_day;
};

/* Region: Date implementation */

Date::Date(i32 year, i32 month, i32 day) :
	m_year(year),
	m_month(month),
	m_day(day)
{
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
	if (lhs.GetYear() < rhs.GetYear())
	{
		return true;
	}
	else if (lhs.GetYear() == rhs.GetYear() and lhs.GetMonth() < rhs.GetMonth())
	{
		return true;
	}
	else if (lhs.GetYear() == rhs.GetYear() and
			 lhs.GetMonth() == rhs.GetMonth() and
			 lhs.GetDay() < rhs.GetDay())
	{
		return true;
	}
	return false;
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
		return false;
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

Date read_date(const std::string& date) noexcept(false)
{
	std::stringstream date_stream(date);
	i32 year, month, day;
	char delemiter;
	std::streampos index = date_stream.tellg();
	date_stream >> year;
	if (date_stream.tellg() - index > 4)
	{
		std::cout << "Wrong date format: " << date << '\n';
		throw std::runtime_error("Wrong date format");
	}
	index = date_stream.tellg();
	date_stream >> delemiter >> month;
	if (delemiter != '-' or date_stream.tellg() - index > 3)
	{
		std::cout << "Wrong date format: " << date << '\n';
		throw std::runtime_error("Wrong date format");
	}
	index = date_stream.tellg();
	date_stream >> delemiter >> day;
	if (delemiter != '-' or date_stream.fail() or date_stream.tellg() - index > 3)
	{
		std::cout << "Wrong date format: " << date << '\n';
		throw std::runtime_error("Wrong date format");
	}
	else if (month < 1 or month > 12)
	{
		std::cout << "Month value is invalid: " << month << '\n';
		throw std::runtime_error("Wrong month value");
	}
	else if (day < 1 or day > 31)
	{
		std::cout << "Day value is invalid: " << day << '\n';
		throw std::runtime_error("Wrong day value");
	}
	if (date_stream)
	{
		std::cout << "Wrong date format: " << date << '\n';
		throw std::runtime_error("Wrong date format");
	}
	return Date(year, month, day);
}

bool Add(Database& db, std::stringstream& line)
{
	std::string date_s, event;
	try
	{
		line >> date_s;
		Date date = read_date(date_s);
		line >> event;
		db.AddEvent(date, event);
		return true;
	}
	catch (const std::runtime_error&)
	{
		return false;
	}
}

bool Del(Database& db, std::stringstream& line)
{
	std::string date_s, event;
	try
	{
		line >> date_s;
		Date date = read_date(date_s);
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
		return true;
	}
	catch (const std::runtime_error&)
	{
		return false;
	}
}

bool Find(Database& db, std::stringstream& line)
{
	std::string date_s;
	try
	{
		line >> date_s;
		Date date = read_date(date_s);
		for (const auto& each : db.Find(date))
		{
			std::cout << each << '\n';
		}
		return true;
	}
	catch (const std::runtime_error&)
	{
		return false;
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
		if (command == "Add") 
		{
			if (not Add(db, line)) break;
		}
		else if (command == "Del") 
		{
			if (not Del(db, line)) break;
		}
		else if (command == "Find") 
		{
			if (not Find(db, line)) break;
		}
		else if (command == "Print") db.Print();
		else
		{
			std::cout << "Unknown command: " << command << '\n';
			break;
		}
	}

	return 0;
}

/* End of region: Base functions */
