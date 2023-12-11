#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>

class Date
{
public:
    Date() = default;

    Date(long long y, int m, int d)
    {
        year = y;

        if (m >= 1 && m <= 12)
            month = m;
        else
        {
            std::cout << "Month value is invalid: " << m << std::endl;
            throw std::runtime_error("Month value is invalid");
        }
        if (d >= 1 && d <= 31)
            day = d;
        else
        {
            std::cout << "Day value is invalid: " << d << std::endl;
            throw std::runtime_error("Day value is invalid");
        }
    }

    long long GetYear() const
    {
        return year;
    }
    int GetMonth() const
    {
        return month;
    }
    int GetDay() const
    {
        return day;
    }

    void PrintDate() const
    {
        long long yr = year;
        if (year < 0)
        {
            std::cout << "-";
            yr = yr * -1;
        }
        if (yr / 1000 != 0)
            std::cout << yr << "-";
        else if (yr / 100 != 0)
            std::cout << "0" << yr << "-";
        else if (yr / 10 != 0)
            std::cout << "00" << yr << "-";
        else
            std::cout << "000" << yr << "-";

        if (month <= 9)
            std::cout << "0";
        std::cout << month << "-";

        if (day <= 9)
            std::cout << "0";
        std::cout << day;
    }

private:
    long long year = 0;
    int month = 1;
    int day = 1;
};

// здесь советую воспользоваться трюком с вектором из лекции, либо std::tie
bool operator<(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() < rhs.GetYear())
        return true;
    else if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() < rhs.GetMonth())
        return true;
    else if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() < rhs.GetDay())
        return true;
    else
        return false;
}

bool operator>(const Date& lhs, const Date& rhs)
{
    if (lhs.GetYear() > rhs.GetYear())
        return true;
    else if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() > rhs.GetMonth())
        return true;
    else if (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() > rhs.GetDay())
        return true;
    else
        return false;
}

bool operator==(const Date& lhs, const Date& rhs)
{
    return lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth() && lhs.GetDay() == rhs.GetDay();
}

std::ostream& operator<<(std::ostream& os, const Date& date)
{
    os << date.GetYear() << "-" << date.GetMonth() << "-" << date.GetDay();
    return os;
}

class Database
{
public:
    void AddEvent(const Date& date, const std::string& event)
    {
        container[date].insert(event);
    }

    void DeleteEvent(const Date& date, const std::string& event)
    {
        if (container[date].find(event) != container[date].end())
        {
            container[date].erase(event);
            std::cout << "Deleted successfully" << std::endl;
        }
        else
            std::cout << "Event not found" << std::endl;
    }

    void DeleteDate(const Date& date)
    {
        int count = container[date].size();
        container.erase(date);
        std::cout << "Deleted " << count << " events" << std::endl;
    }

    void Find(const Date& date)
    {
        for (auto i : container[date])
            std::cout << i << std::endl;
    }

    void Print() const
    {
        for (auto i : container)
            for (auto j : i.second)
            {
                if (i.first.GetYear() >= 0)
                {
                    i.first.PrintDate();
                    std::cout << " " << j << std::endl;
                }
            }
    }

private:
    std::map<Date, std::set<std::string>> container;
};

