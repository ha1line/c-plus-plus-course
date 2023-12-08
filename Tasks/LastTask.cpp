#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

class Date
{
public:
	Date(int year, int month, int day);

	int GetYear() const noexcept;
	int GetMonth() const noexcept;
	int GetDay() const noexcept;
private:
	const int m_year;
	const int m_month;
	const int m_day;
};

/* Region: Date implementation */

Date::Date(int year, int month, int day) :
	m_year(year),
	m_month(month),
	m_day(day)
{
	if (month < 1 or month > 12)
	{
		std::cout << "Month value is invalid: " << month << '\n';
		throw std::bad_cast();
	}
	else if (day < 1 or day > 31)
	{
		std::cout << "Day value is invalid: " << day << '\n';
		throw std::bad_cast();
	}
}

int Date::GetYear() const noexcept
{
	return m_year;
}

int Date::GetMonth() const noexcept
{
	return m_month;
}

int Date::GetDay() const noexcept
{
	return m_day;
}

constexpr bool operator<(const Date& lhs, const Date& rhs) noexcept
{
	return (std::make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) <
		std::make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay()));
}

std::ostream& operator<<(std::ostream& out, const Date& date) noexcept
{
	out << (date.GetYear() < 0 ? "-" : "") << std::setw(4) << std::setfill('0')
		<< std::abs(date.GetYear()) << '-' << std::setw(2) << std::setfill('0')
		<< date.GetMonth() << '-' << std::setw(2) << std::setfill('0') << date.GetDay();
	return out;
}

std::istream& operator>>(std::istream& is, std::shared_ptr<Date>& date) noexcept(false)
{
	std::string date_buffer;
	is >> date_buffer;
	std::istringstream date_is(date_buffer);
	constexpr auto read_number = [](std::istream& is) noexcept(false)
		{
			int number;
			is >> number;
			if (is.fail() or (is.peek() != '-' and not is.eof()))
				throw std::runtime_error("");
			is.ignore(1);
			return number;
		};
	try
	{
		int year = read_number(date_is);
		int month = read_number(date_is);
		int day = read_number(date_is);
		date.reset(new Date(year, month, day));
	}
	catch (const std::runtime_error&)
	{
		std::cout << "Wrong date format: " << date_buffer << '\n';
		throw;
	}
	return is;
}

/* End of region: Date implementation */

class Database
{
public:
	void AddEvent(const Date& date, const std::string& event) noexcept;
	bool DeleteEvent(const Date& date, const std::string& event) noexcept;
	size_t  DeleteDate(const Date& date) noexcept;

	std::set<std::string> Find(const Date& date) const noexcept;

	void Print() const noexcept;
private:

	std::map<Date, std::set<std::string>> m_db;
};

/* Region: Database implementation */

void Database::AddEvent(const Date& date, const std::string& event) noexcept
{
	m_db[date].insert(event);
}

bool Database::DeleteEvent(const Date& date, const std::string& event) noexcept
{
	if (m_db.contains(date) and m_db[date].contains(event))
	{
		m_db[date].erase(event);
		return true;
	}
	return false;
}

size_t Database::DeleteDate(const Date& date) noexcept
{
	if (not m_db.contains(date))
		return 0;
	size_t result = m_db[date].size();
	m_db.erase(date);
	return result;
}

std::set<std::string> Database::Find(const Date& date) const noexcept
{
	if (m_db.contains(date))
		 return m_db.at(date);
	return {};
}

void Database::Print() const noexcept
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

void handle_request(Database& db, std::istringstream& is) noexcept(false)
{
	std::string command;
	is >> command;
	if (command == "Add")
	{
		std::string event;
		std::shared_ptr<Date> date;
		is >> date >> event;
		db.AddEvent(*date, event);
	}
	else if (command == "Del")
	{
		std::string event;
		std::shared_ptr<Date> date;
		is >> date;
		if (not is.eof())
		{
			is >> event;
			if (db.DeleteEvent(*date, event))
				std::cout << "Deleted successfully\n";
			else
				std::cout << "Event not found\n";
		}
		else
		{
			std::cout << "Deleted " << db.DeleteDate(*date) << " events\n";
		}
	}
	else if (command == "Find")
	{
		std::shared_ptr<Date> date;
		is >> date;
		for (const auto& each : db.Find(*date))
		{
			std::cout << each << '\n';
		}
	}
	else if (command == "Print")
	{
		db.Print();
	}
	else
	{
		std::cout << "Unknown command: " << command << '\n';
		throw std::runtime_error("");
	}
}

int main() noexcept
{
	Database db;

	std::string commandLine;
	while (std::getline(std::cin, commandLine))
	{
		if (commandLine == "")
			continue;
		try
		{
			std::istringstream line(commandLine);
			handle_request(db, line);
		}
		catch (const std::exception&)
		{
			break;
		}
	}

	return 0;
}

/* End of region: Base functions */
