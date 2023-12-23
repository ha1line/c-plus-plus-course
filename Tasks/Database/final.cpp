#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <memory>
#include <regex>

static const void CheckDate(std::string date)
{
    static const std::regex r(R"(^(-?\d{1,4})-([+-]?\d+)-([\+-]?\d+)$)");
    std::smatch match;
    bool isCorrectDate = std::regex_search(date, match, r);

    if (!isCorrectDate)
    {
        throw std::runtime_error("Wrong date format: " + date);
    }

    int year = std::stoi(match[1]);
    int month = std::stoi(match[2]);
    int day = std::stoi(match[3]);

    if ((month > 12) or (month <= 0))
    {
        throw std::runtime_error("Month value is invalid: " + std::to_string(month));
    }
    else if (day > 31 or day <= 0) {
        throw std::runtime_error("Day value is invalid: " + std::to_string(day));
    }

    return;
}


class Write {
public:
    Write(std::string in_date, std::string in_date_event)
    {
        date = in_date;
        date_event = in_date_event;
    }

    std::string GetDate()
    {
        return date;
    }

    std::string GetDateEvent()
    {
        return date_event;
    }



private:
    std::string date;
    std::string date_event;
};

static bool comparator(std::shared_ptr<Write>& a, std::shared_ptr<Write>& b)
{
    return a->GetDateEvent() < b->GetDateEvent();


}

static bool printComparator(std::shared_ptr<Write>& a, std::shared_ptr<Write>& b)
{
    if (a->GetDate() == b->GetDate())
    {
        return a->GetDateEvent() < b->GetDateEvent();
    }
    else {
        return a->GetDate() < b->GetDate();
    }


}


static bool comparatorToUnique(std::shared_ptr<Write>& a, std::shared_ptr<Write>& b)
{
    return a->GetDateEvent() == b->GetDateEvent() and a->GetDate() == b->GetDate();
}

class Database
{
public:
    void CreateWrite(std::istringstream& stream)
    {
        std::shared_ptr<Write> A;

        std::string date;
        std::string date_event;
        stream >> date >> date_event;

        try {
            CheckDate(date);
            static const std::regex r(R"(^(-?\d{1,4})-([+-]?\d+)-([\+-]?\d+)$)");
            std::smatch match;
            bool isCorrectDate = std::regex_search(date, match, r);
            std::string year = match[1];
            std::string month = match[2];
            std::string day = match[3];

            while (day.length() < 2)
            {
                day = '0' + day;
            }

            while (month.length() < 2)
            {
                month = '0' + month;
            }

            while (year.length() < 4)
            {
                year = '0' + year;
            }

            date = year + "-" + month + "-" + day;

            this->writesVector.push_back(std::make_shared<Write>(date, date_event));
            std::vector<std::shared_ptr<Write>>::iterator ip;

            ip = std::unique(this->writesVector.begin(), this->writesVector.end(), comparatorToUnique);
            this->writesVector.resize(std::distance(this->writesVector.begin(), ip));
        }
        catch (const std::runtime_error ex)
        {
            throw ex;
        }
    }

    void Print()
    {
        std::vector<std::shared_ptr<Write>> tmp_vec = this->writesVector;

        sort(this->writesVector.begin(), this->writesVector.end(), printComparator);

        for (const auto& current_write : this->writesVector) {
            std::cout << std::fixed << std::setprecision(3)
                << current_write->GetDate() << " "
                << current_write->GetDateEvent() << std::endl;
        }

        this->writesVector = tmp_vec;;
    }

    void FindWrites(std::string date)
    {
        CheckDate(date);
        std::vector<std::shared_ptr<Write>> tmp_vec = this->writesVector;

        static const std::regex r(R"(^(-?\d{1,4})-([+-]?\d+)-([\+-]?\d+)$)");
        std::smatch match;
        bool isCorrectDate = std::regex_search(date, match, r);
        std::string year = match[1];
        std::string month = match[2];
        std::string day = match[3];

        while (day.length() < 2)
        {
            day = '0' + day;
        }

        while (month.length() < 2)
        {
            month = '0' + month;
        }

        while (year.length() < 4)
        {
            year = '0' + year;
        }

        date = year + "-" + month + "-" + day;

        sort(this->writesVector.begin(), this->writesVector.end(), comparator);
        for (const auto& current_write : this->writesVector) {

            if (current_write->GetDate() == date)
            {
                std::cout << current_write->GetDateEvent() << std::endl;
            }
        }

        this->writesVector = tmp_vec;

    }

    void DeleteWrites(std::istringstream& stream)
    {
        std::string date, date_event;
        stream >> date >> date_event;
        CheckDate(date);

        static const std::regex r(R"(^(-?\d{1,4})-([+-]?\d+)-([\+-]?\d+)$)");
        std::smatch match;
        bool isCorrectDate = std::regex_search(date, match, r);
        std::string year = match[1];
        std::string month = match[2];
        std::string day = match[3];

        while (day.length() < 2)
        {
            day = '0' + day;
        }

        while (month.length() < 2)
        {
            month = '0' + month;
        }

        while (year.length() < 4)
        {
            year = '0' + year;
        }

        date = year + "-" + month + "-" + day;

        if (date_event.empty())
        {
            int deletedDates = 0;
            int currentIndex = 0;
            std::vector<std::shared_ptr<Write>> newWritesVector;
            for (const auto& current_write : this->writesVector) {
                if (current_write->GetDate() != date)
                {

                    newWritesVector.push_back(current_write);
                }
                else {
                    deletedDates += 1;
                }
            }

            this->writesVector = newWritesVector;
            std::cout << "Deleted " << deletedDates << " events" << std::endl;
        }
        else {
            std::string outputString = "Event not found";;
            int currentIndex = 0;
            for (const auto& current_write : this->writesVector) {
                if (current_write->GetDate() == date and current_write->GetDateEvent() == date_event)
                {
                    outputString = "Deleted successfully";
                    this->writesVector.erase(this->writesVector.begin() + currentIndex);
                    break;
                }
                currentIndex += 1;
            }

            std::cout << outputString << std::endl;
        }
    }

private:
    std::vector<std::shared_ptr<Write>> writesVector;

};


int main() {
    Database db;
    for (std::string line; std::getline(std::cin, line); ) {
        std::istringstream is(line);

        std::string command;
        is >> command;
        if (command == "Add") {
            is >> std::ws;
            try {
                db.CreateWrite(is);
            }
            catch (const std::runtime_error ex) {
                std::cout << ex.what() << std::endl;
                return 0;
            }

        }
        else if (command == "Print")
        {
            try {
                db.Print();
            }
            catch (const std::runtime_error ex) {
                std::cout << ex.what() << std::endl;
                return 0;
            }
        }

        else if (command == "Find")
        {
            std::string date;
            is >> date;
            try {
                db.FindWrites(date);
            }
            catch (const std::runtime_error ex) {
                std::cout << ex.what() << std::endl;
                return 0;
            }
        }
        else if (command == "Del")
        {
            try {
                db.DeleteWrites(is);
            }
            catch (const std::runtime_error ex) {
                std::cout << ex.what() << std::endl;
                return 0;
            }
        }
        else if (!command.empty())
        {
            std::cout << "Unknown command: " << command << std::endl;
            return 0;
        }

    }
    return 0;
}