bool isNumeric(const std::string& str)
{
    bool plus_skipped = false;
    for (char c : str)
    {
        if (!plus_skipped && str[0] == '+')
        {
            plus_skipped = true;
            continue;
        }
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return true;
}

Date DateParsing(std::string& str)
{
    std::string original_str = str;
    bool after_christ = true;
    if (str[0] == '-')
    {
        after_christ = false;
        str.erase(0, 1);
    }
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> args;
    std::vector<long long> numbers;
    while (getline(ss, item, '-'))
        args.push_back(item);

    if (!after_christ && args[0][0] == '+')
    {
        std::cout << "Wrong date format: " << original_str << std::endl;
        throw std::runtime_error("Wrong date format");
    }

    if (args.size() != 3)
    {
        /*
        1--2-3
        1-2--3
        1--2--3
        */
        try
        {
            if (args.size() == 5 && args[1] == "" && args[3] == "" && args[0] != "" && args[2] != "" && args[4] != "" ||
                args.size() == 4 && args[1] == "" && args[0] != "" && args[2] != "" && args[3] != "")
            {
                for (auto t : args)
                {
                    if (t != "")
                    {
                        if (!isNumeric(t))
                        {
                            std::cout << "Wrong date format: " << original_str << std::endl;
                            throw std::runtime_error("Wrong date format");
                        }
                        numbers.push_back(std::stoll(t));
                    }
                }
                std::cout << "Month value is invalid: " << "-" << args[2] << std::endl;
                throw std::runtime_error("Month value is invalid");
            }
            else if (args.size() == 4 && args[2] == "" && args[0] != "" && args[1] != "" && args[3] != "")
            {
                if (!isNumeric(args[0]) || !isNumeric(args[1]) || !isNumeric(args[2]) || !isNumeric(args[3]))
                {
                    std::cout << "Wrong date format: " << original_str << std::endl;
                    throw std::runtime_error("Wrong date format");
                }

                numbers.push_back(std::stoll(args[0]));
                if (std::stoi(args[1]) >= 1 && std::stoi(args[1]) <= 12)
                    numbers.push_back(std::stoi(args[1]));
                else
                {
                    std::cout << "Month value is invalid: " << args[1] << std::endl;
                    throw std::runtime_error("Month value is invalid");
                }
                numbers.push_back(std::stoi(args[3]));

                std::cout << "Day value is invalid: " << "-" << args[3] << std::endl;
                throw std::runtime_error("Day value is invalid");
            }
        }
        catch (std::invalid_argument& e)
        {
            std::cout << "Wrong date format: " << original_str << std::endl;
            throw std::runtime_error("Wrong date format");
        }

        std::cout << "Wrong date format: " << original_str << std::endl;
        throw std::runtime_error("Wrong date format");
    }
    else
    {
        try
        {
            if (!isNumeric(args[0]) || !isNumeric(args[1]) || !isNumeric(args[2]))
            {
                std::cout << "Wrong date format: " << original_str << std::endl;
                throw std::runtime_error("Wrong date format");
            }
            if (after_christ)
                numbers.push_back(std::stoll(args[0]));
            else
                numbers.push_back(-1 * std::stoll(args[0]));
            numbers.push_back(std::stoi(args[1]));
            numbers.push_back(std::stoi(args[2]));

            Date d(numbers[0], numbers[1], numbers[2]);
            return d;
        }
        catch (std::invalid_argument& e)
        {
            std::cout << "Wrong date format: " << original_str << std::endl;
            throw std::runtime_error("Wrong date format");
        }
    }
}

int main()
{
    Database db;

    std::string commandLine;

   while (std::getline(std::cin, commandLine))
    {
        // построчно читаем команды из потока
        std::istringstream iss(commandLine);
        std::string word;

        while (iss >> word)
        {
            if (word == "Add")
            {
                try
                {
                    std::string arg1, arg2;
                    iss >> arg1 >> arg2;
                    Date d = DateParsing(arg1);

                    if (&d != nullptr)
                    {
                        db.AddEvent(d, arg2);
                    }
                }
                catch (std::exception& e)
                {
                    return 0;
                }
            }
            else if (word == "Del")
            {
                try
                {
                    std::string arg1, arg2;
                    iss >> arg1 >> arg2;
                    Date d = DateParsing(arg1);
                    if (arg2 == "")
                    {
                        db.DeleteDate(d);
                    }
                    else
                    {
                        db.DeleteEvent(d, arg2);
                    }
                }
                catch (std::exception& e)
                {
                    return 0;
                }
            }
            else if (word == "Find")
            {
                try
                {
                    std::string arg;
                    iss >> arg;
                    Date d = DateParsing(arg);
                    if (&d != nullptr)
                    {
                        db.Find(d);
                    }
                }
                catch (std::exception& e)
                {
                    return 0;
                }
            }
            else if (word == "Print")
            {
                db.Print();
            }
            else
            {
                std::cout << "Unknown command: " << word << std::endl;
                return 0;
            }
        }
    }

    return 0;
}
